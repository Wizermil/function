#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <utility>

#include <function/function.hpp>

#include "helper/free_function.hpp"
#include "helper/functor.hpp"

TEST_CASE("function move", "[function]") {
    SECTION("function(function&& f) noexcept") {
        {
            functor::count = 0;
            wiz::function<int(int)> a = functor();
            REQUIRE(functor::count == 1);
            wiz::function<int(int)> a2{std::move(a)};
            REQUIRE(functor::count == 1);
            REQUIRE(!a);
            REQUIRE(!!a2);
        }

        {
            functor::count = 0;
            using Ref = std::reference_wrapper<functor>;
            functor fun;
            Ref bref(fun);
            wiz::function<int(int)> b{bref};
            REQUIRE(functor::count == 1);
            REQUIRE(!!b);
            b(1);
            wiz::function<int(int)> b2{std::move(b)};
            REQUIRE(functor::count == 1);
            REQUIRE(!!b);
            REQUIRE(!!b2);
            b2(1);
            b(1); // b is unchanged because the target is small
        }

        {
            using Ptr = int (*)(int);
            Ptr fun = free_function1;
            wiz::function<int(int)> c{fun};
            REQUIRE(!!c);
            c(1);
            wiz::function<int(int)> c2{std::move(c)};
            REQUIRE(!!c);
            REQUIRE(!!c2);
            c2(1);
            c(1); // b is unchanged because the target is small
        }
    }
}
