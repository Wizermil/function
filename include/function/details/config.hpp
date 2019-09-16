#pragma once

#define FUNCTION_HIDDEN [[gnu::visibility("hidden")]]
#define FUNCTION_INTERNAL_LINKAGE [[clang::internal_linkage]]

#define FUNCTION_HIDE_FROM_ABI FUNCTION_HIDDEN FUNCTION_INTERNAL_LINKAGE

#define FUNCTION_LIKELY(exp) (__builtin_expect((exp), true))
#define FUNCTION_UNLIKELY(exp) (__builtin_expect((exp), false))
