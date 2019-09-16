#pragma once

#include <function/function.hpp>

struct x {
    using callback_type = wiz::function<void(x&)>;
    virtual ~x();

private:
    callback_type _cb;
};

struct incomplete_return_type {
    wiz::function<incomplete_return_type()> fn;
};

extern int called;
incomplete_return_type test_fn();
