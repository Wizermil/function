#pragma once

#include <function/details/config.hpp>

namespace wiz::details {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-template"

    template <typename F>
    FUNCTION_HIDE_FROM_ABI bool not_null(F const&) {
        return true;
    }

    template <typename F>
    FUNCTION_HIDE_FROM_ABI bool not_null(F* ptr) {
        return ptr;
    }

    template <typename Ret, typename Class>
    FUNCTION_HIDE_FROM_ABI bool not_null(Ret Class::*ptr) {
        return ptr;
    }

#pragma clang diagnostic pop

} // namespace wiz::details
