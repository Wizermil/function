#pragma once

extern "C" {
#include <stdlib.h>
}

#include <cstddef>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

#include <function/details/config.hpp>
#include <function/details/default_alloc_unique_func.hpp>
#include <function/details/not_null.hpp>
#include <function/details/policy_invoker.hpp>
#include <function/details/policy_storage.hpp>
#include <function/details/unique_policy.hpp>

namespace wiz::details {

    template <typename F>
    class policy_unique_func;

    template <typename R, typename... Args>
    class policy_unique_func<R(Args...)> {
        // Inline storage for small objects.
        policy_storage _buf;

        // Calls the value stored in _buf. This could technically be part of policy, but storing it here eliminates a level of indirection inside operator().
        using invoker = policy_invoker<R(Args...)>;
        invoker _invoker;

        // The policy that describes how to move / copy / destroy _buf. Never null, even if the function is empty.
        unique_policy const* _policy;

    public:
        FUNCTION_HIDE_FROM_ABI policy_unique_func()
        : _policy(unique_policy::create_empty()) {}

        template <typename F, std::enable_if_t<!std::is_same_v<std::decay_t<F>, policy_unique_func>>* = nullptr>
        FUNCTION_HIDE_FROM_ABI explicit policy_unique_func(F&& f)
        : _policy(unique_policy::create_empty()) {
            using Fun = details::default_alloc_unique_func<F, R(Args...)>;

            if (FUNCTION_LIKELY(not_null(f))) {
                _invoker = invoker::template create<Fun>();
                _policy = unique_policy::create<Fun>();
                if constexpr (use_small_storage_v<Fun>) {
                    new (reinterpret_cast<void*>(&_buf.small)) Fun(std::move(f));
                } else {
                    Fun* hold = static_cast<Fun*>(malloc(sizeof(Fun)));
                    _buf.large = new (hold) Fun(std::move(f));
                }
            }
        }

        FUNCTION_HIDE_FROM_ABI policy_unique_func(policy_unique_func const& f) = delete;

        FUNCTION_HIDE_FROM_ABI policy_unique_func(policy_unique_func&& f)
        : _buf(f._buf)
        , _invoker(f._invoker)
        , _policy(f._policy) {
            if (_policy->destroy) {
                f._policy = unique_policy::create_empty();
                f._invoker = invoker();
            }
        }

        FUNCTION_HIDE_FROM_ABI ~policy_unique_func() {
            if (_policy->destroy) {
                _policy->destroy(_buf.large);
            }
        }

        FUNCTION_HIDE_FROM_ABI policy_unique_func& operator=(policy_unique_func&& f) {
            *this = nullptr;
            _buf = f._buf;
            _invoker = f._invoker;
            _policy = f._policy;
            f._policy = unique_policy::create_empty();
            f._invoker = invoker();
            return *this;
        }

        FUNCTION_HIDE_FROM_ABI policy_unique_func& operator=(std::nullptr_t) {
            unique_policy const* p = _policy;
            _policy = unique_policy::create_empty();
            _invoker = invoker();
            if (p->destroy) {
                p->destroy(_buf.large);
            }
            return *this;
        }

        FUNCTION_HIDE_FROM_ABI R operator()(Args&&... arg) const { return _invoker.call(std::addressof(_buf), std::forward<Args>(arg)...); }

        FUNCTION_HIDE_FROM_ABI void swap(policy_unique_func& f) {
            std::swap(_invoker, f._invoker);
            std::swap(_policy, f._policy);
            std::swap(_buf, f._buf);
        }

        FUNCTION_HIDE_FROM_ABI explicit operator bool() const noexcept { return !_policy->is_null; }
    };
} // namespace wiz::details
