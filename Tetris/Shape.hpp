#pragma once
#ifndef TETRIS_PIECE_HPP
#define TETRIS_PIECE_HPP

#include <utility>
#include <array>


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


// TODO: Finish rotations


enum struct ShapeID: char { T, L, J, I, O, S, Z, };


struct Coordinate { float x, y; };


class Shape
{
  public:
    explicit Shape(ShapeID id, int tl_x = 0);

    template < std::size_t H, std::size_t W >
    void draw_to(Frame<H, W>& frame, int x_off = 0, int y_off = 0);
    void rotate_cl() { rotate(true); }
    void rotate_cc() { rotate(false); }
    void rotate(bool cl = true);
    std::array<Coordinate, 4> coords()
        { return { c1, c2, c3, c4 }; }
    void descend(float spd = 1.0)
    {
        c1.y += spd;
        c2.y += spd;
        c3.y += spd;
        c4.y += spd;
    }

  private:
    void rotate_as_T(bool cl);
    void rotate_as_L(bool cl);
    void rotate_as_J(bool cl);
    void rotate_as_I(bool cl);
    void rotate_as_O(bool cl);
    void rotate_as_S(bool cl);
    void rotate_as_Z(bool cl);

    ShapeID id;
    enum { up, right, down, left } orient;
    Coordinate c1, c2, c3, c4;
};


Shape::Shape(ShapeID id, int tl_x): id(id)
{
    switch (id)
    {
      case ShapeID::T:{
        c1.x = c3.x = tl_x + 1;
        c2.x = tl_x;
        c4.x = tl_x + 2;
        c1.y = 0;
        c2.y = c3.y = c4.y = 1;
      } break;
      case ShapeID::L:{
        c1.x = c2.x = c3.x = tl_x;
        c4.x = tl_x + 1;
        c4.y = c3.y = 1 + (c2.y = 1 + (c1.y = 0));
      } break;
      case ShapeID::J:{
        c1.x = c2.x = c3.x = tl_x + 1;
        c4.x = tl_x;
        c4.y = c3.y = 1 + (c2.y = 1 + (c1.y = 0));
      } break;
      case ShapeID::I:{
        c1.x = c2.x = c3.x = c4.x = tl_x;
        c4.y = 1 + (c3.y = 1 + (c2.y = 1 + (c1.y = 0)));
      } break;
      case ShapeID::O:{
        c1.x = c4.x = tl_x;
        c2.x = c3.x = tl_x + 1;
        c4.x = tl_x + 2;
        c4.y = c3.y = 1 + (c2.y = c1.y = 0);
      } break;
      case ShapeID::S:{
        c1.x = tl_x;
        c2.x = c3.x = tl_x + 1;
        c4.x = tl_x + 2;
        c1.y = c2.y = 1 + (c3.y = c4.y = 0);
      } break;
      case ShapeID::Z:{
        c1.x = tl_x;
        c2.x = c3.x = tl_x + 1;
        c4.x = tl_x + 2;
        c4.y = c3.y = 1 + (c2.y = c1.y = 0);
      } break;
    }
}


template < std::size_t H, std::size_t W >
void Shape::draw_to(Frame<H, W>& frame, int x_off, int y_off)
{
    frame.set('#', c1.x + x_off, c1.y + y_off);
    frame.set('#', c2.x + x_off, c2.y + y_off);
    frame.set('#', c3.x + x_off, c3.y + y_off);
    frame.set('#', c4.x + x_off, c4.y + y_off);
}


void Shape::rotate(bool cl)
{
    switch (id)
    {
      case ShapeID::T: rotate_as_T(cl); break;
      case ShapeID::L: rotate_as_L(cl); break;
      case ShapeID::J: rotate_as_J(cl); break;
      case ShapeID::I: rotate_as_I(cl); break;
      case ShapeID::O: rotate_as_O(cl); break;
      case ShapeID::S: rotate_as_S(cl); break;
      case ShapeID::Z: rotate_as_Z(cl); break;
    }
}

