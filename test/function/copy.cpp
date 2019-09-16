#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <utility>

#include <function/function.hpp>

#include "helper/free_function.hpp"
#include "helper/functor.hpp"

TEST_CASE("function copy", "[function]") {
    SECTION("function(function const& f)") {
        functor::count = 0;
        {
            wiz::function<int(int)> a = functor();
            REQUIRE(functor::count == 1);
            wiz::function<int(int)> a2{a};
            REQUIRE(functor::count == 2);
            REQUIRE(!!a2);
        }
        REQUIRE(functor::count == 0);

        {
            wiz::function<int(int)> b = free_function1;
            wiz::function<int(int)> b2{b};
            REQUIRE(!!b2);
        }

        {
            wiz::function<int(int)> c;
            wiz::function<int(int)> c2{c};
            REQUIRE(!c2);
        }
    }

    SECTION("function& operator=(function const& f)") {
        functor::count = 0;
        {
            wiz::function<int(int)> a = functor();
            REQUIRE(functor::count == 1);
            wiz::function<int(int)> a2;
            a2 = a;
            REQUIRE(functor::count == 2);
            REQUIRE(!!a2);
        }
        REQUIRE(functor::count == 0);

        {
            wiz::function<int(int)> b = free_function1;
            wiz::function<int(int)> b2;
            b2 = b;
            REQUIRE(!!b2);
        }

        {
            wiz::function<int(int)> c;
            wiz::function<int(int)> c2;
            c2 = c;
            REQUIRE(!c2);
        }

        {
            using Func = wiz::function<int()>;
            Func d = free_function0;
            Func& dr = (d = static_cast<Func&>(d));
            REQUIRE(&dr == &d);
        }

        {
            using Func = wiz::function<int(int)>;
            Func e = free_function1;
            Func& er = (e = static_cast<Func&>(e));
            REQUIRE(&er == &e);
        }

        {
            using Func = wiz::function<int(int, int)>;
            Func f = free_function2;
            Func& fr = (f = static_cast<Func&>(f));
            REQUIRE(&fr == &f);
        }

        {
            using Func = wiz::function<int(int, int, int)>;
            Func g = free_function3;
            Func& gr = (g = static_cast<Func&>(g));
            REQUIRE(&gr == &g);
        }
    }
}
