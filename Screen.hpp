#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


class Screen
{
  public:
    Screen(std::size_t height, std::size_t width, std::chrono::milliseconds fps, char bg = ' ');

    Screen(const Screen&) = delete;
    Screen& operator = (const Screen&) = delete;
    Screen(Screen&&) = delete;
    Screen& operator = (Screen&&) = delete;

    ~Screen();

    char At(std::size_t x, std::size_t y) const;
    char& At(std::size_t x, std::size_t y);

    void Clear();
    void Clear(std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2);
    void Set(std::size_t x, std::size_t y, char ch);
    void Fill(char ch);
    void Fill(std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2, char ch);

    std::size_t Height() const { return height; }
    std::size_t Width() const { return width; }
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
    const std::size_t height, width, area;
    const std::string bsp_ln;
    std::thread display_thr;
    char* buffer;

    void display() const;
    void print() const;
    void erase() const;
    std::size_t index(std::size_t x, std::size_t y) const
        { return ((y + 1) * width) - (x - 1); }
};
