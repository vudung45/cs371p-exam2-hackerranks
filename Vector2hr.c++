#include <algorithm>        // copy, equal, fill, swap
#include <cassert>          // assert
#include <cstddef>          // ptrdiff_t, size_t
#include <initializer_list> // initializer_list
#include <iostream>         // cout, endl

using namespace std;

template <typename T> class my_vector {

private:
    T* _b = nullptr;
    T* _e = nullptr;

public:
    explicit my_vector(size_t s = 0, const T& v = T()) {
        if (s != 0)
            _b = new T[s];
        _e = _b + s;
        fill(begin(), end(), v);
    }

    my_vector(initializer_list<T> rhs) {
        if (rhs.size() != 0)
            _b = new T[rhs.size()];
        _e = _b + rhs.size();
        copy(rhs.begin(), rhs.end(), begin());
    }

    my_vector(const my_vector& rhs) {
        if (rhs.size() != 0)
            _b = new T[rhs.size()];
        _e = _b + rhs.size();
        copy(rhs.begin(), rhs.end(), begin());
    }

    my_vector &operator=(const my_vector & rhs) {
        if(rhs.size() != size())
        {
            delete _b;
            _b = new T[rhs.size()];
            _e = _b + rhs.size();
        }
        copy(rhs.begin(), rhs.end(), _b);
        return *this;
    }

    bool operator==(const my_vector& rhs) const {
        return rhs.size() == size() && equal(rhs.begin(), rhs.end(), _b);
    }

    ~my_vector() {
        delete[] _b;
    }

    T& operator[](size_t i) {
        assert(i < size());
        return _b[i];
    } //  warning: Returning null reference

    const T& operator[](size_t i) const {
        return (*const_cast<my_vector *>(this))[i];
    }

    T* begin() {
        return _b;
    }

    const T* begin() const {
        return const_cast<my_vector *>(this)->begin();
    }

    T* end() {
        return _e;
    }

    const T* end() const {
        return const_cast<my_vector *>(this)->end();
    }

    size_t size() const {
        return _e - _b;
    }
};