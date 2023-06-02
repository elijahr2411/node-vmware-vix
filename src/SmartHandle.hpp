#pragma once
#include <vix.h>

namespace vix {

    // A smart/scoped VIX handle. Can't be copied, but can be moved, to extend lifetime.
    struct SmartHandle {
        SmartHandle() = default;
        constexpr explicit SmartHandle(VixHandle handle)
            : handle(handle) {

            }

        SmartHandle(const SmartHandle&) = delete;

        constexpr SmartHandle(SmartHandle&& move)
            : handle(move.handle) {

            }

        SmartHandle& operator=(const SmartHandle&) = delete;

        SmartHandle& operator=(SmartHandle&& move) {
            if(&move == this)
                return *this;

            handle = move.handle;
            move.handle = {};
        }

        constexpr ~SmartHandle() {
            if(handle)
                Vix_ReleaseHandle(handle);
        }

        VixHandle Get() {
            return handle;
        } 

        operator VixHandle() {
            return Get();
        }

    private:
        VixHandle handle{};
    };

}