#pragma once

namespace wiz::details {
    template <bool Last, typename... Preds>
    struct __lazy_and;

    template <typename... Preds>
    struct __lazy_and<false, Preds...> {
        static constexpr bool const value = false;
    };

    template <>
    struct __lazy_and<true> {
        static constexpr bool const value = true;
    };

    template <typename Pred>
    struct __lazy_and<true, Pred> {
        static constexpr bool const value = Pred::value;
    };

    template <typename H, typename... T>
    struct __lazy_and<true, H, T...> {
        static constexpr bool const value = __lazy_and<H::value, T...>::value;
    };

    template <typename P1, typename... Pr>
    struct _lazy_and {
        static constexpr bool const value = __lazy_and<P1::value, Pr...>::value;
    };
} // namespace wiz::details
