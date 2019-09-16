#pragma once

#include <functional>
#include <utility>

#include <function/details/config.hpp>

namespace wiz::details {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-template"

    template <typename Ret>
    struct invoke_void_return_wrapper {
        template <typename... Args>
        FUNCTION_HIDE_FROM_ABI static Ret call(Args&&... args) {
            return std::invoke(std::forward<Args>(args)...);
        }
    };

    template <>
    struct invoke_void_return_wrapper<void> {
        template <typename... Args>
        FUNCTION_HIDE_FROM_ABI static void call(Args&&... args) {
            std::invoke(std::forward<Args>(args)...);
        }
    };

#pragma clang diagnostic pop

} // namespace wiz::details
