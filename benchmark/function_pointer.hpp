#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include "helper/get_random.hpp"

namespace wiz {

    static inline int plain(int val) { return val + 1; }

    template <typename F>
    void function_pointer_invoke_test(benchmark::State& state) noexcept {
        auto fnptr = &plain;
        F f{fnptr};
        int val = get_random<int>(0, 100);
        for (auto _ : state) {
            val += f(val);
            benchmark::DoNotOptimize(val);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
