#include <cassert>  // assert
#include <iostream> // cout, endl, istream, ostream
#include <sstream>  // istringstream, ostringstream
#include <utility>  // !=

using namespace std;
using rel_ops::operator!=;


class Shape {
    friend istream &operator>>(istream &in, Shape &c) {
          return in >> c._l >> c._h;
        }

    friend ostream &operator<<(ostream &out, const Shape &c) {
        return out << c._l << " " << c._h;
    }
    private:
        int _l;
        int _h;

      public:
        Shape(int l, int h) : _l(l), _h(h){}

        Shape(const Shape &) = default;


        bool operator==(const Shape& rhs) const{
            return _l == rhs._l && _h == rhs._h;
        }

        void move(int l, int h)
        {
            _l = l;
            _h = h;
        }

        virtual ~Shape() {}

        double area() const { return 0; }
};

class Circle: public Shape {
    friend istream &operator>>(istream &in, Circle &c) {
          return in >> static_cast<Shape&>(c) >> c._r;
    }

    friend ostream &operator<<(ostream &out, const Circle &c) {
        return out << static_cast<const Shape&>(c) << " " << c._r;
    }
    private:
        int _r;

        
    public:
        Circle(int l, int h, int r) : Shape(l, h), _r(r){}

        Circle(const Circle &) = default;

        int radius() const{
            return _r;
        }

        bool operator==(const Circle& rhs) const{
          return static_cast<const Shape &>(rhs) == *this && _r == rhs._r;
        }

        double area() const { return 3.14 * _r * _r; }
};