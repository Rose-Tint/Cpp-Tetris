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
    Q,WIDTH,E,R,
    A,S,D,F,
    Space,
    Other
};


class Tetris
{
    static constexpr std::uint_fast8_t HEIGHT = 24;
    static constexpr std::uint_fast8_t WIDTH = 10;

  public:
    using Board = std::array<std::bitset<WIDTH>, HEIGHT>;
    using UIntFast = std::uint_fast8_t;
    using UIntLeast = std::uint_least8_t;

    Tetris(Screen& screen);

    ~Tetris();

    void HardDrop();
    void MoveRight();
    void MoveLeft();
    void RotateCC();
    void RotateCW();
    void Hold();

    [[deprecated("Not Yet Supported")]]
    void SoftDrop();

  private:
    UIntLeast landed : 1;
    UIntLeast reset_shape : 1;
    UIntLeast can_hold : 1;
    std::atomic<bool> run = true;
    UIntFast score = 0, level = 1;
    // UIntFast speed = 1, thresh = 15, acc = 3;
    // UIntFast landed_count = 0;
    Board board { };
    Shape shape = rand_shape(), queued = rand_shape();
    Screen& scr;
    std::thread launch_thr, input_thr;

    int getch() const;
    void get_input();
    void input_loop() { while (run) get_input(); }
    void launch();
    void contain_shape();
    Shape rand_shape();
    void clean_rows();
    void on_land();
};
