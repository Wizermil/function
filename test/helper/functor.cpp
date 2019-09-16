#include "functor.hpp"

int functor::count = 0;

functor::functor() {
    ++count;
    for (int i = 0; i < 10; ++i) {
        _data[i] = i;
    }
}

functor::functor(int j) {
    ++count;
    for (int i = 0; i < 10; ++i) {
        _data[i] = i;
    }
    _data[0] = j;
}

functor::functor(functor const& other) {
    ++count;
    for (int i = 0; i < 10; ++i) {
        _data[i] = other._data[i];
    }
}

functor::~functor() {
    --count;
}

int functor::operator()(int i) const {
    for (int j = 0; j < 10; ++j) {
        i += _data[j];
    }
    return i;
}

int functor::func(int) const {
    return 1;
}

// small_functor

int small_functor::count = 0;

small_functor::small_functor()
: _data{0} {
    ++count;
}

small_functor::small_functor(int j)
: _data{j} {
    ++count;
}

small_functor::small_functor(small_functor const& other)
: _data{other._data} {
    ++count;
}

small_functor::~small_functor() {
    --count;
}

int small_functor::operator()(int i) const {
    return i + _data;
}

int small_functor::func(int) const {
    return 1;
}
