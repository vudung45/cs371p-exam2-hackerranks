#include <cassert>  // assert
#include <iostream> // cout, endl, istream, ostream
#include <sstream>  // istringstream, ostringstream
#include <utility>  // !=

using namespace std;
using rel_ops::operator!=;

class Shape {
  friend istream &operator>>(istream &in, Shape &c) {
    return c.opt1(in);
  }

  friend ostream &operator<<(ostream &out, const Shape &c) {
    return c.opt2(out);
  }

private:
  int _l;
  int _h;

protected:
  virtual bool equal(const Shape& rhs) const {
      return rhs._l == _l && rhs._h == _h;
  }

  virtual istream &opt1(istream &in) { 
      return in >> _l >> _h; 
  }

  virtual ostream &opt2(ostream &out) const { return out << _l << " " << _h; }

public:
  Shape(int l, int h) : _l(l), _h(h) {}

  Shape(const Shape &) = default;

  virtual ~Shape() {}

  bool operator==(const Shape &rhs) const {
    return equal(rhs);
  }

  void move(int l, int h) {
    _l = l;
    _h = h;
  }

  virtual double area() const { return 0; }
};

class Circle : public Shape {
    friend istream &operator>>(istream &in, Circle &c) {
        return c.opt1(in);
    }

    friend ostream &operator<<(ostream &out, const Circle &c) {
        return c.opt2(out);
    }

    private:
        int _r;

    protected:
      virtual bool equal(const Shape &rhs) const {
        return Shape::equal(rhs) && dynamic_cast<const Circle&>(rhs)._r == _r;
      }

      virtual istream &opt1(istream &in) { return Shape::opt1(in) >> _r; }

      virtual ostream &opt2(ostream &out) const {
        return Shape::opt2(out) << " " << _r;
      }

    public:
        Circle(int l, int h, int r) : Shape(l, h), _r(r) {}

        Circle(const Circle &) = default;

        int radius() const { return _r; }

        bool operator==(const Shape &rhs) const {
            return equal(rhs);
        }

        double area() const { return 3.14 * _r * _r; }
};