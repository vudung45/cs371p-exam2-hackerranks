template <typename A, typename BI>
BI my_destroy (A& a, BI b, BI e) {
    while (b != e) {
        --e;
        a.destroy(&*e);
    }
    return b;
}

template <typename A, typename II, typename BI>
BI my_uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
    try {
        while (b != e) {
            a.construct(&*x, *b);
            ++b;
            ++x;
        }
    }
    catch (...) {
        my_destroy(a, p, x);
        throw;
    }
    return x;
}

template <typename A, typename BI, typename U>
void my_uninitialized_fill (A& a, BI b, BI e, const U& v) {
    BI p = b;
    try {
        while (b != e) {
            a.construct(&*b, v);
            ++b;
        }
    }
    catch (...) {
        my_destroy(a, p, b);
        throw;
    }
}

/* actual HR */

/* Notes :
  + Initilize Allocator
*/


template <typename T, typename A = allocator<T>>
class my_vector {
private:
    T *_b = nullptr;
    T *_e = nullptr;
    A _a;

public:
    using iterator = T*;

    explicit my_vector(size_t s = 0, const T &v = T(), const A& a = A()) : _a(a) {
        if (s != 0)
            _b = _a.allocate(s);
        _e = _b + s;
        my_uninitialized_fill(_a, begin(), end(), v);
    }

    my_vector(initializer_list<T> rhs, const A& a = A()): _a(a) {
        if (rhs.size() != 0)
            _b = _a.allocate(rhs.size());
        _e = _b + rhs.size();
        my_uninitialized_copy(_a, rhs.begin(), rhs.end(), _b);
    }

    my_vector(my_vector &&rhs): _a(move(rhs._a)) {
        swap(rhs);
    }

    my_vector(const my_vector &rhs) : _a(rhs._a) {
        if (rhs.size() != 0)
            _b = _a.allocate(rhs.size());
        _e = _b + rhs.size();
        my_uninitialized_copy(_a, rhs.begin(), rhs.end(), begin());
    }

    my_vector &operator=(const my_vector &rhs) {
        // if (rhs.size() != size()) {
        //     delete _b;
        //     _b = new T[rhs.size()];
        //     _e = _b + rhs.size();
        // }
        // copy(rhs.begin(), rhs.end(), _b);
        my_vector that(rhs);
        swap(that);
        return *this;
    }

    my_vector &operator=(my_vector &&rhs) {
        my_vector that(move(rhs));
        swap(that);
        return *this;
    }

    bool operator==(const my_vector &rhs) const {
        return rhs.size() == size() && equal(rhs.begin(), rhs.end(), _b);
    }

    bool operator<(const my_vector &rhs) const {
        return lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
    }

    ~my_vector() {
        delete[] _b;
    }

    T &operator[](size_t i) {
        assert(i < size());
        return _b[i];
    } //  warning: Returning null reference

    const T &operator[](size_t i) const {
        return (*const_cast<my_vector *>(this))[i];
    }

    T *begin() {
        return _b;
    }

    const T *begin() const {
        return const_cast<my_vector *>(this)->begin();
    }

    T *end() {
        return _e;
    }

    const T *end() const {
        return const_cast<my_vector *>(this)->end();
    }

    size_t size() const {
        return _e - _b;
    }

    void swap(my_vector &rhs) {
        std::swap(_b, rhs._b);
        std::swap(_e, rhs._e);
    }
};