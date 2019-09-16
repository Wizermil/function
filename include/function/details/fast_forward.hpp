#pragma once

#include <type_traits>

namespace wiz::details {

    template <typename T>
    using fast_forward = std::conditional_t<std::is_scalar_v<T>, T, T&&>;

}
