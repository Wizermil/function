#pragma once

#include <type_traits>

#include <function/details/config.hpp>

namespace wiz::details {

    union policy_storage {
        mutable char small[sizeof(void*) * 2];
        void* large;
    };

    template <typename F>
    FUNCTION_HIDE_FROM_ABI static constexpr bool const use_small_storage_v = sizeof(F) <= sizeof(policy_storage) &&
        alignof(F) <= alignof(policy_storage) && std::is_trivially_copy_constructible_v<F>&& std::is_trivially_destructible_v<F>;

} // namespace wiz::details
