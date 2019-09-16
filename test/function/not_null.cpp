#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <utility>

#include <function/function.hpp>

namespace not_null {
    int test();
    int test() { return 42; }
    int test(int);
    int test(int) { return 42; }
    int test(int, int);
    int test(int, int) { return 42; }
    int test(int, int, int);
    int test(int, int, int) { return 42; }

    int test(...);
    int test(...) { return 42; }
    int test(int, ...);
    int test(int, ...) { return 42; }
    int test(int, int, ...);
    int test(int, int, ...) { return 42; }
    int test(int, int, int, ...);
    int test(int, int, int, ...) { return 42; }

    struct mem_func03 {
        int test() { return 42; }
        int test() const { return 42; }
        int test() volatile { return 42; }
        int test() const volatile { return 42; }

        int test(int) { return 42; }
        int test(int) const { return 42; }
        int test(int) volatile { return 42; }
        int test(int) const volatile { return 42; }

        int test(int, int) { return 42; }
        int test(int, int) const { return 42; }
        int test(int, int) volatile { return 42; }
        int test(int, int) const volatile { return 42; }

        int test(int, int, int) { return 42; }
        int test(int, int, int) const { return 42; }
        int test(int, int, int) volatile { return 42; }
        int test(int, int, int) const volatile { return 42; }

        int test(...) { return 42; }
        int test(...) const { return 42; }
        int test(...) volatile { return 42; }
        int test(...) const volatile { return 42; }

        int test(int, ...) { return 42; }
        int test(int, ...) const { return 42; }
        int test(int, ...) volatile { return 42; }
        int test(int, ...) const volatile { return 42; }

        int test(int, int, ...) { return 42; }
        int test(int, int, ...) const { return 42; }
        int test(int, int, ...) volatile { return 42; }
        int test(int, int, ...) const volatile { return 42; }

        int test(int, int, int, ...) { return 42; }
        int test(int, int, int, ...) const { return 42; }
        int test(int, int, int, ...) volatile { return 42; }
        int test(int, int, int, ...) const volatile { return 42; }
    };

    struct mem_func11 {
        int test() & { return 42; }
        int test() const& { return 42; }
        int test() volatile& { return 42; }
        int test() const volatile& { return 42; }

        int test(...) & { return 42; }
        int test(...) const& { return 42; }
        int test(...) volatile& { return 42; }
        int test(...) const volatile& { return 42; }

        int test() && { return 42; }
        int test() const&& { return 42; }
        int test() volatile&& { return 42; }
        int test() const volatile&& { return 42; }

        int test(...) && { return 42; }
        int test(...) const&& { return 42; }
        int test(...) volatile&& { return 42; }
        int test(...) const volatile&& { return 42; }
    };

    struct mem_data {
        int test;
    };

    // Create a non-null free function by taking the address of &static_cast<Tp&>(test);
    template <typename T>
    struct creator {
        static T create() { return &test; }
    };

    // Create a non-null member pointer.
    template <typename Ret, typename Class>
    struct creator<Ret Class::*> {
        typedef Ret Class::*ReturnType;
        static ReturnType create() { return &Class::test; }
    };
} // namespace not_null

template <typename TestFn, typename Fn>
void test_imp() {
    { // Check that the null value is detected
        TestFn tf = nullptr;
        wiz::function<Fn> f = tf;
        REQUIRE(!f);
    }
    { // Check that the non-null value is detected.
        TestFn tf = not_null::creator<TestFn>::create();
        REQUIRE(tf != nullptr);
        wiz::function<Fn> f = tf;
        REQUIRE(!!f);
    }
}

