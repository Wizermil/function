#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

namespace wiz {

    template <typename T>
    void constructor_default_test(benchmark::State& state) noexcept {
        for (auto _ : state) {
            T const data;
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T, typename U>
    void constructor_invocable_test(benchmark::State& state) noexcept {
        for (auto _ : state) {
            T const data{U{}};
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
