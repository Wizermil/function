#pragma once

namespace wiz::details {

    template <typename F>
    struct strip_signature;

    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...)> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) volatile> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const volatile> {
        using type = R(Args...);
    };

    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...)&> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const&> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) volatile&> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const volatile&> {
        using type = R(Args...);
    };

    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) noexcept> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const noexcept> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) volatile noexcept> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const volatile noexcept> {
        using type = R(Args...);
    };

    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) & noexcept> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const& noexcept> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) volatile& noexcept> {
        using type = R(Args...);
    };
    template <typename R, typename G, typename... Args>
    struct strip_signature<R (G::*)(Args...) const volatile& noexcept> {
        using type = R(Args...);
    };

} // namespace wiz::details
