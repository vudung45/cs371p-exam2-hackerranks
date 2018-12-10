template <typename T> class my_vector {

private:
    T* _b = nullptr;
    T* _e = nullptr;

public:
    using iterator = T*;

    explicit my_vector(size_t s = 0, const T &v = T()) {
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

    my_vector(my_vector&& rhs)
    {
        swap(rhs);
    }

    my_vector(const my_vector &rhs) {
        if (rhs.size() != 0)
            _b = new T[rhs.size()];
        _e = _b + rhs.size();
        copy(rhs.begin(), rhs.end(), begin());
    }

    my_vector &operator=(const my_vector &rhs) {
        // if (rhs.size() != size()) {
        //     delete _b;
        //     _b = new T[rhs.size()];
        //     _e = _b + rhs.size();
        // }
        // copy(rhs.begin(), rhs.end(), _b);
        my_vector that(rhs); //cleaner way to do things
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
        return lexicographical_compare(begin(),end(), rhs.begin(), rhs.end());
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