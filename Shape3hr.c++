#include <cassert>  // assert
#include <iostream> // cout, endl, istream, ostream
#include <sstream>  // istringstream, ostringstream
#include <utility>  // !=

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
    virtual bool equal(const AbstractShape& rhs) const {
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
        return AbstractShape::equal(rhs) && dynamic_cast<const Circle&>(rhs)._r == _r;
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

    Circle* clone() const override {
        return new Circle(*this);
    }

    bool operator==(const AbstractShape &rhs) const {
        return equal(rhs);
    }

    double area() const override {
        return 3.14 * _r * _r;
    }
};