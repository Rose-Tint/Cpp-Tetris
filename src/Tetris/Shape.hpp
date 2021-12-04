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
    using Coordinates = std::array<std::pair<UIntFast, UIntFast>, 4>;

    Shape(const ShapeID);
    Shape(const ShapeID id, const UIntFast x_pos, const UIntFast y_pos);
    Shape(const Shape& other);

    Shape& operator = (const Shape& other);
    Shape& operator = (Shape&& other);

    void Draw(Screen& scr) const;
    void Stain(Screen& scr) const;
    void EraseLast(Screen& scr) const;
    void RotateCW();
    void RotateCC();
    UIntFast X() const { return x_pos; }
    void MoveRight(UIntFast count = 1) { x_pos += count; }
    void MoveLeft(UIntFast count = 1) { x_pos -= count; }
    UIntFast Height() const;
    UIntFast Width() const;
    UIntFast SetY(UIntFast y) { return (y_pos = y); }
    UIntFast SetX(UIntFast x) { return (x_pos = x); }
    Coordinates Coords() const;
    void Descend(UIntFast dist = 1) { y_pos += dist; }
    void ResetPos() { y_pos = 4; x_pos = 2; }
    void FullReset() { y_pos = 4; x_pos = 2; reset_matrix(); }

    void swap(Shape& other);

  private:
    UIntFast x_pos, y_pos;
    std::array<std::bitset<4>, 4> matrix;
    const ShapeID id;
    mutable Coordinates prev_crds { };

    char id_as_char() const;
    void reset_matrix();
    void justify();
};
