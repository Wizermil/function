#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include "helper/get_random.hpp"

namespace wiz {

    struct functor {
        int operator()(int val);
    };

    struct light_functor {
        int operator()(int val);

        int a;
    };

    struct heavy_functor {
        int operator()(int val);

        int a[10];
    };

    struct non_assignable_functor {
        int operator()(int val);

        int& a;
    };

    template <typename F, typename T>
    void functor_invoke_test(benchmark::State& state) noexcept {
        F f{T{}};
        int val = static_cast<int>(get_random(0, 100));
        for (auto _ : state) {
            val += f(val);
            benchmark::DoNotOptimize(val);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
