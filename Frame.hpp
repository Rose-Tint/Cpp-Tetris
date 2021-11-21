#ifndef FRAME_HPP
#define FRAME_HPP

#include <cstddef>
#include <array>
#include <utility>
#include <mutex>
#include <cassert>


template < std::size_t H, std::size_t W >
class Frame final
{
    static_assert( (H > 0), "Frame must have a height (`H`) of at least 1" );
    static_assert( (W > 0), "Frame must have a width (`W`) of at least 1" );

  public:
    using line_type = std::array<char, W>;

    static constexpr auto Height = H;
    static constexpr auto Width = W;

    Frame(char bg_ch = ' ');
    explicit Frame(const char (&array)[H][W], char bg_ch = ' ');
    explicit Frame(const std::array<std::array<char, W>, H>& ref, char bg_ch = ' ');

    Frame(const Frame<H, W>&) = delete;
    Frame<H, W>& operator = (const Frame<H, W>&) = delete;
    Frame(Frame<H, W>&&) = delete;
    Frame<H, W>& operator = (Frame<H, W>&&) = delete;

    char at(std::size_t x, std::size_t y) const;
    const line_type& at(std::size_t y) const;
    const line_type& operator [] (std::size_t y) const;

    void clear();
    void clear(std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2);

    void set(char ch, std::size_t x, std::size_t y);
    void fill(char ch);
    void fill(char ch, std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2);

    std::string str() const;
    explicit operator std::string() const { return str(); }

    template < std::size_t _H, std::size_t _W >
    void draw(std::size_t x, std::size_t y, const Frame<_H, _W>& ref);
    template < std::size_t _H, std::size_t _W >
    void draw(std::size_t x, std::size_t y, const std::array<std::array<char, _W>, _H>& ref);
    template < template<class...> class Tmpl >
    void draw(std::size_t x, std::size_t y, const Tmpl<Tmpl<char>>& ref);
    template < class Ctr >
    void draw(std::size_t x, std::size_t y, const Ctr& ref);

  private:
    std::mutex mtx;
    char bg_ch;
    std::array<line_type, H> lines;
};



template < std::size_t H, std::size_t W >
Frame<H, W>::Frame(char bg_ch)
    : bg_ch(bg_ch)
{
    fill(bg_ch);
}


template < std::size_t H, std::size_t W >
Frame<H, W>::Frame(const char (&array)[H][W], char bg_ch)
    : bg_ch(bg_ch)
{
    using std::begin;
    using std::end;
    using std::copy;

    for (std::size_t i = 0; i < H; i++)
        copy(begin(array[i]), end(array[i]), begin(lines[i]));
}


template < std::size_t H, std::size_t W >
Frame<H, W>::Frame(const std::array<std::array<char, W>, H>& ref, char bg_ch)
    : bg_ch(bg_ch)
{
    lines = ref;
}


template < std::size_t H, std::size_t W >
char Frame<H, W>::at(std::size_t x, std::size_t y) const
{
    assert(x < W);
    assert(y < H);
    return lines.at(y).at(x);
}


template < std::size_t H, std::size_t W >
const typename Frame<H, W>::line_type& Frame<H, W>::at(std::size_t y) const
{
    assert(y < H);
    return lines.at(y);
}


template < std::size_t H, std::size_t W >
void Frame<H, W>::set(char ch, std::size_t x, std::size_t y)
{
    assert(x < W);
    assert(y < H);
    lines.at(y).at(x) = ch;
}


template < std::size_t H, std::size_t W >
const typename Frame<H, W>::line_type& Frame<H, W>::operator [] (std::size_t y) const
    { return lines.at(y); }


template < std::size_t H, std::size_t W >
void Frame<H, W>::clear()
{
    for (line_type& line : lines)
        line.fill(bg_ch);
}


template < std::size_t H, std::size_t W >
void Frame<H, W>::clear(std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2)
{
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);

    const std::size_t x_bound = std::min(x2, W);
    const std::size_t y_bound = std::min(y2, H);
    for (std::size_t y_off = 0; y1 + y_off < y_bound; y_off++)
        for (std::size_t x_off = 0; x1 + x_off < x_bound; x_off++)
            set(bg_ch, x1 + x_off, y1 + y_off);
}


template < std::size_t H, std::size_t W >
void Frame<H, W>::fill(char c)
{
    for (line_type& line : lines)
        line.fill(c);
}


template < std::size_t H, std::size_t W >
void Frame<H, W>::fill(char c, std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2)
{
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);

    const std::size_t x_bound = std::min(x2, W);
    const std::size_t y_bound = std::min(y2, H);
    for (std::size_t y_off = 0; y1 + y_off < y_bound; y_off++)
        for (std::size_t x_off = 0; x1 + x_off < x_bound; x_off++)
            set(c, x1 + x_off, y1 + y_off);
}


template < std::size_t H, std::size_t W >
std::string Frame<H, W>::str() const
{
    std::string string;
    string.reserve(H * (W + 1));
    for (const line_type& line : lines)
    {
        string.append(line.cbegin(), line.cend());
        string += '\n';
    }
    return string;
}


template < std::size_t H, std::size_t W >
template < std::size_t _H, std::size_t _W >
void Frame<H, W>::draw(std::size_t x, std::size_t y, const Frame<_H, _W>& ref)
{
    const std::size_t x_bound = std::min(x + _W, W);
    const std::size_t y_bound = std::min(y + _H, H);
    for (std::size_t y_off = 0; y + y_off < y_bound; y_off++)
        for (std::size_t x_off = 0; x + x_off < x_bound; x_off++)
            set(ref.at(x_off, y_off), x + x_off, y + y_off);
}


template < std::size_t H, std::size_t W >
template < std::size_t _H, std::size_t _W >
void Frame<H, W>::draw(std::size_t x, std::size_t y, const std::array<std::array<char, _W>, _H>& ref)
{
    const std::size_t x_bound = std::min(x + _W, W);
    const std::size_t y_bound = std::min(y + _H, H);
    for (std::size_t y_off = 0; y + y_off < y_bound; y_off++)
        for (std::size_t x_off = 0; x + x_off < x_bound; x_off++)
            at(x + x_off, y + y_off) = ref.at(y_off).at(x_off);
}


template < std::size_t H, std::size_t W >
template < template<class...> class Tmpl >
void Frame<H, W>::draw(std::size_t x, std::size_t y, const Tmpl<Tmpl<char>>& ref)
{
    const std::size_t y_bound = std::min(y + ref.size(), H);
    for (std::size_t y_off = 0; y + y_off < y_bound; y_off++)
        for (std::size_t x_off = 0; x + x_off < std::min(x + ref[y].size(), W); x_off++)
            at(x + x_off, y + y_off) = ref[y_off][x_off];
}


template < std::size_t H, std::size_t W >
template < class Ctr >
void Frame<H, W>::draw(std::size_t x, std::size_t y, const Ctr& ref)
{
    const std::size_t y_bound = std::min(y + ref.size(), H);
    for (std::size_t y_off = 0; y + y_off < y_bound; y_off++)
        for (std::size_t x_off = 0; x + x_off < std::min(x + ref[0].size(), W); x_off++)
            at(x + x_off, y + y_off) = ref[y_off][x_off];
}


#endif
