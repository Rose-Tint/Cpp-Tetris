#pragma once

#include <bitset>
#include <array>
#include <cstdint>

#include "../Screen.hpp"


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
    using UIntFast = std::uint_fast8_t;

    Shape(ShapeID id, UIntFast x_pos, UIntFast y_pos);
    Shape(const Shape& other);

    Shape& operator = (const Shape& other);
    Shape& operator = (Shape&& other);

    void Draw(Screen& scn, bool reset = false) const;
    void RotateCW();
    void RotateCC();
    UIntFast X() const { return x_pos; }
    void MoveRight(UIntFast count = 1) { x_pos += count; }
    void MoveLeft(UIntFast count = 1) { x_pos -= count; }
    UIntFast Height() const;
    UIntFast Width() const;
    UIntFast SetY(UIntFast y) { return (y_pos = y); }
    std::array<std::pair<UIntFast, UIntFast>, 4>
        Coords() const;
    void Descend(UIntFast dist = 1) { y_pos += dist; }

  private:
    UIntFast x_pos: 4;
    UIntFast y_pos: 4;
    std::array<std::bitset<4>, 4> matrix;
    const ShapeID id;

    char id_as_char() const;
    void reset_matrix();
    void justify();
};
