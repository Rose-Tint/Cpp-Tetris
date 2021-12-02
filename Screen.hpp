#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


class Screen
{
    using Ms = std::chrono::milliseconds;

  public:
    using scoped_lock_t = std::scoped_lock<std::mutex>;
    using lock_guard_t = std::lock_guard<std::mutex>;
    using unique_lock_t = std::unique_lock<std::mutex>;
    using size_type = std::size_t;

    Screen(size_type H, size_type W, Ms fps, char bg = ' ');

    Screen(const Screen&) = delete;
    Screen& operator = (const Screen&) = delete;
    Screen(Screen&&) = delete;
    Screen& operator = (Screen&&) = delete;

    ~Screen();

    char at(size_type x, size_type y) const;
    char& at(size_type x, size_type y);

    void clear();
    void clear(size_type x1, size_type y1, size_type x2, size_type y2);
    void set(size_type x, size_type y, char ch);
    void fill(char ch);
    void fill(size_type x1, size_type y1, size_type x2, size_type y2, char ch);

    size_type height() const { return H; }
    size_type width() const { return W; }
    char bg_char() const { return bg; }
    char bg_char(char ch) { return (bg = ch); }
    std::mutex& get_mutex() const { return io_mtx; }
    const bool& io_ready() const { return outputting; }
    std::condition_variable& io_cvar() const { return io_cv; }
    void lock() const { io_mtx.lock(); }
    void unlock() const { io_mtx.unlock(); }

    char* begin() { return buffer; }
    char* end() { return buffer + (H * W); }
    const char* begin() const { return buffer; }
    const char* end() const { return buffer + (H * W); }
    const char* cbegin() const { return buffer; }
    const char* cend() const { return buffer + (H * W); }


  private:
    mutable bool outputting = true, do_display = true;
    mutable std::condition_variable io_cv;
    mutable std::mutex io_mtx;
    const Ms fps_lim;
    char bg;
    const size_type H, W, area;
    const std::string bsp_ln;
    std::thread display_thread;
    char* buffer;

    void display() const;
    void print() const;
    void erase() const;
    size_type index(size_type x, size_type y) const
        { return ((y + 1) * W) - (x - 1); }
};
