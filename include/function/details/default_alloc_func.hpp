#pragma once

extern "C" {
#include <stdlib.h>
}

#include <new>
#include <utility>

#include <function/details/config.hpp>
#include <function/details/invoke_void_return_wrapper.hpp>

namespace wiz::details {

    template <typename F, typename FB>
    class default_alloc_func;

    template <typename F, typename R, typename... Args>
    class default_alloc_func<F, R(Args...)> {
        F _f;

    public:
        FUNCTION_HIDE_FROM_ABI explicit default_alloc_func(F const& other)
        : _f(other) {}
        FUNCTION_HIDE_FROM_ABI explicit default_alloc_func(F&& other) noexcept
        : _f(std::move(other)) {}

        FUNCTION_HIDE_FROM_ABI F const& target() const { return _f; }

        FUNCTION_HIDE_FROM_ABI R operator()(Args&&... arg) {
            using Invoker = invoke_void_return_wrapper<R>;
            return Invoker::call(_f, std::forward<Args>(arg)...);
        }

        FUNCTION_HIDE_FROM_ABI default_alloc_func* clone() const {
            default_alloc_func* hold = static_cast<default_alloc_func*>(malloc(sizeof(default_alloc_func)));
            return new (hold) default_alloc_func(_f);
        }

        FUNCTION_HIDE_FROM_ABI void destroy() noexcept { _f.~F(); }

        static void destroy_and_delete(default_alloc_func* f) {
            f->destroy();
            free(f);
        }
    };

} // namespace wiz::details
