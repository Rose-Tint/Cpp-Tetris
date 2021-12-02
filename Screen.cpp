#include <cstdio>
#include "Screen.hpp"


using std::size_t;


Screen::Screen(size_t h, size_t w, std::chrono::milliseconds fps, char bg)
    : fps_lim(fps), bg(bg), height(h), width(w), area(height * width),
      bsp_ln(std::string((width * 2), '\b') + "\x1B[A"),
      display_thr(&Screen::display, this),
      buffer(new char[h * w])
{
    std::fill(begin(), end(), bg);
}


Screen::~Screen()
{
    do_display = false;
    display_thr.join();
    delete[] buffer;
}

char Screen::At(size_t x, size_t y) const
    { return buffer[index(x, y)]; }

void Screen::Set(size_t x, size_t y, char ch)
    { buffer[index(x, y)] = ch; }

void Screen::Clear()
    { std::fill(begin(), end(), bg); }

void Screen::Clear(size_t x1, size_t y1, size_t x2, size_t y2)
    { Fill(x1, y1, x2, y2, bg); }

void Screen::Fill(char ch)
    { std::fill(begin(), end(), ch); }


void Screen::Fill(size_t x1, size_t y1, size_t x2, size_t y2, char ch)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    for (size_t y = y1; y < y2; y++)
        for (size_t x = x1; x < x2; x++)
            Set(x, y, ch);
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
    std::lock_guard<std::mutex> lock(io_mtx);

    const size_t area = height * width;
    for (size_t i = 0; i < area; i++)
    {
        if (i && (i % width == 0))
            std::putchar('\n');
        std::printf("%c ", buffer[i]);
    }
    std::fflush(stdout);

    io_cv.notify_one();
}


void Screen::erase() const
{
    std::lock_guard<std::mutex> lock(io_mtx);

    for (size_t i = 0; i < height - 1; i++)
        printf("%s", bsp_ln.c_str());
    std::fflush(stdout);

    io_cv.notify_one();
}
