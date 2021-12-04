#include <cstdio>

#include "Screen.hpp"


Screen::Screen(size_t h, size_t w, MSecs fps, char bg)
    : fps_lim(fps), bg(bg), fillch(' '),
      height(h), width(w), area(h * w),
      display_thr(&Screen::display, this),
      wnd(newwin(2 * height, 2 * width, 0, 0)),
      buffer(new char[h * w])
{
    wrefresh(wnd);
    Clear();
}


Screen::~Screen()
{
    do_display = false;
    std::scoped_lock lk(omtx, bufmtx);
    display_thr.join();
    wclear(wnd);
    wrefresh(wnd);
    delwin(wnd);
    delete[] buffer;
}


void Screen::Set(csize_t x, csize_t y, cchar ch)
{
    std::scoped_lock lk(bufmtx);
    buffer[index(x, y)] = ch;
}


void Screen::Clear()
{
    std::scoped_lock lk(bufmtx);
    std::fill(begin(), end(), bg);
}


void Screen::Clear(csize_t x, csize_t y)
    { Set(x, y, bg); }


void Screen::Clear(csize_t x1, csize_t y1, csize_t x2, csize_t y2)
    { Fill(x1, y1, x2, y2, bg); }


void Screen::Fill(const char ch)
{
    std::scoped_lock lk(bufmtx);
    std::fill(begin(), end(), ch);
}


void Screen::Fill(size_t x1, size_t y1, size_t x2, size_t y2, const char ch)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    std::scoped_lock lk(bufmtx);
    for (size_t y = y1; y < y2; y++)
        for (size_t x = x1; x < x2; x++)
            Set(x, y, ch);
}


void Screen::FillLn(csize_t ln, const char ch)
{
    std::scoped_lock lk(bufmtx);
    std::fill(PtrAt(0, ln), PtrAt(width, ln), ch);
}


void Screen::display() const
{
    csize_t area = height * width;
    while (do_display)
    {
        std::thread sleep_thr([this]{
            std::this_thread::sleep_for(fps_lim);
        });
        std::scoped_lock lk(omtx, bufmtx);
        mov(height, 0);
        for (size_t i = 0; i < area; i++)
        {
            if (i && (x_comp(i) == 0))
                mov(y_comp(i) + 1, 0);
            print(buffer[i]);
        }
        mov(height, 0);
        wrefresh(wnd);
        omtx.unlock();
        sleep_thr.join();
    }
}


void Screen::print(const char ch) const
{
    std::scoped_lock lk(omtx);
    waddch(wnd, ch);
    waddch(wnd, fillch);
}


void Screen::print(csize_t y, csize_t x, cchar ch) const
{
    std::scoped_lock lk(omtx);
    mvwaddch(wnd, y, x, ch);
}


void Screen::mov(csize_t y, csize_t x) const
{
    std::scoped_lock lk(omtx);
    wmove(wnd, y, x);
}

