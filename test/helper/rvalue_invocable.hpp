#pragma once

struct rvalue_invocable {
    template <typename... Args>
    void operator()(Args&&...) && {}
};
