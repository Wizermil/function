#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <type_traits>
#include <vector>
#include <memory>

#include <function/unique_function.hpp>
#include <function/function.hpp>

#include "helper/free_function.hpp"
#include "helper/functor.hpp"
#include "helper/lvalue_invocable.hpp"
#include "helper/rvalue_invocable.hpp"

TEST_CASE("unique_function assign", "[unique_function]") {
    SECTION("unique_function& operator=(std::nullptr_t) noexcept") {
        std::vector<wiz::unique_function<int ()>> container;


        std::unique_ptr<int> test = std::make_unique<int>(10);


        container.emplace_back([t = std::move(test)]() {
            return *t + 1;
        });

        for (auto&& fn : container) {
            auto data = fn();
        }


    }
}
