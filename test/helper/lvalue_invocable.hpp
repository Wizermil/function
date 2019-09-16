#pragma once

struct lvalue_invocable {
    template <typename... Args>
    void operator()(Args&&...) & {}
};
