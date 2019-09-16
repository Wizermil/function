#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <function/function.hpp>

#include "helper/free_function.hpp"

TEST_CASE("function equal", "[function]") {
    SECTION("inline bool operator==(function<R(Args...)> const& f, std::nullptr_t) noexcept", "[function]") {
        {
            wiz::function<void()> const a;
            REQUIRE(a == nullptr);
        }

        {
            wiz::function<int(int)> const b = &free_function1;
            REQUIRE_FALSE(b == nullptr);
        }
    }

    SECTION("inline bool operator==(std::nullptr_t, function<R(Args...)> const& f) noexcept", "[function]") {
        {
            wiz::function<void()> const a;
            REQUIRE(nullptr == a);
        }

        {
            wiz::function<int(int)> const b = &free_function1;
            REQUIRE_FALSE(nullptr == b);
        }
    }

    SECTION("inline bool operator!=(function<R(Args...)> const& f, std::nullptr_t) noexcept", "[function]") {
        {
            wiz::function<void()> const a;
            REQUIRE_FALSE(a != nullptr);
        }

        {
            wiz::function<int(int)> const b = &free_function1;
            REQUIRE(b != nullptr);
        }
    }

    SECTION("inline bool operator!=(std::nullptr_t, function<R(Args...)> const& f) noexcept", "[function]") {
        {
            wiz::function<void()> const a;
            REQUIRE_FALSE(nullptr != a);
        }

        {
            wiz::function<int(int)> const b = &free_function1;
            REQUIRE(nullptr != b);
        }
    }
}
