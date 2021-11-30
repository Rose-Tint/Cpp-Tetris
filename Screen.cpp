#include <iostream>
#include "Screen.hpp"


Screen::Screen(size_type h, size_type w, Ms fps, char bg)
    : fps_lim(fps), bg(bg), _height(h), _width(w),
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
    if (x1 > x2)     std::swap(x1, x2);
    if (y1 > y2)     std::swap(y1, y2);
    if (x2 > _width)  x2 = _width;
    if (y2 > _height) y2 = _height;

    for (size_type y_pos = y1; y_pos < y2; y_pos++)
        std::fill(array + index(y_pos, x1), array + index(y_pos, x2), ch);
}


void Screen::display()
{
    const std::string bsp_ln = std::string((_width * 2) + 1, '\b') + "\x1B[A";
    const std::string del_str = ([&bsp_ln](size_type w) -> std::string
    {
        std::string del_str;
        for (int i = 0; i < w; i++)
            del_str += bsp_ln;
        return del_str;
    })(_width);

    const auto output = [](Screen& scn)
    {
        for (typename Screen::size_type i = 0; i < scn._height * scn._width; i++)
        {
            if (i && (i % scn._width == 0))
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

        for (typename Screen::size_type j = 0; j < _height; j++)
            std::cout << bsp_ln;
        std::cout << std::flush;
    }
}
