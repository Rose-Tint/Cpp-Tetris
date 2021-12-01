#include <iostream>
#include "Screen.hpp"


Screen::Screen(size_type h, size_type w, Ms fps, char bg)
    : fps_lim(fps), bg(bg), H(h), W(w),
      array(new char[h * w])
{
    display_thread = std::thread(&Screen::display, this);
    fill(bg);
}


Screen::~Screen()
{
    do_display = false;
    display_thread.join();
    delete[] array;
}


char Screen::at(size_type x, size_type y) const
    { return array[index(x, y)]; }


void Screen::set(size_type x, size_type y, char ch)
    { array[index(x, y)] = ch; }


void Screen::clear()
    { fill(bg); }


void Screen::clear(size_type x1, size_type y1, size_type x2, size_type y2)
    { fill(x1, y1, x2, y2, bg); }


void Screen::fill(char ch)
    { std::fill(begin(), end(), ch); }


void Screen::fill(size_type x1, size_type y1, size_type x2, size_type y2, char ch)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    for (size_type y = y1; y < y2; y++)
        for (size_type x = x1; x < x2; x++)
            set(x, y, ch);
}


void Screen::display()
{
    const std::string bsp_ln = std::string((W * 2), '\b') + "\x1B[A";

    const size_type area = H * W;

    const auto output = [&area](Screen& scn)
    {
        for (typename Screen::size_type i = 0; i < area; i++)
        {
            if (i && (i % scn.W == 0))
                std::cout << '\n';
            std::cout << scn.array[i] << ' ';
        }
        std::cout << std::flush;
    };

    while (do_display)
    {
        std::thread out_thr(output, std::ref(*this));
        std::this_thread::sleep_for(fps_lim);
        out_thr.join();

        for (typename Screen::size_type j = 0; j < H; j++)
            std::cout << bsp_ln;
        std::cout << std::flush;
    }
}
