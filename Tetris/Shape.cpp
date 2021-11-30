#include "Tetris/Shape.hpp"


Shape::Shape(ShapeID id, size_type x)
    : x(x), y(0), id(id)
{
    reset_matrix();
}


Shape& Shape::operator = (Shape&& other)
{
    if (this == &other)
        return *this;
    const ShapeID& id_ref = id;
    const_cast<ShapeID&>(id_ref) = other.id;
    y = 0.0f;
    reset_matrix();
    return *this;
}


void Shape::rotate()
{
    static constexpr size_type layer_c = 2;
    static constexpr size_type height = 3;

    for (size_type layer = 0; layer < layer_c; layer++)
    {
        size_type first = layer;
        size_type last = height - first;

        for (size_type elem = first; elem < last; elem++)
        {
            size_type off = elem - first;

            bool topl = matrix[first][elem];
            bool topr = matrix[elem][last];
            bool btmr = matrix[last][last - off];
            bool btml = matrix[last - off][first];
            matrix[first][elem] = btml;
            matrix[elem][last] = topl;
            matrix[last][last - off] = topr;
            matrix[last - off][first] = btmr;
        }
    }
}


Shape::size_type Shape::left_most() const
{
    size_type lmost = 4;
    for (std::bitset<4> row : matrix)
    {
        if (row.any())
        {
            for (size_type i = 0; i < 4; i++)
            {
                if (row.test(i))
                {
                    if (i > lmost)
                        lmost = i;
                    break;
                }
            }
        }
        if (lmost == 0)
            break;
    }
    return lmost;
}


Shape::size_type Shape::right_most() const
{
    size_type rmost = 4;
    for (std::bitset<4> row : matrix)
    {
        if (row.any())
        {
            for (size_type i = 0; i < 4; i++)
            {
                if (row.test(i))
                {
                    if (i > rmost)
                        rmost = i;
                    break;
                }
            }
        }
        if (rmost == 4)
            break;
    }
    return rmost;
}


std::array<std::pair<Shape::size_type, Shape::size_type>, 4>
    Shape::coords() const
{
    std::array<std::pair<size_type, size_type>, 4> array { };

    size_type to_set = 0;
    for (size_type i = 0; i < 4; i++)
        for (size_type j = 0; j < 4; j++)
            if (matrix.at(i).test(j))
                array[to_set++] = std::make_pair(j, i);
    return array;
}


void Shape::draw(Screen& scn, bool reset)
{
    static std::array<std::pair<size_type, size_type>, 4>
        prev_crds = {{{0,0},{0,0},{0,0},{0,0}}};
    static size_type prev_x = 0, prev_y = 0;

    if (reset)
    {
        prev_crds = {{{0,0},{0,0},{0,0},{0,0}}};
        prev_x = 0;
        prev_y = 0;
    }
    else for (std::pair<size_type, size_type> crd : prev_crds)
        scn.set(prev_x + crd.first, prev_y + crd.second, scn.bg_char());
    const auto curr_crds = coords();
    for (std::pair<size_type, size_type> crd : curr_crds)
        scn.set(x + crd.first, y + crd.second, id_as_char());
    prev_crds = curr_crds;
    prev_x = x;
    prev_y = y;
}


char Shape::id_as_char() const
{
    switch (id)
    {
      case ShapeID::T: return '#';
      case ShapeID::L: return '?';
      case ShapeID::J: return '%';
      case ShapeID::I: return '$';
      case ShapeID::O: return '&';
      case ShapeID::S: return '0';
      case ShapeID::Z: return '@';
      default:
        throw std::invalid_argument(
            "ShapeID argument given to Shape is not valid"
        );
    }
}


void Shape::reset_matrix()
{
    switch (id)
    {
      case ShapeID::T: matrix = { 0b0'0'1'0'0,
                                  0b0'1'1'1'0,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0 };
        break;
      case ShapeID::L: matrix = { 0b0'1'0'0'0,
                                  0b0'1'0'0'0,
                                  0b0'1'1'0'0,
                                  0b0'0'0'0'0 };
        break;
      case ShapeID::J: matrix = { 0b0'0'1'0'0,
                                  0b0'0'1'0'0,
                                  0b0'1'1'0'0,
                                  0b0'0'0'0'0 };
        break;
      case ShapeID::I: matrix = { 0b0'1'0'0'0,
                                  0b0'1'0'0'0,
                                  0b0'1'0'0'0,
                                  0b0'1'0'0'0 };
        break;
      case ShapeID::O: matrix = { 0b0'1'1'0'0,
                                  0b0'1'1'0'0,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0 };
        break;
      case ShapeID::S: matrix = { 0b0'0'1'1'0,
                                  0b0'1'1'0'0,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0 };
        break;
      case ShapeID::Z: matrix = { 0b0'1'1'0'0,
                                  0b0'0'1'1'0,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0 };
        break;
      default:
        throw std::invalid_argument(
            "ShapeID argument given to Shape is not valid"
            "\n\tchar(id): '"
            + std::string(1, id_as_char()) + '\''
        );
    }
}

