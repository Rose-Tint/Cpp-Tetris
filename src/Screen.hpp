#pragma once

#include <ncurses.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <chrono>


class Screen
{
    using size_t = std::size_t;
    using csize_t = const std::size_t;
    using cchar = const char;
    using MSecs = std::chrono::milliseconds;

  public:
    Screen(size_t height, size_t width, MSecs fps, char bg = ' ');

    Screen(const Screen&) = delete;
    Screen& operator = (const Screen&) = delete;
    Screen(Screen&&) = delete;
    Screen& operator = (Screen&&) = delete;

    ~Screen();

    const char& At(csize_t x, csize_t y) const;
    char& At(csize_t x, csize_t y);
    char* PtrAt(csize_t x, csize_t y)
        { return buffer + index(x, y); }
    cchar* PtrAt(csize_t x, csize_t y) const
        { return buffer + index(x, y); }

    void Refresh() { wrefresh(wnd); }
    void Clear();
    void Clear(csize_t x, csize_t y);
    void Clear(csize_t x1, csize_t y1, csize_t x2, csize_t y2);
    void Set(csize_t x, csize_t y, char ch);
    void Fill(cchar ch);
    void Fill(size_t x1, size_t y1, size_t x2, size_t y2, cchar ch);
    void FillLn(csize_t ln, cchar ch);
    int GetChar() { return wgetch(wnd); }
    WINDOW* Window() { return wnd; }

    size_t Height() const { return height; }
    size_t Width() const { return width; }
    char BgChar() const { return bg; }
    char BgChar(char ch) { return (bg = ch); }
    char FillChar() const { return fillch; }
    char FillChar(char ch) { return (fillch = ch); }
    void LockOutput() const;
    void LockBuffer() const;
    void UnlockOutput() const;
    void UnlockBuffer() const;

    char* begin() { return buffer; }
    char* end() { return buffer + (height * width); }
    cchar* begin() const { return buffer; }
    cchar* end() const { return buffer + (height * width); }
    cchar* cbegin() const { return buffer; }
    cchar* cend() const { return buffer + (height * width); }


  private:
    mutable std::atomic_bool do_display = true;
    mutable std::recursive_mutex omtx, bufmtx;
    const MSecs fps_lim;
    char bg = ' ', fillch = ' ';
    csize_t height, width, area;
    std::thread display_thr;
    WINDOW* const wnd;
    char* const buffer;

    void display() const;
    void print() const;
    void print(cchar ch) const;
    void print(csize_t i, cchar ch) const
        { print(y_comp(i), x_comp(i), ch); }
    void print(csize_t y, csize_t x, cchar ch) const;
    void mov(csize_t i) const { mov(y_comp(i), x_comp(i)); }
    void mov(csize_t y, csize_t x) const;
    size_t index(csize_t x, csize_t y) const
        { return (y * width) + x; }
    size_t y_comp(csize_t i) const
        { return i / width; }
    size_t x_comp(csize_t i) const
        { return i % width; }
};