void Shape::rotate_as_T(bool cl)
{
    if (cl)
    {
        switch (orient)
        {
          case up:{
            orient = right;
          } break;
          case left:{
            orient = up;
          } break;
          case down:{
            orient = left;
          } break;
          case right:{
            orient = down;
          } break;
        }
    }
    else
    {
        switch (orient)
        {
          case up:{
            orient = left;
          } break;
          case left:{
            orient = down;
          } break;
          case down:{
            orient = right;
          } break;
          case right:{
            orient = up;
          } break;
        }
    }
}


void Shape::rotate_as_L(bool cl)
{
    if (cl)
    {
        switch (orient)
        {
          case up:{
            orient = right;
          } break;
          case left:{
            orient = up;
          } break;
          case down:{
            orient = left;
          } break;
          case right:{
            orient = down;
          } break;
        }
    }
    else
    {
        switch (orient)
        {
          case up:{
            orient = left;
          } break;
          case left:{
            orient = down;
          } break;
          case down:{
            orient = right;
          } break;
          case right:{
            orient = up;
          } break;
        }
    }
}


void Shape::rotate_as_J(bool cl)
{
    if (cl)
    {
        switch (orient)
        {
          case up:{
            orient = right;
          } break;
          case left:{
            orient = up;
          } break;
          case down:{
            orient = left;
          } break;
          case right:{
            orient = down;
          } break;
        }
    }
    else
    {
        switch (orient)
        {
          case up:{
            orient = left;
          } break;
          case left:{
            orient = down;
          } break;
          case down:{
            orient = right;
          } break;
          case right:{
            orient = up;
          } break;
        }
    }
}


void Shape::rotate_as_I(bool cl)
{
    if (cl)
    {
        switch (orient)
        {
          case up:{
            orient = right;
          } break;
          case left:{
            orient = up;
          } break;
          case down:{
            orient = left;
          } break;
          case right:{
            orient = down;
          } break;
        }
    }
    else
    {
        switch (orient)
        {
          case up:{
            orient = left;
          } break;
          case left:{
            orient = down;
          } break;
          case down:{
            orient = right;
          } break;
          case right:{
            orient = up;
          } break;
        }
    }
}


void Shape::rotate_as_O(bool cl)
{
    if (cl)
    {
        switch (orient)
        {
          case up:{
            orient = right;
          } break;
          case left:{
            orient = up;
          } break;
          case down:{
            orient = left;
          } break;
          case right:{
            orient = down;
          } break;
        }
    }
    else
    {
        switch (orient)
        {
          case up:{
            orient = left;
          } break;
          case left:{
            orient = down;
          } break;
          case down:{
            orient = right;
          } break;
          case right:{
            orient = up;
          } break;
        }
    }
}


void Shape::rotate_as_S(bool cl)
{
    if (cl)
    {
        switch (orient)
        {
          case up:{
            orient = right;
          } break;
          case left:{
            orient = up;
          } break;
          case down:{
            orient = left;
          } break;
          case right:{
            orient = down;
          } break;
        }
    }
    else
    {
        switch (orient)
        {
          case up:{
            orient = left;
          } break;
          case left:{
            orient = down;
          } break;
          case down:{
            orient = right;
          } break;
          case right:{
            orient = up;
          } break;
        }
    }
}


void Shape::rotate_as_Z(bool cl)
{
    if (cl)
    {
        switch (orient)
        {
          case up:{
            orient = right;
          } break;
          case left:{
            orient = up;
          } break;
          case down:{
            orient = left;
          } break;
          case right:{
            orient = down;
          } break;
        }
    }
    else
    {
        switch (orient)
        {
          case up:{
            orient = left;
          } break;
          case left:{
            orient = down;
          } break;
          case down:{
            orient = right;
          } break;
          case right:{
            orient = up;
          } break;
        }
    }
}


#endif