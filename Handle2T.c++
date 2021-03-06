#include <algorithm> // swap
#include <typeinfo>  // bad_cast
#include <utility>   // !=

#include "gtest/gtest.h"

using namespace std;
using rel_ops::operator!=;

class AbstractShape {
    friend istream &operator>>(istream &in, AbstractShape &c) {
        return c.opt1(in);
    }

    friend ostream &operator<<(ostream &out, const AbstractShape &c) {
        return c.opt2(out);
    }

private:
    int _l;
    int _h;

protected:
    virtual bool equal(const AbstractShape &rhs) const {
        return rhs._l == _l && rhs._h == _h;
    }

    virtual istream &opt1(istream &in) {
        return in >> _l >> _h;
    }

    virtual ostream &opt2(ostream &out) const {
        return out << _l << " " << _h;
    }

public:
    AbstractShape(int l, int h) : _l(l), _h(h) {}

    AbstractShape(const AbstractShape &) = default;

    virtual ~AbstractShape() {}

    bool operator==(const AbstractShape &rhs) const {
        return equal(rhs);
    }

    void move(int l, int h) {
        _l = l;
        _h = h;
    }
    virtual AbstractShape *clone() const = 0;

    virtual double area() const {
        return 0;
    }
};

class Circle : public AbstractShape {
    friend istream &operator>>(istream &in, Circle &c) {
        return c.opt1(in);
    }

    friend ostream &operator<<(ostream &out, const Circle &c) {
        return c.opt2(out);
    }

private:
    int _r;

protected:
    virtual bool equal(const AbstractShape &rhs) const override {
        return AbstractShape::equal(rhs) &&
               dynamic_cast<const Circle &>(rhs)._r == _r;
    }

    virtual istream &opt1(istream &in) override {
        return AbstractShape::opt1(in) >> _r;
    }

    virtual ostream &opt2(ostream &out) const override {
        return AbstractShape::opt2(out) << " " << _r;
    }

public:
    Circle(int l, int h, int r) : AbstractShape(l, h), _r(r) {}

    Circle(const Circle &) = default;

    ~Circle() = default;

    int radius() const {
        return _r;
    }

    AbstractShape *clone() const override {
        return new Circle(*this);
    }

    bool operator==(const AbstractShape &rhs) const {
        return equal(rhs);
    }

    double area() const override {
        return 3.14 * _r * _r;
    }
};

template <typename T>
class Handle {
    T *_p = nullptr;

    friend bool operator==(const Handle &lhs, const Handle &rhs) {
        // sanity check
        if (lhs.get_ptr() == nullptr || rhs.get_ptr() == nullptr)
            return lhs.get_ptr() == rhs.get_ptr();
        else
            return *(lhs.get_ptr()) == *(rhs.get_ptr());
    }

protected:

    T *get_ptr() const {
        return _p;
    }

public:
    Handle(T *p) : _p(p) {}

    Handle(const Handle &rhs) {
        if (rhs._p != nullptr)
            _p = rhs._p->clone();
    }

    Handle &operator=(const Handle &rhs) {
        // delete _p;
        // _p = rhs._p->clone();
        Handle<T> that(rhs);
        swap(that);
        return *this;
    }

    void swap(Handle &rhs) {
        std::swap(_p, rhs._p);
    }

    T& operator*() const {
        return *get_ptr();
    }

    T *operator->() const {
        return get_ptr();
    }

    ~Handle() {
        delete _p;
    }
};

/*
------
TESTS
------
*/

using Shape       = Handle<      AbstractShape>;
using Const_Shape = Handle<const AbstractShape>;

TEST(HandleFixture, test1) {
    Const_Shape x = new Circle(2, 3, 4);
//  x->move(5, 6);                                                             // doesn't compile
    ASSERT_EQ(x->area(), 3.14 * 4 * 4);
//  x->radius();                                                               // doesn't compile
    if (const Circle* const p = dynamic_cast<const Circle*>(x.operator->())) {
        ASSERT_EQ(p->radius(), 4);
    }
    try {
        const Circle& r = dynamic_cast<const Circle&>(*x);
        ASSERT_EQ(4, r.radius());
    }
    catch (const bad_cast& e) {
        assert(false);
    }
}

TEST(HandleFixture, test2) {
    Shape x = new Circle(2, 3, 4);
    x->move(5, 6);
    ASSERT_EQ(x,         new Circle(5, 6, 4));
    ASSERT_EQ(x->area(), 3.14 * 4 * 4);
//  x.radius();                                                                // doesn't compile
    if (const Circle* const p = dynamic_cast<const Circle*>(x.operator->())) {
        ASSERT_EQ(p->radius(), 4);
    }
    try {
        const Circle& r = dynamic_cast<const Circle&>(*x);
        ASSERT_EQ(r.radius(), 4);
    }
    catch (const bad_cast& e) {
        assert(false);
    }
}

TEST(Handle_Fixture, test3) {
    const Shape x = new Circle(2, 3, 4);
    Shape y = x;
    ASSERT_EQ(x, y);
    y->move(5, 6);
    ASSERT_EQ(x,         new Circle(2, 3, 4));
    ASSERT_EQ(y,         new Circle(5, 6, 4));
    ASSERT_EQ(y->area(), 3.14 * 4 * 4);
}

TEST(Handle_Fixture, test4) {
    const Shape x = new Circle(2, 3, 4);
    Shape y = new Circle(2, 3, 5);
    ASSERT_NE(x, y);
    y = x;
    ASSERT_EQ(x, y);
    y->move(5, 6);
    ASSERT_EQ(x,         new Circle(2, 3, 4));
    ASSERT_EQ(y,         new Circle(5, 6, 4));
    ASSERT_EQ(y->area(), 3.14 * 4 * 4);
}

