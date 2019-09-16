#pragma once

class functor {
    int _data[10];

public:
    static int count;

    functor();
    explicit functor(int j);
    functor(functor const&);
    ~functor();

    int operator()(int i) const;

    inline int id() const noexcept { return _data[0]; }

    int func(int) const;
};

class small_functor {
    int _data;

public:
    static int count;

    small_functor();
    explicit small_functor(int j);
    small_functor(small_functor const&);
    ~small_functor();

    int operator()(int i) const;

    inline int id() const noexcept { return _data; }

    int func(int) const;
};
