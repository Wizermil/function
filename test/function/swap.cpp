#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <utility>

#include <function/function.hpp>

#include "helper/free_function.hpp"
#include "helper/functor.hpp"

TEST_CASE("function swap", "[function]") {
    SECTION("void swap(function& f) noexcept") {
        functor::count = 0;
        {
            wiz::function<int(int)> a1 = functor(1);
            wiz::function<int(int)> a2 = functor(2);
            static_assert(noexcept(swap(a1, a2)), "");
            REQUIRE(functor::count == 2);
            REQUIRE(a1(0) == 46);
            REQUIRE(a2(0) == 47);
            swap(a1, a2);
            REQUIRE(a1(0) == 47);
            REQUIRE(a2(0) == 46);
        }
        REQUIRE(functor::count == 0);

        small_functor::count = 0;
        {
            wiz::function<int(int)> b1 = small_functor(1);
            wiz::function<int(int)> b2 = small_functor(2);
            static_assert(noexcept(swap(b1, b2)), "");
            REQUIRE(small_functor::count == 2);
            REQUIRE(b1(0) == 1);
            REQUIRE(b2(0) == 2);
            swap(b1, b2);
            REQUIRE(b1(0) == 2);
            REQUIRE(b2(0) == 1);
        }
        REQUIRE(small_functor::count == 0);

        functor::count = 0;
        {
            wiz::function<int(int)> c1 = functor(1);
            wiz::function<int(int)> c2 = free_function1;
            static_assert(noexcept(swap(c1, c2)), "");
            REQUIRE(functor::count == 1);
            REQUIRE(c1(0) == 46);
            REQUIRE(c2(0) == 1);
            swap(c1, c2);
            REQUIRE(functor::count == 1);
            REQUIRE(c1(0) == 1);
            REQUIRE(c2(0) == 46);
        }
        REQUIRE(functor::count == 0);

        functor::count = 0;
        {
            wiz::function<int(int)> d1 = free_function1;
            wiz::function<int(int)> d2 = functor(1);
            static_assert(noexcept(swap(d1, d2)), "");
            REQUIRE(functor::count == 1);
            REQUIRE(d1(0) == 1);
            REQUIRE(d2(0) == 46);
            swap(d1, d2);
            REQUIRE(functor::count == 1);
            REQUIRE(d1(0) == 46);
            REQUIRE(d2(0) == 1);
        }
        REQUIRE(functor::count == 0);

        {
            std::function<int(int)> e1 = free_function1;
            std::function<int(int)> e2 = free_function12;
            static_assert(noexcept(swap(e1, e2)), "");
            REQUIRE(e1(0) == 1);
            REQUIRE(e2(0) == 2);
            swap(e1, e2);
            REQUIRE(e1(0) == 2);
            REQUIRE(e2(0) == 1);
        }
    }
}
