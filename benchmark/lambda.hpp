#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include "helper/get_random.hpp"

namespace wiz {

    template <typename F>
    void lambda_invoke_test(benchmark::State& state) noexcept {
        F f{[](int a) { return a + 1; }};
        int val = static_cast<int>(get_random(0, 100));
        for (auto _ : state) {
            val += f(val);
            benchmark::DoNotOptimize(val);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <>
    void lambda_invoke_test<void>(benchmark::State& state) noexcept {
        auto f{[](int a) { return a + 1; }};
        int val = static_cast<int>(get_random(0, 100));
        for (auto _ : state) {
            val += f(val);
            benchmark::DoNotOptimize(val);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
