#include <functional>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#include <function/function.hpp>
#define DELEGATE_ARGS_SIZE 40
#define DELEGATE_ARGS_ALIGN 8
#include <Delegate/delegate.h>
#include <function2/function2.hpp>
#include <folly/Function.h>
#include <cxx_function/cxx_function.hpp>
#include <elixir/Function.hpp>
#pragma clang diagnostic pop

#include <function/function.hpp>
#include <function/function_ref.hpp>

#include "constructor.hpp"
#include "function_pointer.hpp"
#include "functor.hpp"
#include "lambda.hpp"

namespace wiz {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

// function

    // Function Pointer

    BENCHMARK_TEMPLATE(function_pointer_invoke_test, int(*)(int));
    BENCHMARK_TEMPLATE(function_pointer_invoke_test, std::function<int(int)>);
    BENCHMARK_TEMPLATE(function_pointer_invoke_test, fu2::function<int(int)>);
    BENCHMARK_TEMPLATE(function_pointer_invoke_test, delegate::Delegate<int, int>);
    BENCHMARK_TEMPLATE(function_pointer_invoke_test, folly::Function<int(int)>);
    BENCHMARK_TEMPLATE(function_pointer_invoke_test, cxx_function::function<int(int)>);
    BENCHMARK_TEMPLATE(function_pointer_invoke_test, elixir::Function<int, int>);
    BENCHMARK_TEMPLATE(function_pointer_invoke_test, wiz::function<int(int)>);

    // Functor

//    BENCHMARK_TEMPLATE(constructor_default_test, functor);
//    BENCHMARK_TEMPLATE(constructor_default_test, std::function<int(int)>);
//    BENCHMARK_TEMPLATE(constructor_default_test, fu2::function<int(int)>);
//    BENCHMARK_TEMPLATE(constructor_default_test, delegate::Delegate<int, int>);
//    BENCHMARK_TEMPLATE(constructor_default_test, wiz::function<int(int)>);
//
//    BENCHMARK_TEMPLATE(constructor_invocable_test, functor, functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, std::function<int(int)>, functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, fu2::function<int(int)>, functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, delegate::Delegate<int, int>, functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, wiz::function<int(int)>, functor);
//
//    BENCHMARK_TEMPLATE(functor_invoke_test, functor, functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, std::function<int(int)>, functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, fu2::function<int(int)>, functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, delegate::Delegate<int, int>, functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, wiz::function<int(int)>, functor);
//
//    BENCHMARK_TEMPLATE(constructor_invocable_test, light_functor, light_functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, std::function<int(int)>, light_functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, fu2::function<int(int)>, light_functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, delegate::Delegate<int, int>, light_functor);
//    BENCHMARK_TEMPLATE(constructor_invocable_test, wiz::function<int(int)>, light_functor);
//
//    BENCHMARK_TEMPLATE(functor_invoke_test, light_functor, light_functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, std::function<int(int)>, light_functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, fu2::function<int(int)>, light_functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, delegate::Delegate<int, int>, light_functor);
//    BENCHMARK_TEMPLATE(functor_invoke_test, wiz::function<int(int)>, light_functor);

    BENCHMARK_TEMPLATE(functor_invoke_test, heavy_functor, heavy_functor);
    BENCHMARK_TEMPLATE(functor_invoke_test, std::function<int(int)>, heavy_functor);
    BENCHMARK_TEMPLATE(functor_invoke_test, fu2::function<int(int)>, heavy_functor);
    BENCHMARK_TEMPLATE(functor_invoke_test, delegate::Delegate<int, int>, heavy_functor);
    BENCHMARK_TEMPLATE(functor_invoke_test, folly::Function<int(int)>, heavy_functor);
    BENCHMARK_TEMPLATE(functor_invoke_test, cxx_function::function<int(int)>, heavy_functor);
    BENCHMARK_TEMPLATE(functor_invoke_test, elixir::Function<int, int>, heavy_functor);
    BENCHMARK_TEMPLATE(functor_invoke_test, wiz::function<int(int)>, heavy_functor);

    // lambda

    BENCHMARK_TEMPLATE(lambda_invoke_test, void);
    BENCHMARK_TEMPLATE(lambda_invoke_test, std::function<int(int)>);
    BENCHMARK_TEMPLATE(lambda_invoke_test, fu2::function<int(int)>);
    BENCHMARK_TEMPLATE(lambda_invoke_test, delegate::Delegate<int, int>);
    BENCHMARK_TEMPLATE(lambda_invoke_test, folly::Function<int(int)>);
    BENCHMARK_TEMPLATE(lambda_invoke_test, cxx_function::function<int(int)>);
    BENCHMARK_TEMPLATE(lambda_invoke_test, elixir::Function<int, int>);
    BENCHMARK_TEMPLATE(lambda_invoke_test, wiz::function<int(int)>);

// function_ref

//    BENCHMARK_TEMPLATE(function_pointer_invoke_test, int(*)(int));
//    BENCHMARK_TEMPLATE(function_pointer_invoke_test, folly::FunctionRef<int(int)>);
//    BENCHMARK_TEMPLATE(function_pointer_invoke_test, wiz::function_ref<int(int)>);

#pragma clang diagnostic pop

} // namespace wiz

BENCHMARK_MAIN();
