#include <algorithm> // swap
#include <cassert>   // assert
#include <iostream>  // cout, endl, istream, ostream
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

    Circle& operator = (const Circle&) = default;

    int radius() const {
        return _r;
    }

    Circle *clone() const override {
        return new Circle(*this);
    }

    bool operator==(const AbstractShape &rhs) const {
        return equal(rhs);
    }

    double area() const override {
        return 3.14 * _r * _r;
    }
};

template<typename T>
class Handle
{

    struct count {
        T* _p;
        int _c;
        count(T* p) : _p(p), _c(1) { }

        ~count()
        {
            delete _p;
        }
    };

    friend bool operator==(const Handle &lhs, const Handle &rhs) {
        //sanity check
        if(lhs.get_ptr() == nullptr || rhs.get_ptr() == nullptr)
            return lhs.get_ptr() == rhs.get_ptr();
        else
            return *(lhs.get_ptr()) == *(rhs.get_ptr());
    }

    struct count* rc = nullptr; //reference counter


protected:
    T *get_ptr() {
        if(!unique())
        {
            --(rc->_c);
            rc = new count(rc->_p->clone());
        }
        return rc->_p;
    }

    const T *get_ptr() const {
        return rc->_p;
    }

public:
    Handle(T *p)
    {
        if(p != nullptr)
            rc = new count(p);
    }

    Handle(const Handle &rhs) : rc(rhs.rc) {
        if(rc != nullptr)
            ++rc->_c;
    }

    Handle &operator=(const Handle &rhs) {
        // delete _p;
        // _p = rhs._p->clone();
        Handle<T> that(rhs);
        swap(that);
        return *this;
    }

    void swap(Handle& rhs)
    {
        std::swap(rc, rhs.rc);
    }

    T& operator * () const {
        return *get_ptr();
    }

    T* operator ->() const {
        return get_ptr();
    }

    bool unique() const
    {
        if(rc != nullptr)
            return rc->_c <= 1;
        return true;
    }

    ~Handle()
    {
        if(unique())
            delete rc;
        else
            --rc->_c;
    }
};

class Shape: public Handle<AbstractShape> {
public:
    Shape(AbstractShape *p) : Handle<AbstractShape>(p) {}

    Shape(const Shape&) = default;
    ~Shape() = default;
    Shape &operator=(const Shape &) = default;

    double area() const {
        return get_ptr()->area();
    }
    void move(int l, int h)
    {
        get_ptr()->move(l,h);
    }
};

TEST(HandleFixture, test1) {
    const Shape x = new Circle(2, 3, 4);
//  x.move(5, 6);                        // doesn't compile
    ASSERT_EQ(x.area(), 3.14 * 4 * 4);
//  x.radius();                          // doesn't compile
    ASSERT_TRUE(x.unique());
}

TEST(HandleFixture, test2) {
    Shape x = new Circle(2, 3, 4);
    x.move(5, 6);
    ASSERT_EQ(x,        new Circle(5, 6, 4));
    ASSERT_EQ(x.area(), 3.14 * 4 * 4);
//  x.radius();                        // doesn't compile
    ASSERT_TRUE(x.unique());
}

TEST(HandleFixture, test3) {
    const Shape x = new Circle(2, 3, 4);
    Shape y = x;
    ASSERT_EQ(x, y);
    ASSERT_FALSE(x.unique());
    ASSERT_FALSE(y.unique());
    y.move(5, 6);
    ASSERT_EQ(x, new Circle(2, 3, 4));
    ASSERT_EQ(y, new Circle(5, 6, 4));
    ASSERT_TRUE(x.unique());
    ASSERT_TRUE(y.unique());
    ASSERT_EQ(y.area(), 3.14 * 4 * 4);
}

TEST(HandleFixture, test4) {
    const Shape x = new Circle(2, 3, 4);
    Shape y = new Circle(2, 3, 5);
    ASSERT_NE(x, y);
    y = x;
    ASSERT_EQ(x, y);
    ASSERT_FALSE(x.unique());
    ASSERT_FALSE(y.unique());
    y.move(5, 6);
    //ASSERT_EQ(new Circle(2, 3, 4), x);
    //ASSERT_EQ(new Circle(5, 6, 4), y);
    ASSERT_TRUE(x.unique());
    ASSERT_TRUE(y.unique());
    ASSERT_EQ(y.area(), 3.14 * 4 * 4);
}


