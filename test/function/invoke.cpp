#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <utility>

#include <function/function.hpp>
#include <function/function_ref.hpp>

namespace invoke {

    static int count = 0;

    // 0 args, return int

    int f_int_0();
    int f_int_0() { return 3; }

    struct A_int_0 {
        int operator()() { return 4; }
    };

    void test_int_0();
    void test_int_0() {
        // function
        {
            wiz::function<int()> r1(f_int_0);
            REQUIRE(r1() == 3);
        }
        // function pointer
        {
            int (*fp)() = f_int_0;
            wiz::function<int()> r1(fp);
            REQUIRE(r1() == 3);
        }
        // functor
        {
            A_int_0 a0;
            wiz::function<int()> r1(a0);
            REQUIRE(r1() == 4);
        }
    }

    // 0 args, return void

    void f_void_0();
    void f_void_0() { ++invoke::count; }

    struct A_void_0 {
        void operator()() { ++invoke::count; }
    };

    // 1 arg, return void

    void f_void_1(int i);
    void f_void_1(int i) { count += i; }

    struct A_void_1 {
        void operator()(int i) { count += i; }

        void mem1() { ++count; }
        void mem2() const { ++count; }
    };

    // 1 arg, return int

    int f_int_1(int i);
    int f_int_1(int i) { return i + 1; }

    struct A_int_1 {
        A_int_1()
        : data_(5) {}
        int operator()(int i) { return i - 1; }

        int mem1() { return 3; }
        int mem2() const { return 4; }
        int data_;
    };

    // 2 arg, return void

    void f_void_2(int i, int j);
    void f_void_2(int i, int j) { count += i + j; }

    struct A_void_2 {
        void operator()(int i, int j) { count += i + j; }

        void mem1(int i) { count += i; }
        void mem2(int i) const { count += i; }
    };

    // 2 arg, return int

    int f_int_2(int i, int j);
    int f_int_2(int i, int j) { return i + j; }

    struct A_int_2 {
        int operator()(int i, int j) { return i + j; }

        int mem1(int i) { return i + 1; }
        int mem2(int i) const { return i + 2; }
    };
} // namespace invoke

