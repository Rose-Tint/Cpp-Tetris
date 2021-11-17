#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>


namespace tetris
{
    template < std::size_t H, std::size_t W, char >
    class Frame;


    template < std::size_t H, std::size_t W >
    void delete_frame()
    {
        static constexpr const char* line_up = "\x1B[A\r";
        // reserve enough room for the deletion string
        // 3 x H x W because there are three strings with the same length as the line
        //     [backspaces to clear the line, spaces to override, backspaces to clear the line again],
        // plus room for "\x1B[A\r" at the end of every line
        static constexpr std::size_t res_sz = (3 * H * W) + (sizeof(line_up) * H);
        std::string init_backsps(W, '\b');
        std::string spaces(W, ' ');
        std::string end_bsp_line = init_backsps + line_up;
        std::string del_str;

        del_str.reserve(res_sz);

        for (std::size_t ln = 0; ln < H; ln++)
            del_str.append(init_backsps);
        for (std::size_t ln = 0; ln < H; ln++)
            del_str.append(spaces);
        for (std::size_t ln = 0; ln < H; ln++)
            del_str.append(end_bsp_line);

        // insert additional `line_up` to account for trailing '\n'
        std::cout << del_str << line_up << line_up << std::flush;
    }


    /**
    * streams the given message to the console, and then clears it
    *
    * @param frame  message to display
    * @param ms number of milliseconds to display for
    */
    template < std::size_t H, std::size_t W >
    void display_frame(const Frame<H, W>& frame, std::chrono::milliseconds limit)
    {
        using namespace std::chrono_literals;
        std::cout << frame.str() << std::flush;
        if (limit != 0ms)
        {
            // create new process
            std::thread th(delete_frame<H, W>);
            // sleep to limit fps. if the process above finishes before
            // the limit is up, it will delay, otherwise it will take
            // as long as it takes to delete the frame
            std::this_thread::sleep_for(limit);
            th.join();
        }
    }
}

#endif
