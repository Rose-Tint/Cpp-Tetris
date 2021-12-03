#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>


class Screen
{
    using size_t = std::size_t;
    using csize_t = const std::size_t;
    using milliseconds = std::chrono::milliseconds;

  public:
    Screen(size_t height, size_t width, milliseconds fps, char bg = ' ');

    Screen(const Screen&) = delete;
    Screen& operator = (const Screen&) = delete;
    Screen(Screen&&) = delete;
    Screen& operator = (Screen&&) = delete;

    ~Screen();

    const char& At(csize_t x, csize_t y) const;
    char& At(csize_t x, csize_t y);
    char* PtrAt(csize_t x, csize_t y)
        { return buffer + index(x, y); }
    const char* PtrAt(csize_t x, csize_t y) const
        { return buffer + index(x, y); }

    void Clear();
    char Clear(csize_t x, csize_t y);
    void Clear(csize_t x1, csize_t y1, csize_t x2, csize_t y2);
    char Set(csize_t x, csize_t y, char ch);
    void Fill(const char ch);
    void Fill(size_t x1, size_t y1, size_t x2, size_t y2, const char ch);
    void FillLn(csize_t ln, const char ch);

    size_t Height() const { return height; }
    size_t Width() const { return width; }
    char BgChar() const { return bg; }
    char BgChar(char ch) { return (bg = ch); }
    std::mutex& Mutex() const { return io_mtx; }
    const bool& ioReady() const { return outputting; }
    std::condition_variable& IoCondition() const { return io_cv; }
    void Lock() const { io_mtx.lock(); }
    void Unlock() const { io_mtx.unlock(); }

    char* begin() { return buffer; }
    char* end() { return buffer + (height * width); }
    const char* begin() const { return buffer; }
    const char* end() const { return buffer + (height * width); }
    const char* cbegin() const { return buffer; }
    const char* cend() const { return buffer + (height * width); }


  private:
    mutable bool outputting = true, do_display = true;
    mutable std::condition_variable io_cv;
    mutable std::mutex io_mtx;
    const std::chrono::milliseconds fps_lim;
    char bg;
    const size_t height, width, area;
    const std::string bsp_ln, breakln;
    std::thread display_thr;
    char* buffer;

    void display() const;
    void print() const;
    void erase() const;
    size_t index(csize_t x, csize_t y) const
        { return ((y + 1) * width) - x; }
};
