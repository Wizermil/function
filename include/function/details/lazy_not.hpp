#pragma once

namespace wiz::details {
    template <typename Pred>
    struct _lazy_not {
        static constexpr bool const value = !Pred::value;
    };
} // namespace wiz::details
