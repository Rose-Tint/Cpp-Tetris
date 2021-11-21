#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <cstddef>
#include <cmath>

#include "math/Vector.hpp"


class Rectangle
{
  public:
    Rectangle(std::size_t h, std::size_t w, float x = 0, float y = 0)
        : _x(x), _y(y), h(h), w(w) { }

    float x() const { return _x; }
    float y() const { return _y; }
    void x(float x) { _x = x; }
    void y(float y) { _y = y; }
    float bottom() const { return _y + h; }
    float right() const { return _x + w; }
    std::size_t height() const { return h; }
    std::size_t width() const { return w; }

  private:
    float _x;
    float _y;
    std::size_t h;
    std::size_t w;
};


void move_rect(const Vector& v, Rectangle& r, float s = 1)
{
    r.x((r.x() + v.x()) * s);
    r.y((r.y() + v.y()) * s);
}


void move_rect(const Vector& v, Rectangle& r, float xs, float ys)
{
    r.x((r.x() + v.x()) * xs);
    r.y((r.y() + v.y()) * ys);
}


void move_rect(const Vector& v, Rectangle& r, const Vector& s)
{
    r.x(r.x() + v.x() * s.x());
    r.y(r.y() + v.y() * s.y());
}

#endif
