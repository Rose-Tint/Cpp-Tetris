#include <iostream>
#include <cmath>
#include <thread>
#include <algorithm>
#include <unordered_map>

#include "Frame.hpp"
#include "display.hpp"
#include "argparse.hpp"
#include "Tetris/Tetris.hpp"


constexpr float ResScale = 2;
constexpr std::size_t Height = 20; //9 * ResScale;
// double the width to account for the extra spacing between lines in the terminal
constexpr std::size_t Width = 10; //(16 * 2) * ResScale;


using frame_t = Frame<Height, Width>;


template < std::size_t H, std::size_t W >
void main_loop(Frame<H, W>& frame)//, const ArgInfo&)
{
    Tetris tetris;
    tetris(frame);
}


int main(int argc, const char** argv)
{
    using std::ref;

    const ArgInfo info = parse(argc, argv);

    // make IO faster
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr); // unties cin from cout
    std::cout.tie(nullptr); // unties cout from cin
    std::nounitbuf(std::cout); // disables automatic flushing

    frame_t frame(' ');

    // seperate graphics from logic
    std::thread display_thr(run_display<Height, Width>, ref(frame), ref(info));
    std::thread main_loop_thr(main_loop<Height, Width>, ref(frame));//, ref(info));

    display_thr.join();
    main_loop_thr.join();
}

