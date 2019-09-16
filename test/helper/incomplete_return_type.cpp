#include "incomplete_return_type.hpp"

x::~x() {
}

int called = 0;
incomplete_return_type test_fn() {
    ++called;
    incomplete_return_type I;
    return I;
}
