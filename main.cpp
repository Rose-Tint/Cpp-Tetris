#include <iostream>
#include <thread>
#include <chrono>
#include <cstdint>
#include <cstdio>

#include "Screen.hpp"
#include "argparse.hpp"
#include "Tetris/Tetris.hpp"


constexpr std::uint_fast8_t Height = 24;
constexpr std::uint_fast8_t Width = 10;


int main(int argc, const char** argv)
{
    [[maybe_unused]]
    int _ = std::system("/bin/stty -echo");

    const ArgInfo info = parse(argc, argv);

    // make IO faster
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr); // unties cin from cout
    std::cout.tie(nullptr); // unties cout from cin
    std::nounitbuf(std::cout); // disables automatic flushing

    Screen scr(Height, Width, info.frame_limit, ' ');

    Tetris tetris(scr);
}