TEST_CASE("function not_null function pointer", "[function]") {
    test_imp<int (*)(), int()>();
    test_imp<int (*)(...), int()>();
    test_imp<int (*)(int), int(int)>();
    test_imp<int (*)(int, ...), int(int)>();
    test_imp<int (*)(int, int), int(int, int)>();
    test_imp<int (*)(int, int, ...), int(int, int)>();
    test_imp<int (*)(int, int, int), int(int, int, int)>();
    test_imp<int (*)(int, int, int, ...), int(int, int, int)>();

    test_imp<int (not_null::mem_func03::*)(), int(not_null::mem_func03&)>();
    test_imp<int (not_null::mem_func03::*)(...), int(not_null::mem_func03&)>();
    test_imp<int (not_null::mem_func03::*)() const, int(not_null::mem_func03&)>();
    test_imp<int (not_null::mem_func03::*)(...) const, int(not_null::mem_func03&)>();
    test_imp<int (not_null::mem_func03::*)() volatile, int(not_null::mem_func03&)>();
    test_imp<int (not_null::mem_func03::*)(...) volatile, int(not_null::mem_func03&)>();
    test_imp<int (not_null::mem_func03::*)() const volatile, int(not_null::mem_func03&)>();
    test_imp<int (not_null::mem_func03::*)(...) const volatile, int(not_null::mem_func03&)>();

    test_imp<int (not_null::mem_func03::*)(int), int(not_null::mem_func03&, int)>();
    test_imp<int (not_null::mem_func03::*)(int, ...), int(not_null::mem_func03&, int)>();
    test_imp<int (not_null::mem_func03::*)(int) const, int(not_null::mem_func03&, int)>();
    test_imp<int (not_null::mem_func03::*)(int, ...) const, int(not_null::mem_func03&, int)>();
    test_imp<int (not_null::mem_func03::*)(int) volatile, int(not_null::mem_func03&, int)>();
    test_imp<int (not_null::mem_func03::*)(int, ...) volatile, int(not_null::mem_func03&, int)>();
    test_imp<int (not_null::mem_func03::*)(int) const volatile, int(not_null::mem_func03&, int)>();
    test_imp<int (not_null::mem_func03::*)(int, ...) const volatile, int(not_null::mem_func03&, int)>();

    test_imp<int (not_null::mem_func03::*)(int, int), int(not_null::mem_func03&, int, int)>();
    test_imp<int (not_null::mem_func03::*)(int, int, ...), int(not_null::mem_func03&, int, int)>();
    test_imp<int (not_null::mem_func03::*)(int, int) const, int(not_null::mem_func03&, int, int)>();
    test_imp<int (not_null::mem_func03::*)(int, int, ...) const, int(not_null::mem_func03&, int, int)>();
    test_imp<int (not_null::mem_func03::*)(int, int) volatile, int(not_null::mem_func03&, int, int)>();
    test_imp<int (not_null::mem_func03::*)(int, int, ...) volatile, int(not_null::mem_func03&, int, int)>();
    test_imp<int (not_null::mem_func03::*)(int, int) const volatile, int(not_null::mem_func03&, int, int)>();
    test_imp<int (not_null::mem_func03::*)(int, int, ...) const volatile, int(not_null::mem_func03&, int, int)>();

    test_imp<int (not_null::mem_func11::*)()&, int(not_null::mem_func11&)>();
    test_imp<int (not_null::mem_func11::*)(...)&, int(not_null::mem_func11&)>();
    test_imp<int (not_null::mem_func11::*)() const&, int(not_null::mem_func11&)>();
    test_imp<int (not_null::mem_func11::*)(...) const&, int(not_null::mem_func11&)>();
    test_imp<int (not_null::mem_func11::*)() volatile&, int(not_null::mem_func11&)>();
    test_imp<int (not_null::mem_func11::*)(...) volatile&, int(not_null::mem_func11&)>();
    test_imp<int (not_null::mem_func11::*)() const volatile&, int(not_null::mem_func11&)>();
    test_imp<int (not_null::mem_func11::*)(...) const volatile&, int(not_null::mem_func11&)>();

    test_imp<int (not_null::mem_func11::*)()&&, int(not_null::mem_func11 &&)>();
    test_imp<int (not_null::mem_func11::*)(...)&&, int(not_null::mem_func11 &&)>();
    test_imp<int (not_null::mem_func11::*)() const&&, int(not_null::mem_func11 &&)>();
    test_imp<int (not_null::mem_func11::*)(...) const&&, int(not_null::mem_func11 &&)>();
    test_imp<int (not_null::mem_func11::*)() volatile&&, int(not_null::mem_func11 &&)>();
    test_imp<int (not_null::mem_func11::*)(...) volatile&&, int(not_null::mem_func11 &&)>();
    test_imp<int (not_null::mem_func11::*)() const volatile&&, int(not_null::mem_func11 &&)>();
    test_imp<int (not_null::mem_func11::*)(...) const volatile&&, int(not_null::mem_func11 &&)>();

    test_imp<int not_null::mem_data::*, int(not_null::mem_data&)>();
}
