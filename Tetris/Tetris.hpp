#pragma once
#ifndef TETRIS_TETRIS_HPP
#define TETRIS_TETRIS_HPP

#include <bitset>
#include <array>
#include <cstdint>
#include <chrono>

#include "Tetris/Shape.hpp"


class Screen;


void TetrisFunc(Screen& screen);


class Tetris
{
  public:
    static constexpr std::uint_fast8_t H = 24;
    static constexpr std::uint_fast8_t W = 10;

    using board_type = std::array<std::bitset<W>, H>;
    using uint = std::uint_fast16_t;
    using millis = std::chrono::milliseconds;

    Tetris(Screen& screen);
    void operator () ();

  private:
    bool landed = true;
    bool reset_shape = true;
    uint level = 1;
    uint landed_count = 0;
    uint thresh = 15;
    static constexpr uint accel = 3;
    board_type board { };
    Shape shape = get_new_shape();
    Screen& scr;

    Shape get_new_shape();
    void clean_rows();
    void on_land();
};

#endif
