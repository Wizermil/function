#pragma once

#include <cstdlib>
#include <utility>

#include <function/details/config.hpp>
#include <function/details/fast_forward.hpp>
#include <function/details/policy_storage.hpp>

namespace wiz::details {

    template <typename F>
    struct policy_invoker;

    template <typename R, typename... Args>
    struct policy_invoker<R(Args...)> {
        using Call = R (*)(policy_storage const*, fast_forward<Args>...);

        Call call;

        // Creates an invoker that abort.
        FUNCTION_HIDE_FROM_ABI policy_invoker()
        : call(&_call_empty) {}

        // Creates an invoker that calls the given instance of func.
        template <typename F>
        FUNCTION_HIDE_FROM_ABI static policy_invoker create() {
            return policy_invoker{&_call_impl<F>};
        }

    private:
        FUNCTION_HIDE_FROM_ABI explicit policy_invoker(Call c)
        : call(c) {}

        FUNCTION_HIDE_FROM_ABI static R _call_empty(policy_storage const*, fast_forward<Args>...) { std::abort(); }

        template <typename F>
        FUNCTION_HIDE_FROM_ABI static R _call_impl(policy_storage const* buf, fast_forward<Args>... args) {
            using Pointer = std::add_pointer_t<F>;
            if constexpr (use_small_storage_v<F>) {
                Pointer f = reinterpret_cast<Pointer>(&buf->small);
                return (*f)(std::forward<Args>(args)...);
            } else {
                Pointer f = static_cast<Pointer>(buf->large);
                return (*f)(std::forward<Args>(args)...);
            }
        }
    };

} // namespace wiz::details
