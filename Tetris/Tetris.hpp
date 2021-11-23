#pragma once
#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <bitset>

#include "random.hpp"
#include "Frame.hpp"
#include "Shape.hpp"


class Tetris
{
  public:
    Tetris() : Tetris(0.5) { }
    explicit Tetris(float speed, float accel = 0.025);
    template < std::size_t H, std::size_t W >
    void operator () (Frame<H, W>& frame);

  private:
    static Shape get_new_shape();

    void on_move();
    void on_land();
    void draw_board();

    bool move = true, landed = false;
    float speed, acceleration;
    std::array<std::bitset<10>, 20> board;
    Shape shape;
    // TODO: Add shape functionality and playablity
    Frame<20, 10> window;
};


template < std::size_t H, std::size_t W >
void Tetris::operator () (Frame<H, W>& frame)
{
    while (true)
    {
        if (move)
            on_move();
        if (landed)
            on_land();
        shape.draw_to(window);
        frame.draw(0, 0, window);
        speed += acceleration;
    }
}


Tetris::Tetris(float speed, float accel)
    : speed(speed),
      acceleration(accel),
      board({}),
      shape(get_new_shape()),
      window('+')
{ }


void Tetris::on_move()
{
    shape.descend(speed);
    for (auto coord : shape.coords())
    {
        // test if y coordinate is at the bottom to avoid
        // out_of_range error in the next branch condition
        if (board.at(coord.y).test(coord.x))
            landed = true;
        move = !landed;
    }
}


void Tetris::on_land()
{
    landed = false;
    // detects full rows, starting at the bottom,
    // and shifts the board down when one is found,
    // deleting the full rows
    for (int i = 19; i > 0; i--)
    {
        if (board[i].all())
        {
            for (int j = i; j > 0; j--)
                board[j] = board[j - 1];
            board[0].reset();
        }
    }
    shape = get_new_shape();
}


Shape Tetris::get_new_shape()
{
    // 0 - 7 because a shape will likely extend out to
    // the right at 1 or 2 blocks
    int x_pos = rand::random<int>(0, 7);
    ShapeID id = ShapeID(rand::random<char>(0, 7));
    return Shape(id, x_pos);
}


#endif
