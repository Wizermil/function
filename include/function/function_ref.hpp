#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include <function/details/config.hpp>
#include <function/details/lazy_and.hpp>
#include <function/details/lazy_not.hpp>
#include <function/details/not_null.hpp>
#include <function/details/policy_invoker_ref.hpp>
#include <function/details/strip_signature.hpp>

namespace wiz {

    template <typename F>
    class function_ref;

    template <typename R, typename... Args>
    class function_ref<R(Args...)> {
        template <typename F, bool = details::_lazy_and<details::_lazy_not<std::is_same<std::remove_cvref_t<F>, function_ref>>, std::is_invocable_r<void, F&, Args...>>::value>
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

        using invoker = details::policy_invoker_ref<R(Args...)>;
        invoker _invoker;
        void* _obj;

    public:
        constexpr function_ref() noexcept
        : _obj{nullptr} {}
        constexpr function_ref(function_ref const& other) noexcept
        : _invoker{other._invoker}
        , _obj{other._obj} {}
        constexpr function_ref& operator=(function_ref const& other) noexcept {
            _obj = other._obj;
            _invoker = other._invoker;
            return *this;
        }
        constexpr function_ref(function_ref&& other) noexcept
        : _invoker{other._invoker}
        , _obj{other._obj} {
            other._obj = nullptr;
            other._invoker = invoker();
        }
        constexpr function_ref& operator=(function_ref&& other) noexcept {
            _invoker = other._invoker;
            _obj = other._obj;
            other._invoker = invoker();
            other._obj = nullptr;
            return *this;
        }

        constexpr function_ref(std::nullptr_t) noexcept
        : _obj{nullptr} {}
        constexpr function_ref& operator=(std::nullptr_t) noexcept {
            _obj = nullptr;
            _invoker = invoker();
            return *this;
        }

        template <typename F, _enable_if_invocable_t<F>* = nullptr>
        constexpr function_ref(F&& f) noexcept
        : _obj{nullptr} {
            if (FUNCTION_LIKELY(details::not_null(f))) {
                _invoker = invoker::template create<F>();
                _obj = const_cast<void*>(static_cast<void const*>(std::addressof(f)));
            }
        }
        template <typename F, _enable_if_invocable_t<F>* = nullptr>
        constexpr function_ref& operator=(F&& f) noexcept {
            function_ref(std::forward<F>(f)).swap(*this);
            return *this;
        }

        constexpr void swap(function_ref& other) noexcept {
            std::swap(_invoker, other._invoker);
            std::swap(_obj, other._obj);
        }

        constexpr explicit operator bool() const noexcept { return _obj; }

        template <typename R2, class... Args2>
        bool operator==(function_ref<R2(Args2...)> const&) const = delete;
        template <typename R2, typename... Args2>
        bool operator!=(function_ref<R2(Args2...)> const&) const = delete;

        R operator()(Args... args) const { return _invoker.call(_obj, std::forward<Args>(args)...); }
    };

    template <typename R, typename... Args>
    function_ref(R (*)(Args...))->function_ref<R(Args...)>;

    template <typename F, typename Stripped = typename details::strip_signature<decltype(&F::operator())>::type>
    function_ref(F)->function_ref<Stripped>;

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator==(function_ref<R(Args...)> const& f, std::nullptr_t) noexcept {
        return !f;
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator==(std::nullptr_t, function_ref<R(Args...)> const& f) noexcept {
        return !f;
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator!=(function_ref<R(Args...)> const& f, std::nullptr_t) noexcept {
        return static_cast<bool>(f);
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline bool operator!=(std::nullptr_t, function_ref<R(Args...)> const& f) noexcept {
        return static_cast<bool>(f);
    }

    template <typename R, typename... Args>
    FUNCTION_HIDE_FROM_ABI inline void swap(function_ref<R(Args...)>& x, function_ref<R(Args...)>& y) noexcept {
        return x.swap(y);
    }

} // namespace wiz
