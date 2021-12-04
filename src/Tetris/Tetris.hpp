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
    Q,W,E,
    A,S,D,F,
    Space,
    Other
};


class Tetris
{
    static constexpr std::uint_fast8_t HEIGHT = 2 + 1 + 20;
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
    bool landed = true;
    bool erase_shape = false;
    bool can_hold = true;
    std::atomic<bool> run = true;
    UIntFast score = 0, level = 1;
    // UIntFast speed = 1, thresh = 15, acc = 3;
    // UIntFast landed_count = 0;
    Board board { };
    Shape shape = rand_shape();
    Shape queued = rand_shape();
    Screen& scr;
    std::thread launch_thr, input_thr;

    void get_input();
    void display() const;
    void launch();
    void contain_shape();
    Shape rand_shape();
    void clean_rows();
    void on_land();
};
