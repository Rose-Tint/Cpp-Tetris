#pragma once

#include <bitset>
#include <array>
#include <cstdint>

#include "Screen.hpp"


/* SHAPE KEY
* T shape: | L shape: | J shape: | I shape:
* 0 1 0 0  | 1 0 0 0  | 0 1 0 0  | 1 0 0 0 
* 2 3 4 0  | 2 0 0 0  | 0 2 0 0  | 2 0 0 0 
* 0 0 0 0  | 3 4 0 0  | 4 3 0 0  | 3 0 0 0 
* 0 0 0 0  | 0 0 0 0  | 0 0 0 0  | 4 0 0 0 
* ---------+----------+----------+---------
* O shape: | S shape: | Z shape: |
* 1 2 0 0  | 0 3 4 0  | 1 2 0 0  |
* 4 3 0 0  | 1 2 0 0  | 0 3 4 0  |
* 0 0 0 0  | 0 0 0 0  | 0 0 0 0  |
* 0 0 0 0  | 0 0 0 0  | 0 0 0 0  |
*/


enum struct ShapeID : char { T, L, J, I, O, S, Z, };


class Shape
{
  public:
    using size_type = std::uint_fast16_t;

    Shape(ShapeID id, size_type x_pos, size_type y_pos);
    Shape(const Shape& other);

    Shape& operator = (const Shape& other);
    Shape& operator = (Shape&& other);

    void draw(Screen& scn, bool reset = false) const;
    void rotate_cw();
    void rotate_cc();
    size_type get_x() const { return x_pos; }
    void go_right() { x_pos++; }
    void go_left() { x_pos--; }
    size_type height() const;
    size_type width() const;
    size_type set_y(size_type y) { return (y_pos = y); }
    std::array<std::pair<size_type, size_type>, 4>
        coords() const;
    void descend(size_type dist = 1) { y_pos += dist; }

  private:
    size_type x_pos, y_pos;
    std::array<std::bitset<4>, 4> matrix;
    const ShapeID id;

    char id_as_char() const;
    void reset_matrix();
};
