#include "vmware.hpp"

class VIXClient : public Napi::ObjectWrap<VIXClient> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        VIXClient(const Napi::CallbackInfo& info);
    private:
        VixHandle _hostHandle;
        VixHandle _vmHandle;
        Napi::Value Test(const Napi::CallbackInfo& info);
};

Napi::Object VIXClient::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "VIXClient", {
        InstanceMethod<&VIXClient::Test>("Test", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    });
    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("VIXClient", func);
    env.SetInstanceData<Napi::FunctionReference>(constructor);
    return exports;
}

VIXClient::VIXClient(const Napi::CallbackInfo& info) : Napi::ObjectWrap<VIXClient>(info) {
    Napi::Env env = info.Env();
    VixHandle jobHandle = VixHost_Connect(VIX_API_VERSION, VIX_SERVICEPROVIDER_VMWARE_WORKSTATION, NULL, NULL, NULL, NULL, 0, VIX_INVALID_HANDLE, NULL, NULL);
    VixError err = VixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &this->_hostHandle, VIX_PROPERTY_NONE);
    if (err != VIX_OK) {
        Napi::TypeError::New(env, "VIX Failed to Connect").ThrowAsJavaScriptException();
        return;
    }
    Vix_ReleaseHandle(jobHandle);
}

Napi::Value VIXClient::Test(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, "what hath god wrought");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    VIXClient::Init(env, exports);
    return exports;
}

NODE_API_MODULE(vmware, Init)