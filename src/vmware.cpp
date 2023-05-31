#include "vmware.hpp"

class VIXClient : public Napi::ObjectWrap<VIXClient> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        VIXClient(const Napi::CallbackInfo& info);
    private:
        VixHandle _hostHandle;
        VixHandle _vmHandle;
        Napi::Value Test(const Napi::CallbackInfo& info);
        Napi::Value PowerOn(const Napi::CallbackInfo& info);
};

Napi::Object VIXClient::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "VIXClient", {
        InstanceMethod<&VIXClient::Test>("Test", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&VIXClient::PowerOn>("PowerOn", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    });
    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("VIXClient", func);
    env.SetInstanceData<Napi::FunctionReference>(constructor);
    return exports;
}

VIXClient::VIXClient(const Napi::CallbackInfo& info) : Napi::ObjectWrap<VIXClient>(info) {
    // 0: VMX Path
    Napi::Env env = info.Env();
    if (info.Length() != 1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return;
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Argument 1 must be of type string").ThrowAsJavaScriptException();
        return;
    }
    // Connect to VMWare Workstation
    VixHandle jobHandle = VixHost_Connect(VIX_API_VERSION, VIX_SERVICEPROVIDER_VMWARE_WORKSTATION, NULL, 0, NULL, NULL, 0, VIX_INVALID_HANDLE, NULL, NULL);

    VixError err = VixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &this->_hostHandle, VIX_PROPERTY_NONE);
    if (err != VIX_OK) {
        Napi::Error::New(env, "VIX Failed to Connect").ThrowAsJavaScriptException();
        return;
    }
    Vix_ReleaseHandle(jobHandle);
    // Open the VMX
    jobHandle = VixHost_OpenVM(this->_hostHandle, std::string(info[0].As<Napi::String>()).c_str(), VIX_VMOPEN_NORMAL, VIX_INVALID_HANDLE, NULL, NULL);
    err = VixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &this->_vmHandle, VIX_PROPERTY_NONE);
    if (err != VIX_OK) {
        Napi::Error::New(env, Vix_GetErrorText(err, NULL)).ThrowAsJavaScriptException();
        return;
    }
    Vix_ReleaseHandle(jobHandle);
    // we shoooould be connected now I think
}

Napi::Value VIXClient::Test(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, "what hath god wrought");
}

Napi::Value VIXClient::PowerOn(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    VixHandle jobHandle = VixVM_PowerOn(this->_vmHandle, VIX_VMPOWEROP_NORMAL, VIX_INVALID_HANDLE, NULL, NULL);
    VixError err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
    if (err != VIX_OK) {
        Napi::Error::New(env, "VIX Failed to power up the VM").ThrowAsJavaScriptException();
    }
    return env.Null();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    VIXClient::Init(env, exports);
    return exports;
}

NODE_API_MODULE(vmware, Init)