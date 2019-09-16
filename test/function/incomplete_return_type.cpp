#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <utility>

#include <function/function.hpp>

#include "helper/incomplete_return_type.hpp"

TEST_CASE("incomplete return type", "[function]") {
    {
        called = 0;
        incomplete_return_type X;
        X.fn = test_fn;
        incomplete_return_type const& CX = X;
        incomplete_return_type X2 = CX;
        REQUIRE(X2.fn);
        REQUIRE(called == 0);
        X2.fn();
        REQUIRE(called == 1);
    }

    {
        incomplete_return_type Empty;
        incomplete_return_type X2 = Empty;
        REQUIRE(!X2.fn);
    }

    static_assert(std::is_copy_constructible_v<incomplete_return_type>, "");
    static_assert(std::is_copy_assignable_v<incomplete_return_type>, "");
}
