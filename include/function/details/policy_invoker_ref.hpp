#pragma once

#include <cstdlib>
#include <type_traits>
#include <utility>

#include <function/details/config.hpp>
#include <function/details/fast_forward.hpp>
#include <function/details/invoke_void_return_wrapper.hpp>
#include <function/details/policy_storage.hpp>

namespace wiz::details {

    template <typename F>
    struct policy_invoker_ref;

    template <typename R, typename... Args>
    struct policy_invoker_ref<R(Args...)> {
        using Call = R (*)(void*, fast_forward<Args>...);

        Call call;

        // Creates an invoker that abort.
        FUNCTION_HIDE_FROM_ABI policy_invoker_ref()
        : call(&_call_empty) {}

        // Creates an invoker that calls the given instance of func.
        template <typename F>
        FUNCTION_HIDE_FROM_ABI static policy_invoker_ref create() {
            return policy_invoker_ref{&_call_impl<F>};
        }

    private:
        FUNCTION_HIDE_FROM_ABI explicit policy_invoker_ref(Call c)
        : call(c) {}

        FUNCTION_HIDE_FROM_ABI static R _call_empty(void*, fast_forward<Args>...) { std::abort(); }

        template <typename F>
        FUNCTION_HIDE_FROM_ABI static R _call_impl(void* buf, fast_forward<Args>... args) {
            using Invoker = invoke_void_return_wrapper<R>;
            using Pointer = std::add_pointer_t<F>;
            Pointer f = static_cast<Pointer>(buf);
            return Invoker::call(*f, std::forward<Args>(args)...);
        }
    };

} // namespace wiz::details
