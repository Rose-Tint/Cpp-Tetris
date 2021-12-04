#include <iostream>
#include <cstdint>
#include <ncurses.h>
#include <unistd.h>

#include "Screen.hpp"
#include "argparse.hpp"
#include "Tetris/Tetris.hpp"


// queued piece preview + delim line + play field
constexpr std::uint_fast8_t Height = 2 + 1 + 20;
constexpr std::uint_fast8_t Width = 10;


int main(int argc, const char** argv)
{
    SCREEN* curscr = nullptr;
    curscr = newterm(nullptr, stdout, stdin);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    const ArgInfo info = parse(argc, argv);

    // make IO faster
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr); // unties cin from cout
    std::cout.tie(nullptr); // unties cout from cin
    std::nounitbuf(std::cout); // disables automatic flushing

    Screen scr(Height, Width, info.frame_limit, ' ');

    Tetris tetris(scr);

    endwin();
    delscreen(curscr);
}

