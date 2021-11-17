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


    template < std::size_t W >
    std::string make_deletion_line()
    {
        using str = std::string;
        static const str line =
            str(W, '\b')
          + str(W, ' ')
          + str(W, '\b')
          + "\x1B[A\r";
        return line;
    }
    

    template < std::size_t H, std::size_t W >
    void delete_frame()
    {
        static std::string del_line = make_deletion_line<W>();
        std::string stream;
        for (std::size_t i = 0; i < H; i++)
            stream += del_line;
        std::cout << stream << std::flush;
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
