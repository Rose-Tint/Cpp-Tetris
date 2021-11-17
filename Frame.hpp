#ifndef FRAME_HPP
#define FRAME_HPP

#include <cstddef>
#include <array>
#include <utility>


namespace tetris
{
    template < std::size_t H, std::size_t W, char Fill_Ch = ' ' >
    class Frame final
    {
        static_assert((H > 0), "Frame must have a height (`H`) of at least 1");
        static_assert((W > 0), "Frame must have a width (`W`) of at least 1");

      public:
        using line_type = std::array<char, W>;
        using coord_type = std::pair<std::size_t, std::size_t>;

        static constexpr auto HEIGHT = H;
        static constexpr auto WIDTH = W;

        Frame() { fill(Fill_Ch); }

        char& operator [] (coord_type coords);
        const char& operator [] (coord_type coords) const;
        std::string operator [] (std::size_t y) const;
        // TODO: subscript operator that returns a proxy to a line
        void fill(char c);
        void downshift(std::string new_top_line);
        void upshift(std::string new_btm_line);
        void rotate();
        std::string str() const;

        operator std::string() const { return str(); }

      private:
        std::array<line_type, H> lines;
    };


    template < std::size_t H, std::size_t W, char Fill_Ch >
    void tetris::Frame<H, W, Fill_Ch>::fill(char c)
    {
        for (line_type& line : lines)
            line.fill(c);
    }


    template < std::size_t H, std::size_t W, char Fill_Ch >
    void tetris::Frame<H, W, Fill_Ch>::downshift(std::string topl)
    {
        line_type& orig_topl = lines[0];
        topl.resize(W, Fill_Ch);

        for (std::size_t i = 1; i < H; i++)
            lines[i] = lines[i - 1];

        for (std::size_t i = 0; i < W; i++)
            orig_topl[i] = topl[i];
    }


    template < std::size_t H, std::size_t W, char Fill_Ch >
    void tetris::Frame<H, W, Fill_Ch>::rotate()
    {
        lines[0] = lines[H];
        for (std::size_t i = 1; i < H; i++)
            lines[i] = lines[i - 1];
    }


    template < std::size_t H, std::size_t W, char Fill_Ch >
    std::string tetris::Frame<H, W, Fill_Ch>::str() const
    {
        std::string string;
        for (const line_type& line : lines)
        {
            string.append(line.cbegin(), line.cend());
            string += '\n';
        }
        return string;
    }
}

#endif
