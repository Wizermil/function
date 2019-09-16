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

TEST_CASE("function assign", "[function]") {
    SECTION("function& operator=(std::nullptr_t) noexcept") {
        functor::count = 0;
        {
            wiz::function<int(int)> a = functor();
            REQUIRE(functor::count == 1);
            REQUIRE(!!a);
            a = nullptr;
            REQUIRE(functor::count == 0);
            REQUIRE(!a);
        }
        REQUIRE(functor::count == 0);

        {
            wiz::function<int(int)> b = free_function1;
            REQUIRE(!!b);
            b = nullptr;
        }
    }

    SECTION("function& operator=(F& f)") {
        functor::count = 0;
        {
            wiz::function<int(int)> a;
            a = functor();
            REQUIRE(!!a);
            REQUIRE(functor::count == 1);
        }
        REQUIRE(functor::count == 0);

        {
            wiz::function<int(int)> b;
            b = free_function1;
            REQUIRE(!!b);
            REQUIRE(b(1) == 1);
        }

        {
            wiz::function<int(int)> c;
            c = reinterpret_cast<int (*)(int)>(0);
            REQUIRE(!c);
        }

        {
            wiz::function<int(functor const*, int)> d;
            d = &functor::func;
            REQUIRE(!!d);
            functor dd;
            REQUIRE(d(&dd, 1) == 1);
        }

        {
            wiz::function<void(int)> e;
            e = &free_function1;
            REQUIRE(!!e);
            e(1);
        }
    }

    using Fn = wiz::function<void(int, int, int)>;
    static_assert(std::is_assignable_v<Fn&, lvalue_invocable&>, "");
    static_assert(std::is_assignable_v<Fn&, lvalue_invocable>, "");
    static_assert(!std::is_assignable_v<Fn&, rvalue_invocable&>, "");
    static_assert(!std::is_assignable_v<Fn&, rvalue_invocable>, "");
}
