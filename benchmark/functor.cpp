#include "functor.hpp"

namespace wiz {

    int functor::operator()(int val) { return val + 1; }

    int light_functor::operator()(int val) { return val + 1; }

    int heavy_functor::operator()(int val) { return val + 1; }

    int non_assignable_functor::operator()(int val) { return val + 1; }

} // namespace wiz
