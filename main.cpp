#include <iostream>
#include <thread>
#include <chrono>
#include <ncurses.h>

#include "Screen.hpp"
#include "argparse.hpp"
#include "Tetris/Tetris.hpp"


constexpr int Height = 24;
constexpr int Width = 10;


void run(Screen& screen)
{
    using namespace std::chrono_literals;
    Tetris tetris(screen);
    tetris();
}


int main(int argc, const char** argv)
{
    const ArgInfo info = parse(argc, argv);

    // make IO faster
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr); // unties cin from cout
    std::cout.tie(nullptr); // unties cout from cin
    std::nounitbuf(std::cout); // disables automatic flushing

    Screen screen(Height, Width, info.frame_limit, ' ');

    std::thread main_thr(run, std::ref(screen));
    main_thr.join();
}

