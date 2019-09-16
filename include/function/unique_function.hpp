#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include <function/details/config.hpp>
#include <function/details/lazy_and.hpp>
#include <function/details/lazy_not.hpp>
#include <function/details/policy_unique_func.hpp>
#include <function/details/strip_signature.hpp>

namespace wiz {

    template <typename F>
    class unique_function;

    template <typename R, typename... Args>
    class unique_function<R(Args...)> {
        template <typename F,
                  bool = details::_lazy_and<details::_lazy_not<std::is_same<std::remove_cvref_t<F>, unique_function>>, std::is_invocable_r<void, F&, Args...>>::value>
        struct _invocable;

        template <typename F>
        struct _invocable<F, true> {
            static constexpr bool const value = std::is_same_v<void, R> || std::is_convertible_v<std::invoke_result_t<F&, Args...>, R>;
        };

        template <typename F>
        struct _invocable<F, false> {
            static constexpr bool const value = false;
        };

        template <typename F>
        using _enable_if_invocable_t = std::enable_if_t<_invocable<F>::value>;

        details::policy_unique_func<R(Args...)> _f;

    public:
        using result_type = R;

        unique_function() noexcept {}
        unique_function(unique_function const& f) = delete;
        unique_function& operator=(unique_function const& f) = delete;
        unique_function(unique_function&& f) noexcept
        : _f(std::move(f._f)) {}
        unique_function& operator=(unique_function&& f) noexcept {
            _f = std::move(f._f);
            return *this;
        }
        ~unique_function() {}

        unique_function(std::nullptr_t) noexcept {}
        unique_function& operator=(std::nullptr_t) noexcept {
            _f = nullptr;
            return *this;
        }

        template <typename F, _enable_if_invocable_t<F>* = nullptr>
        unique_function(F f)
        : _f(std::move(f)) {}
        template <typename F, _enable_if_invocable_t<F>* = nullptr>
        unique_function& operator=(F& f) {
            unique_function(std::forward<F>(f)).swap(*this);
            return *this;
        }

        void swap(unique_function& f) noexcept { _f.swap(f._f); }

        explicit operator bool() const noexcept { return static_cast<bool>(_f); }

        template <typename R2, class... Args2>
        bool operator==(unique_function<R2(Args2...)> const&) const = delete;
        template <typename R2, typename... Args2>
        bool operator!=(unique_function<R2(Args2...)> const&) const = delete;

        R operator()(Args... arg) const { return _f(std::forward<Args>(arg)...); }
    };

    template <typename R, typename... Args>
    unique_function(R (*)(Args...))->unique_function<R(Args...)>;

    template <typename F, typename Stripped = typename details::strip_signature<decltype(&F::operator())>::type>
    unique_function(F)->unique_function<Stripped>;

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator==(unique_function<R(Args...)> const& f, std::nullptr_t) noexcept {
        return !f;
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator==(std::nullptr_t, unique_function<R(Args...)> const& f) noexcept {
        return !f;
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator!=(unique_function<R(Args...)> const& f, std::nullptr_t) noexcept {
        return static_cast<bool>(f);
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator!=(std::nullptr_t, unique_function<R(Args...)> const& f) noexcept {
        return static_cast<bool>(f);
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline void swap(unique_function<R(Args...)>& x, unique_function<R(Args...)>& y) noexcept {
        return x.swap(y);
    }

} // namespace wiz
