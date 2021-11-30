#pragma once
#ifndef TETRIS_TETRIS_HPP
#define TETRIS_TETRIS_HPP

#include <bitset>
#include <array>
#include <cstdint>
#include <chrono>

#include "Tetris/Shape.hpp"


class Screen;


class Tetris
{
  public:
    using size_type = std::uint_fast16_t;
    using millis = std::chrono::milliseconds;

    Tetris(Screen& scn, millis spd, millis acc);
    void operator () ();

  private:
    static Shape get_new_shape();

    bool landed = false;
    const millis acc;
    millis speed;
    std::array<std::bitset<10>, 20> board;
    Shape shape;
    Screen& screen;
};

#endif
