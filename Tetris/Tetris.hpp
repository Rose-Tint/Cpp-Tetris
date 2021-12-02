#pragma once

#include <bitset>
#include <array>
#include <cstdint>
#include <thread>
#include <atomic>

#include "../Tetris/Shape.hpp"


class Screen;


enum struct Key : int
{
          Up,
    Left,Down,Right,
    Q,W,E,R,
    A,S,D,F,
    Space,
    Other
};


class Tetris
{
  public:
    static constexpr std::uint_fast8_t H = 24;
    static constexpr std::uint_fast8_t W = 10;

    using board_type = std::array<std::bitset<W>, H>;
    using uint = std::uint_fast16_t;

    Tetris(Screen& screen);

    ~Tetris();

    void soft_drop();
    void hard_drop();
    void go_right();
    void go_left();
    void rotate_cw();
    void rotate_cc();
    void hold();

  private:
    bool landed = true, reset_shape = true;
    std::atomic<bool> run = true;
    std::atomic<uint> score = 0, level = 1;
    std::atomic<uint> speed = 1, thresh = 15, acc = 3;
    std::atomic<uint> landed_count = 0;
    board_type board { };
    Shape shape = rand_shape(), queued = rand_shape();
    Screen& scr;
    std::thread launch_thr, input_thr;

    int getch() const;
    void get_input();
    void input_loop() { while (run) get_input(); }
    void launch();
    Shape rand_shape();
    void clean_rows();
    void on_land();
};
