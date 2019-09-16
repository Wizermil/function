#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>

#include <function/function.hpp>

#include "helper/free_function.hpp"
#include "helper/functor.hpp"
#include "helper/lvalue_invocable.hpp"
#include "helper/rvalue_invocable.hpp"

TEST_CASE("function constructor", "[function]") {
    SECTION("function() noexcept") {
        wiz::function<void()> a;
        REQUIRE(!a);

        wiz::function<int()> b;
        REQUIRE(!b);

        wiz::function<int(int)> c;
        REQUIRE(!c);

        wiz::function<void(int)> d;
        REQUIRE(!d);
    }

    SECTION("function(std::nullptr_t) noexcept") {
        wiz::function<int(int)> a{nullptr};
        REQUIRE(!a);
    }

    SECTION("function(F f)") {
        functor::count = 0;
        {
            wiz::function<int(int)> a{functor()};
            REQUIRE(!!a);
            REQUIRE(functor::count == 1);
        }
        REQUIRE(functor::count == 0);

        {
            wiz::function<int(int)> b{free_function1};
            REQUIRE(!!b);
            REQUIRE(b(1) == 1);
        }

        {
            wiz::function<int(int)> c{reinterpret_cast<int (*)(int)>(0)};
            REQUIRE(!c);
        }

        {
            wiz::function<int(functor const*, int)> d{&functor::func};
            REQUIRE(!!d);
            functor dd;
            REQUIRE(d(&dd, 1) == 1);
        }

        {
            wiz::function<void()> e{static_cast<void (*)()>(nullptr)};
            REQUIRE(!e);
        }

        {
            wiz::function<void(int)> f{&free_function1};
            REQUIRE(!!f);
            f(1);
        }
    }

    using Fn = wiz::function<void(int, int, int)>;
    static_assert(std::is_constructible_v<Fn, lvalue_invocable&>, "");
    static_assert(std::is_constructible_v<Fn, lvalue_invocable>, "");
    static_assert(!std::is_constructible_v<Fn, rvalue_invocable&>, "");
    static_assert(!std::is_constructible_v<Fn, rvalue_invocable>, "");
}
