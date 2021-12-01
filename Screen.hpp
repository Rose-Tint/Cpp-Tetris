#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <fstream>
#include <array>
#include <thread>
#include <chrono>


class Screen
{
    using Ms = std::chrono::milliseconds;

  public:
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

    char* begin() { return array; }
    char* end() { return array + (H * W); }
    const char* begin() const { return array; }
    const char* end() const { return array + (H * W); }
    const char* cbegin() const { return array; }
    const char* cend() const { return array + (H * W); }

  private:
    mutable bool do_display = true;
    void display();
    const Ms fps_lim;
    char bg;
    const size_type H, W;
    std::thread display_thread;
    std::ofstream file;
    char* array;

    size_type index(size_type x, size_type y) const
        { return ((y + 1) * W) - (x - 1); }
};

#endif
