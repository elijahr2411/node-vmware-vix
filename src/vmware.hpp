#include <napi.h>
#include <vix.h>
//#include "vixundocumented.h"

// TODO: make the header file not completely useless

#if 0
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
#endif