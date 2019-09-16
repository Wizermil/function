#pragma once

#include <type_traits>

#include <function/details/config.hpp>
#include <function/details/policy_storage.hpp>

namespace wiz::details {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-template"
#pragma clang diagnostic ignored "-Wpadded"

    struct unique_policy {
        void (*const destroy)(void*);
        bool const is_null;

        template <typename F>
        FUNCTION_HIDE_FROM_ABI static const unique_policy* create() {
            if constexpr (use_small_storage_v<F>) {
                static constexpr unique_policy const _policy = {nullptr, false};
                return &_policy;
            } else {
                static constexpr unique_policy const _policy = {&_large_destroy<F>, false};
                return &_policy;
            }
        }

        FUNCTION_HIDE_FROM_ABI static const unique_policy* create_empty() {
            static constexpr unique_policy const _policy = {nullptr, true};
            return &_policy;
        }

    private:
        template <typename F>
        FUNCTION_HIDE_FROM_ABI static void _large_destroy(void* s) {
            F::destroy_and_delete(static_cast<F*>(s));
        }
    };

#pragma clang diagnostic pop

} // namespace wiz::details
