#include <cstdio>
#include "Screen.hpp"


Screen::Screen(size_t h, size_t w, std::chrono::milliseconds fps, char bg)
    : fps_lim(fps), bg(bg), height(h), width(w), area(height * width),
      bsp_ln(std::string((width + 2) * 2, '\b') + "\x1B[A"),
      breakln("+" + std::string(width * 2 + 1, '-') + "+"),
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

const char& Screen::At(csize_t x, csize_t y) const
    { return buffer[index(x, y)]; }


char Screen::Set(csize_t x, csize_t y, char ch)
{
    std::swap(buffer[index(x, y)], ch);
    return ch;
}


void Screen::Clear()
    { std::fill(begin(), end(), bg); }


char Screen::Clear(csize_t x, csize_t y)
    { return Set(x, y, bg); }


void Screen::Clear(csize_t x1, csize_t y1, csize_t x2, csize_t y2)
    { Fill(x1, y1, x2, y2, bg); }


void Screen::Fill(const char ch)
    { std::fill(begin(), end(), ch); }


void Screen::Fill(size_t x1, size_t y1, size_t x2, size_t y2, const char ch)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    for (size_t y = y1; y < y2; y++)
        for (size_t x = x1; x < x2; x++)
            Set(x, y, ch);
}


void Screen::FillLn(csize_t ln, const char ch)
{
    std::fill(PtrAt(0, ln - 1), PtrAt(width, ln + 1), ch);
}


void Screen::display() const
{
    while (do_display)
    {
        std::thread sleep_thr([this]{std::this_thread::sleep_for(fps_lim);});
        print();
        sleep_thr.join();
        erase();
    }
}


void Screen::print() const
{
    std::lock_guard<std::mutex> lock(io_mtx);

    const char* breakline = breakln.c_str();

    csize_t area = height * width;
    std::puts(breakline);     // puts appends \n
    std::fputs("| ", stdout); // fputs does not
    for (size_t i = 0; i < area; i++)
    {
        if (i && (i % width == 0))
            std::fputs("|\n| ", stdout);
        std::printf("%c ", buffer[i]);
    }
    std::puts("|");               // puts appends \n
    std::fputs(breakline, stdout); // fputs does not
    std::fflush(stdout);

    io_cv.notify_one();
}


void Screen::erase() const
{
    std::lock_guard<std::mutex> lock(io_mtx);

    const char* bspline = bsp_ln.c_str();

    for (size_t i = 0; i < height + 1; i++)
        printf("%s", bspline);
    std::fflush(stdout);

    io_cv.notify_one();
}
