#include <cstdio>
#include "Screen.hpp"


Screen::Screen(size_type h, size_type w, Ms fps, char bg)
    : fps_lim(fps), bg(bg), H(h), W(w), area(H * W),
      bsp_ln(std::string((W * 2), '\b') + "\x1B[A"),
      display_thread(&Screen::display, this),
      buffer(new char[h * w])
{
    std::fill(begin(), end(), bg);
}


Screen::~Screen()
{
    do_display = false;
    display_thread.join();
    delete[] buffer;
}

char Screen::at(size_type x, size_type y) const
    { return buffer[index(x, y)]; }

void Screen::set(size_type x, size_type y, char ch)
    { buffer[index(x, y)] = ch; }

void Screen::clear()
    { std::fill(begin(), end(), bg); }

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


void Screen::display() const
{
    while (do_display)
    {
        print();
        std::this_thread::sleep_for(fps_lim);
        erase();
    }
}


void Screen::print() const
{
    lock_guard_t lock(io_mtx);

    const size_t area = H * W;
    for (size_type i = 0; i < area; i++)
    {
        if (i && (i % W == 0))
            std::putchar('\n');
        std::printf("%c ", buffer[i]);
    }
    std::fflush(stdout);

    io_cv.notify_one();
}


void Screen::erase() const
{
    lock_guard_t lock(io_mtx);

    for (size_type i = 0; i < H - 1; i++)
        printf("%s", bsp_ln.c_str());
    std::fflush(stdout);

    io_cv.notify_one();
}
