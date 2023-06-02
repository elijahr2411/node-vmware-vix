#include "VixWorker.hpp"
#include "SmartHandle.hpp"

namespace vix {

	struct VixHost : Napi::ObjectWrap<VixHost> {
		// Needed to use VixWorker
		constexpr static auto VixWaitType = VIX_PROPERTY_JOB_RESULT_HANDLE;
		using ReturnType = VixHandle;

		static Napi::Object Init(Napi::Env env, Napi::Object exports) {
			// clang-format off
			Napi::Function func = DefineClass(env, "VixHost", {
				// STATIC methods
				StaticMethod<&VixHost::ConnectLocal>("ConnectLocal", static_cast<napi_property_attributes>(napi_static | napi_configurable)),

				// INSTANCE methods
				//InstanceMethod<&VixHost::ConnectLocal>("ConnectLocal", static_cast<napi_property_attributes>(napi_static | napi_configurable)),
			});
			//clang-format on

			//Napi::FunctionReference* constructor = new Napi::FunctionReference();
			_constructor = Napi::Persistent(func);
			exports.Set("VixHost", func);
			//env.SetInstanceData<Napi::FunctionReference>(constructor);
			return exports;
		}

		static Napi::Value Create(VixHandle hostHandle) {
			printf("go fuck yourself\n");
			auto obj = _constructor.New({});
			auto* host = Unwrap(obj);
			printf("go fuck yourself with a hercules hook\n");
			host->hostHandle = hostHandle;
			return obj;
		}

		VixHost(const Napi::CallbackInfo& info)
			: Napi::ObjectWrap<VixHost>(info) {
			// TODO: Figure out a way to gate this off. Using this will leave a uninitalized host handle
			// at best and potentionally crash at worst.
			//Napi::TypeError::New(Env(), "Do not use `new VixHost()` to connect to VIX")
			//	.ThrowAsJavaScriptException();
		}
		
		~VixHost() {

			printf("Game Over!\n");
			//if(hostHandle)
			//	Vix_ReleaseHandle(hostHandle);
		}

		static Napi::Value ConnectLocal(const Napi::CallbackInfo& info) {
			// Create a async worker which will create the host object once done
			auto* worker = new VixAsyncJobWorker<VixHost>(info.Env(),
				VixHost_Connect(VIX_API_VERSION, 
					VIX_SERVICEPROVIDER_VMWARE_WORKSTATION, 
					nullptr, 
					0,
					nullptr, 
					nullptr,
					0, 
					VIX_INVALID_HANDLE, 
					nullptr, 
					nullptr
				)
			);

			// let it rip
			worker->Queue();
			return worker->Promise().Promise();
		}


		private: 
		static Napi::FunctionReference _constructor;
		VixHandle hostHandle{};
	};

	Napi::FunctionReference VixHost::_constructor;

} // namespace vix