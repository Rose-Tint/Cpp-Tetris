#pragma once
#ifndef TETRIS_SHAPE_HPP
#define TETRIS_SHAPE_HPP

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
    using size_type = std::int_fast8_t;

    Shape(ShapeID id, size_type x_pos, size_type y_pos);

    Shape& operator = (Shape&& other);

    void draw(Screen& scn, bool reset = false) const;
    void rotate();
    size_type left_most() const;
    size_type right_most() const;
    std::array<std::pair<size_type, size_type>, 4>
        coords() const;
    void descend(size_type spd = 1) { y_pos += spd; }

  private:
    size_type x_pos, y_pos;
    std::array<std::bitset<4>, 4> matrix;
    const ShapeID id;

    char id_as_char() const;
    void reset_matrix();
};


#endif