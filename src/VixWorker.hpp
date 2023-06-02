#pragma once

#include <napi.h>
#include <vix.h>

#include "SmartHandle.hpp"

#include <cstdio>

namespace vix {

	/**
	 * A async job worker thing for Vix jobs
	 * This expects a promise that will resolve to a C++->JS wrapper type
	 */
	template <class WrapperType>
	struct VixAsyncJobWorker : public Napi::AsyncWorker {
		VixAsyncJobWorker(Napi::Env env, VixHandle jobHandle)
			: Napi::AsyncWorker(env),
			  jobHandle(jobHandle),
			  promise(Napi::Promise::Deferred::New(env)) {
		}

		virtual ~VixAsyncJobWorker() = default;

		void Execute() override {
			// Yes, this is a blocking call. However:
			// - For years (until io_uring), libuv did something similar, with blocking calls no less, for AIO on Linux,
			//      not causing too many problems (less than the... not so good linux AIO calls, at least)
			// - It won't stop the Javascript thread at least
			// - ...

            std::printf("waiting on job\n");
			auto err = VixJob_Wait(jobHandle,
								   WrapperType::VixWaitType,
								   &returnValue,
								   VIX_PROPERTY_NONE);


            std::printf("job done: %s\n",Vix_GetErrorText(err, nullptr) );

			if(err != VIX_OK)
				SetError(Vix_GetErrorText(err, nullptr));
		}

		void OnOK() override {
            printf("OnOK called\n");
			// Create the wrapper object now, since we can't do it in the async thread
			promise.Resolve(WrapperType::Create(returnValue));
		}

		void OnError(const Napi::Error& error) override {
			promise.Reject(error.Value());
		}

		Napi::Promise::Deferred& Promise() {
			return promise;
		}

	   private:
		VixHandle jobHandle;
		typename WrapperType::ReturnType returnValue {};
		Napi::Promise::Deferred promise;
	};

	// in which cases we don't care (the function doesn't return anything)
	template <>
	struct VixAsyncJobWorker<void> : Napi::AsyncWorker {
		VixAsyncJobWorker(Napi::Env env, VixHandle jobHandle)
			: Napi::AsyncWorker(env),
			  jobHandle(jobHandle),
			  promise(Napi::Promise::Deferred::New(env)) {
		}

		virtual ~VixAsyncJobWorker() = default;

		void Execute() override {
			auto err = VixJob_Wait(jobHandle,
								   VIX_PROPERTY_NONE);

			if(err != VIX_OK)
				SetError(Vix_GetErrorText(err, nullptr));
		}

		void OnOK() override {
			// Create the wrapper object now, since we can't do it in the async thread
			promise.Resolve({});
		}

		void OnError(const Napi::Error& error) override {
			promise.Reject(error.Value());
		}

		Napi::Promise::Deferred& Promise() {
			return promise;
		}

	   private:
		VixHandle jobHandle;
		Napi::Promise::Deferred promise;
	};

} // namespace vix
