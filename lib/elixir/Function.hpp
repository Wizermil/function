//
//  Function.hpp
//  elixir
//
//  Created by Pierre on 24/01/2019.
//  Copyright © 2019 Pretty Simple. All rights reserved.
//

#pragma once

extern "C" {
#include <assert.h>
}

#include <new>
#include <type_traits>
#include <utility>

namespace elixir {

    template <typename ReturnType, typename... Args>
    class Function {
    private:
        // Internal classes to be able to store a copy of a callable
        class CallableBase {
        public:
            virtual ~CallableBase() {}

            virtual ReturnType operator()(Args&&... args) = 0;

            virtual CallableBase* clone() = 0;
        };

        template <typename U>
        class Callable final : public CallableBase {
        public:
            Callable(const U& p_callable)
            : callable(p_callable) {}
            Callable(U&& p_callable) noexcept
            : callable(std::move(p_callable)) {}

            ReturnType operator()(Args&&... args) final { return callable(std::forward<Args>(args)...); }

            CallableBase* clone() final { return new Callable(callable); }

        private:
            U callable;
        };

    public:
        Function() = default;

        // Copy a function of the same type
        Function(const Function& other) {
            if (other.callable) {
                // Not storing a Callable<Function> here otherwise we would get an infinite recursion (Function that creates a callable that creates a Function...)
                callable = other.callable->clone();
            }
        }

        // Copy anything that has operator()(Args...)
        template <typename U>
        Function(U other) {
            if constexpr (std::is_move_constructible_v<U>) {
                callable = new Callable<U>(std::move(other));
            } else {
                callable = new Callable<U>(other);
            }
        }

        Function(Function&& other) {
            callable = other.callable;
            other.callable = nullptr;
        }

        Function(decltype(nullptr)) { callable = nullptr; }

        ~Function() {
            if (callable != nullptr)
                delete callable;
        }

        Function& operator=(const Function& other) {
            if (callable != nullptr)
                delete callable;

            // Not storing a Callable<Function> here otherwise we would get an infinite recursion (Function that creates a Callable that creates a Function...)
            if (other.callable)
                callable = other.callable->clone();
            else
                callable = nullptr;
            return *this;
        }

        Function& operator=(Function&& other) {
            if (callable != nullptr)
                delete callable;

            callable = other.callable;
            other.callable = nullptr;
            return *this;
        }

        template <typename U>
        Function& operator=(U other) {
            if (callable != nullptr)
                delete callable;
            if constexpr (std::is_move_constructible_v<U>) {
                callable = new Callable<U>(std::move(other));
            } else {
                callable = new Callable<U>(other); // Copy the callable
            }
            return *this;
        }

        Function& operator=(decltype(nullptr)) {
            if (callable != nullptr)
                delete callable;
            callable = nullptr;
            return *this;
        }

        ReturnType operator()(Args... args) const {
            assert(callable != nullptr);
            return (*callable)(std::forward<Args>(args)...);
        }

        operator bool() const { return callable != nullptr; }

        bool operator==(decltype(nullptr)) noexcept { return callable == nullptr; }

        bool operator!=(decltype(nullptr)) noexcept { return callable != nullptr; }

    private:
        CallableBase* callable = nullptr;
    };
} // namespace elixir
