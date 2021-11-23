#pragma once
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "argparse.hpp"


template < std::size_t H, std::size_t W >
class Frame;


template < std::size_t H, std::size_t W >
std::string make_deletion_frame()
{
    using std::string;
    static constexpr const char lineUpEscCh[] = "\x1B[A";
    static constexpr std::size_t lWidth = W + sizeof(lineUpEscCh);
    static const string bspLine = string(W, '\b') + lineUpEscCh;

    string del_str;
    del_str.reserve(H * lWidth);
    for (std::size_t ln = 0; ln < H; ln++)
        del_str.append(bspLine);

    return del_str;
}


/**
* streams the given message to the console, and then clears it
*
* @param frame  message to display
* @param ms number of milliseconds to display for
* @param fn  function to execute while displaying the frame
* @param args  args to feed the function.
*/
template < std::size_t H, std::size_t W >
void display_frame(const Frame<H, W>& frame, const ArgInfo& info)
{
    using namespace std::chrono_literals;
    static auto delete_frame = []()
        {
            static const std::string del_str = make_deletion_frame<H, W>();
            std::cout << del_str << std::flush;
        };
    std::cout << frame.str() << std::flush;
    if (info.frame_limit > 0ms)
    {
        std::thread del_th(delete_frame);
        // sleep to limit fps. if the processes above finishe before
        // the limit is up, it will delay, otherwise it will take
        // as long as it takes to delete the frame
        std::this_thread::sleep_for(info.frame_limit);
        del_th.join();
    }
}


template < std::size_t H, std::size_t W >
void run_display(const Frame<H, W>& frame, const ArgInfo& info)
{
    while (1)
        display_frame(frame, info);
    
}

#endif