TEST_CASE("function invoke", "[function]") {
    SECTION("void_0") {
        int save_count = invoke::count;
        // function
        {
            wiz::function<void()> r1(invoke::f_void_0);
            r1();
            REQUIRE(invoke::count == save_count + 1);
            save_count = invoke::count;
        }
        // function pointer
        {
            void (*fp)() = invoke::f_void_0;
            wiz::function<void()> r1(fp);
            r1();
            REQUIRE(invoke::count == save_count + 1);
            save_count = invoke::count;
        }
        // functor
        {
            invoke::A_void_0 a0;
            wiz::function<void()> r1(a0);
            r1();
            REQUIRE(invoke::count == save_count + 1);
            save_count = invoke::count;
        }
    }

    SECTION("void_1") {
        int save_count = invoke::count;
        // function
        {
            wiz::function<void(int)> r1(invoke::f_void_1);
            int i = 2;
            r1(i);
            REQUIRE(invoke::count == save_count + 2);
            save_count = invoke::count;
        }
        // function pointer
        {
            void (*fp)(int) = invoke::f_void_1;
            wiz::function<void(int)> r1(fp);
            int i = 3;
            r1(i);
            REQUIRE(invoke::count == save_count + 3);
            save_count = invoke::count;
        }
        // functor
        {
            invoke::A_void_1 a0;
            wiz::function<void(int)> r1(a0);
            int i = 4;
            r1(i);
            REQUIRE(invoke::count == save_count + 4);
            save_count = invoke::count;
        }
        // member function pointer
        {
            void (invoke::A_void_1::*fp)() = &invoke::A_void_1::mem1;
            wiz::function<void(invoke::A_void_1)> r1(fp);
            invoke::A_void_1 a;
            r1(a);
            REQUIRE(invoke::count == save_count + 1);
            save_count = invoke::count;
            invoke::A_void_1* ap = &a;
            wiz::function<void(invoke::A_void_1*)> r2 = fp;
            r2(ap);
            REQUIRE(invoke::count == save_count + 1);
            save_count = invoke::count;
        }
        // const member function pointer
        {
            void (invoke::A_void_1::*fp)() const = &invoke::A_void_1::mem2;
            wiz::function<void(invoke::A_void_1)> r1(fp);
            invoke::A_void_1 a;
            r1(a);
            REQUIRE(invoke::count == save_count + 1);
            save_count = invoke::count;
            wiz::function<void(invoke::A_void_1*)> r2(fp);
            invoke::A_void_1* ap = &a;
            r2(ap);
            REQUIRE(invoke::count == save_count + 1);
            save_count = invoke::count;
        }
    }

    SECTION("int_1") {
        // function
        {
            wiz::function<int(int)> r1(invoke::f_int_1);
            int i = 2;
            REQUIRE(r1(i) == 3);
        }
        // function pointer
        {
            int (*fp)(int) = invoke::f_int_1;
            wiz::function<int(int)> r1(fp);
            int i = 3;
            REQUIRE(r1(i) == 4);
        }
        // functor
        {
            invoke::A_int_1 a0;
            wiz::function<int(int)> r1(a0);
            int i = 4;
            REQUIRE(r1(i) == 3);
        }
        // member function pointer
        {
            int (invoke::A_int_1::*fp)() = &invoke::A_int_1::mem1;
            wiz::function<int(invoke::A_int_1)> r1(fp);
            invoke::A_int_1 a;
            REQUIRE(r1(a) == 3);
            wiz::function<int(invoke::A_int_1*)> r2(fp);
            invoke::A_int_1* ap = &a;
            REQUIRE(r2(ap) == 3);
        }
        // const member function pointer
        {
            int (invoke::A_int_1::*fp)() const = &invoke::A_int_1::mem2;
            wiz::function<int(invoke::A_int_1)> r1(fp);
            invoke::A_int_1 a;
            REQUIRE(r1(a) == 4);
            wiz::function<int(invoke::A_int_1*)> r2(fp);
            invoke::A_int_1* ap = &a;
            REQUIRE(r2(ap) == 4);
        }
        // member data pointer
        {
            int invoke::A_int_1::*fp = &invoke::A_int_1::data_;
            wiz::function<int&(invoke::A_int_1&)> r1(fp);
            invoke::A_int_1 a;
            REQUIRE(r1(a) == 5);
            r1(a) = 6;
            REQUIRE(r1(a) == 6);
            wiz::function<int&(invoke::A_int_1*)> r2(fp);
            invoke::A_int_1* ap = &a;
            REQUIRE(r2(ap) == 6);
            r2(ap) = 7;
            REQUIRE(r2(ap) == 7);
        }
    }

    SECTION("void_2") {
        int save_count = invoke::count;
        // function
        {
            wiz::function<void(int, int)> r1(invoke::f_void_2);
            int i = 2;
            int j = 3;
            r1(i, j);
            REQUIRE(invoke::count == save_count + 5);
            save_count = invoke::count;
        }
        // function pointer
        {
            void (*fp)(int, int) = invoke::f_void_2;
            wiz::function<void(int, int)> r1(fp);
            int i = 3;
            int j = 4;
            r1(i, j);
            REQUIRE(invoke::count == save_count + 7);
            save_count = invoke::count;
        }
        // functor
        {
            invoke::A_void_2 a0;
            wiz::function<void(int, int)> r1(a0);
            int i = 4;
            int j = 5;
            r1(i, j);
            REQUIRE(invoke::count == save_count + 9);
            save_count = invoke::count;
        }
        // member function pointer
        {
            void (invoke::A_void_2::*fp)(int) = &invoke::A_void_2::mem1;
            wiz::function<void(invoke::A_void_2, int)> r1(fp);
            invoke::A_void_2 a;
            int i = 3;
            r1(a, i);
            REQUIRE(invoke::count == save_count + 3);
            save_count = invoke::count;
            wiz::function<void(invoke::A_void_2*, int)> r2(fp);
            invoke::A_void_2* ap = &a;
            r2(ap, i);
            REQUIRE(invoke::count == save_count + 3);
            save_count = invoke::count;
        }
        // const member function pointer
        {
            void (invoke::A_void_2::*fp)(int) const = &invoke::A_void_2::mem2;
            wiz::function<void(invoke::A_void_2, int)> r1(fp);
            invoke::A_void_2 a;
            int i = 4;
            r1(a, i);
            REQUIRE(invoke::count == save_count + 4);
            save_count = invoke::count;
            wiz::function<void(invoke::A_void_2*, int)> r2(fp);
            invoke::A_void_2* ap = &a;
            r2(ap, i);
            REQUIRE(invoke::count == save_count + 4);
            save_count = invoke::count;
        }
    }

    SECTION("int_2") {
        // function
        {
            wiz::function<int(int, int)> r1(invoke::f_int_2);
            int i = 2;
            int j = 3;
            REQUIRE(r1(i, j) == i + j);
        }
        // function pointer
        {
            int (*fp)(int, int) = invoke::f_int_2;
            wiz::function<int(int, int)> r1(fp);
            int i = 3;
            int j = 4;
            REQUIRE(r1(i, j) == i + j);
        }
        // functor
        {
            invoke::A_int_2 a0;
            wiz::function<int(int, int)> r1(a0);
            int i = 4;
            int j = 5;
            REQUIRE(r1(i, j) == i + j);
        }
        // member function pointer
        {
            int (invoke::A_int_2::*fp)(int) = &invoke::A_int_2::mem1;
            wiz::function<int(invoke::A_int_2, int)> r1(fp);
            invoke::A_int_2 a;
            int i = 3;
            REQUIRE(r1(a, i) == i + 1);
            wiz::function<int(invoke::A_int_2*, int)> r2(fp);
            invoke::A_int_2* ap = &a;
            REQUIRE(r2(ap, i) == i + 1);
        }
        // const member function pointer
        {
            int (invoke::A_int_2::*fp)(int) const = &invoke::A_int_2::mem2;
            wiz::function<int(invoke::A_int_2, int)> r1(fp);
            invoke::A_int_2 a;
            int i = 4;
            REQUIRE(r1(a, i) == i + 2);
            wiz::function<int(invoke::A_int_2*, int)> r2(fp);
            invoke::A_int_2* ap = &a;
            REQUIRE(r2(ap, i) == i + 2);
        }
    }
}
