#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <vector>

#include <function/function_ref.hpp>

#include "helper/lvalue_invocable.hpp"
#include "helper/rvalue_invocable.hpp"

TEST_CASE("function_ref assign", "[function_ref]") {
    SECTION("function& operator=(std::nullptr_t) noexcept") {}

    SECTION("function& operator=(F& f)") {
        // simple

        int x = 1000;

        auto lambda = [&x](int v) { return x += v; };

        {
            wiz::function_ref<int(int)> a = lambda;
            REQUIRE(a(5) == 1005);
            REQUIRE(a(6) == 1011);
            REQUIRE(a(7) == 1018);
        }

        {
            wiz::function_ref<int(int)> const b = lambda;
            REQUIRE(b(5) == 1023);
            REQUIRE(b(6) == 1029);
            REQUIRE(b(7) == 1036);
        }

        auto const& clambda = lambda;

        {
            wiz::function_ref<int(int)> c = clambda;
            REQUIRE(c(5) == 1041);
            REQUIRE(c(6) == 1047);
            REQUIRE(c(7) == 1054);
        }

        {
            wiz::function_ref<int(int)> const d = clambda;
            REQUIRE(d(5) == 1059);
            REQUIRE(d(6) == 1065);
            REQUIRE(d(7) == 1072);
        }

        // function pointer

        int (*funcptr)(int) = [](int v) { return v * v; };

        {
            wiz::function_ref<int(int)> e = funcptr;
            REQUIRE(e(10) == 100);
            REQUIRE(e(11) == 121);
        }

        {
            wiz::function_ref<int(int)> const f = funcptr;
            REQUIRE(f(10) == 100);
            REQUIRE(f(11) == 121);
        }

        // functor

        struct OverloadedFunctor {
            // variant 1
            int operator()(int x) { return 100 + 1 * x; }

            // variant 2 (const-overload of v1)
            int operator()(int x) const { return 100 + 2 * x; }

            // variant 3
            int operator()(int x, int) { return 100 + 3 * x; }

            // variant 4 (const-overload of v3)
            int operator()(int x, int) const { return 100 + 4 * x; }

            // variant 5 (non-const, has no const-overload)
            int operator()(int x, char const*) { return 100 + 5 * x; }

            // variant 6 (const only)
            int operator()(int x, std::vector<int> const&) const { return 100 + 6 * x; }
        };
        OverloadedFunctor of;
        OverloadedFunctor const& cof = of;

        {
            wiz::function_ref<int(int)> g = of;
            REQUIRE(g(15) == 100 + 1 * 15);
            wiz::function_ref<int(int)> const cg = of;
            REQUIRE(cg(15) == 100 + 1 * 15);
        }

        {
            wiz::function_ref<int(int)> h = cof;
            REQUIRE(h(16) == 100 + 2 * 16);
            wiz::function_ref<int(int)> const ch = cof;
            REQUIRE(ch(16) == 100 + 2 * 16);
        }

        {
            wiz::function_ref<int(int, int)> i = of;
            REQUIRE(i(17, 0) == 100 + 3 * 17);
            wiz::function_ref<int(int, int)> const ci = of;
            REQUIRE(ci(17, 0) == 100 + 3 * 17);
        }

        {
            wiz::function_ref<int(int, int)> j = cof;
            REQUIRE(j(18, 0) == 100 + 4 * 18);
            wiz::function_ref<int(int, int)> const cj = cof;
            REQUIRE(cj(18, 0) == 100 + 4 * 18);
        }

        {
            wiz::function_ref<int(int, char const*)> k = of;
            REQUIRE(k(19, "foo") == 100 + 5 * 19);
            wiz::function_ref<int(int, char const*)> const ck = of;
            REQUIRE(ck(19, "foo") == 100 + 5 * 19);
        }

        {
            wiz::function_ref<int(int, std::vector<int> const&)> l = of;
            REQUIRE(l(20, {}) == 100 + 6 * 20);
            REQUIRE(l(20, {1, 2, 3}) == 100 + 6 * 20);
            wiz::function_ref<int(int, std::vector<int> const&)> const cl = of;
            REQUIRE(cl(20, {}) == 100 + 6 * 20);
            REQUIRE(cl(20, {1, 2, 3}) == 100 + 6 * 20);
        }

        {
            wiz::function_ref<int(int, std::vector<int> const&)> m = cof;
            REQUIRE(m(21, {}) == 100 + 6 * 21);
            wiz::function_ref<int(int, std::vector<int> const&)> const cm = cof;
            REQUIRE(cm(21, {}) == 100 + 6 * 21);
        }
    }

    using Fn = wiz::function_ref<void(int, int, int)>;
    static_assert(std::is_assignable_v<Fn&, lvalue_invocable&>, "");
    static_assert(std::is_assignable_v<Fn&, lvalue_invocable>, "");
    static_assert(!std::is_assignable_v<Fn&, rvalue_invocable&>, "");
    static_assert(!std::is_assignable_v<Fn&, rvalue_invocable>, "");

    using Fn2 = wiz::function_ref<int(int)>;
    static_assert(std::is_copy_constructible_v<Fn2>, "");
    static_assert(std::is_move_constructible_v<Fn2>, "");
    static_assert(std::is_constructible_v<Fn2, Fn2&>, "");
    static_assert(std::is_copy_assignable_v<Fn2>, "");
    static_assert(std::is_move_assignable_v<Fn2>, "");
    static_assert(std::is_assignable_v<Fn2, Fn2&>, "");
    static_assert(std::is_nothrow_copy_constructible_v<Fn2>, "");
    static_assert(std::is_nothrow_move_constructible_v<Fn2>, "");
    static_assert(std::is_nothrow_constructible_v<Fn2, Fn2&>, "");
    static_assert(std::is_nothrow_copy_assignable_v<Fn2>, "");
    static_assert(std::is_nothrow_move_assignable_v<Fn2>, "");
    static_assert(std::is_nothrow_assignable_v<Fn2, Fn2&>, "");
}
