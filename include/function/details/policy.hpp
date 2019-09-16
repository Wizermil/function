#pragma once

#include <type_traits>

#include <function/details/config.hpp>
#include <function/details/policy_storage.hpp>

namespace wiz::details {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-template"
#pragma clang diagnostic ignored "-Wpadded"

    struct policy {
        void* (*const clone)(void const*);
        void (*const destroy)(void*);
        bool const is_null;

        template <typename F>
        FUNCTION_HIDE_FROM_ABI static const policy* create() {
            if constexpr (use_small_storage_v<F>) {
                static constexpr policy const _policy = {nullptr, nullptr, false};
                return &_policy;
            } else {
                static constexpr policy const _policy = {&_large_clone<F>, &_large_destroy<F>, false};
                return &_policy;
            }
        }

        FUNCTION_HIDE_FROM_ABI static const policy* create_empty() {
            static constexpr policy const _policy = {nullptr, nullptr, true};
            return &_policy;
        }

    private:
        template <typename F>
        FUNCTION_HIDE_FROM_ABI static void* _large_clone(void const* s) {
            F const* f = static_cast<F const*>(s);
            return f->clone();
        }

        template <typename F>
        FUNCTION_HIDE_FROM_ABI static void _large_destroy(void* s) {
            F::destroy_and_delete(static_cast<F*>(s));
        }
    };

#pragma clang diagnostic pop

} // namespace wiz::details
