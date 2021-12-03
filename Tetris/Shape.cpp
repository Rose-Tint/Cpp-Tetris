#include "../Tetris/Shape.hpp"


Shape::Shape(ShapeID id, UIntFast x_pos, UIntFast y_pos)
    : x_pos(x_pos), y_pos(y_pos), id(id)
{
    reset_matrix();
}


Shape::Shape(const Shape& other)
    : x_pos(other.x_pos), y_pos(other.y_pos),
      matrix(other.matrix), id(other.id)
{
    ;
}


Shape& Shape::operator = (const Shape& other)
{
    if (this == &other)
        return *this;
    const_cast<ShapeID&>(id) = other.id;
    matrix = other.matrix;
    y_pos = other.y_pos;
    x_pos = other.x_pos;
    return *this;
}


Shape& Shape::operator = (Shape&& other)
{
    if (this == &other)
        return *this;
    const ShapeID& id_ref = id;
    const_cast<ShapeID&>(id_ref) = other.id;
    matrix = other.matrix;
    y_pos = other.y_pos;
    x_pos = other.x_pos;
    return *this;
}


void Shape::swap(Shape& other)
{
    const ShapeID t_id = id;
    const ShapeID o_id = other.id;
    const_cast<ShapeID&>(id) = o_id;
    const_cast<ShapeID&>(other.id) = t_id;
    std::swap(matrix, other.matrix);
    std::swap(x_pos, other.x_pos);
    std::swap(y_pos, other.y_pos);
}


void Shape::justify()
{

    // adjust rows so that the top of the tetroid
    // is at the top of the matrix
    while (matrix[0].none())
    {
        // shift the first three rows up
        for (UIntFast i = 0; i < 3; i++)
            matrix[i] = matrix[i + 1];
        matrix[3].reset();
    }

    // left justify the tetroid by checking if all
    // of the bits in the first column are 0, and
    // if so, left-bitshift each row
    while (!(matrix[0][0] && matrix[1][0]
          && matrix[2][0] && matrix[3][0]))
    {
        for (UIntFast i = 0; i < 4; i++)
            matrix[i] <<= 1;
    }
}


void Shape::RotateCW()
{
    static constexpr UIntFast LAYER_C = 2;
    static constexpr UIntFast HEIGHT = 4;

    for (UIntFast layer = 0; layer < LAYER_C; layer++)
    {
        UIntFast first = layer;
        UIntFast last = HEIGHT - first;

        for (UIntFast elem = first; elem < last; elem++)
        {
            UIntFast off = elem - first;

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
    justify();
}


void Shape::RotateCC()
{
    static constexpr UIntFast LAYER_C = 2;
    static constexpr UIntFast HEIGHT = 4;

    for (UIntFast layer = 0; layer < LAYER_C; layer++)
    {
        UIntFast first = layer;
        UIntFast last = HEIGHT - first;

        for (UIntFast elem = first; elem < last; elem++)
        {
            UIntFast off = elem - first;

            bool topl = matrix[first][elem];
            bool topr = matrix[elem][last];
            bool btmr = matrix[last][last - off];
            bool btml = matrix[last - off][first];
            matrix[first][elem] = topr;
            matrix[elem][last] = btml;
            matrix[last][last - off] = btmr;
            matrix[last - off][first] = topl;
        }
    }
    justify();
}


std::array<std::pair<Shape::UIntFast, Shape::UIntFast>, 4>
    Shape::Coords() const
{
    std::array<std::pair<UIntFast, UIntFast>, 4> array { };

    UIntFast next = 0;
    for (UIntFast y = 0; y < 4; y++)
        for (UIntFast x = 0; x < 4; x++)
            if (matrix.at(y).test(x))
                array[next++] = std::make_pair(x_pos + x, y_pos + y);
    return array;
}


void Shape::Draw(Screen& scr) const
{
    const auto curr_crds = Coords();
    for (auto [x, y] : curr_crds)
        scr.Set(x, y, id_as_char());
    prev_crds = curr_crds;
}


void Shape::EraseLast(Screen& scr) const
{
    for (auto [x, y] : prev_crds)
        scr.Set(x, y, scr.BgChar());
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
      case ShapeID::L: matrix = { 0b0'0'0'1'0,
                                  0b0'1'1'1'0,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0 };
        break;
      case ShapeID::J: matrix = { 0b0'1'0'0'0,
                                  0b0'1'1'1'0,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0 };
        break;
      case ShapeID::I: matrix = { 0b0'1'1'1'1,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0,
                                  0b0'0'0'0'0 };
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


Shape::UIntFast Shape::Height() const
{
    UIntFast h = 0;
    h += matrix.at(0).any();
    h += matrix.at(1).any();
    h += matrix.at(2).any();
    h += matrix.at(3).any();
    return h;
}


Shape::UIntFast Shape::Width() const
{
    UIntFast w = 0;
    bool col_any = false;
    for (UIntFast x = 0; x < 4; x++)
    {
        col_any = false;
        for (UIntFast y = 0; y < 4 && !col_any; y++)
            col_any |= matrix.at(y).test(x);
        w += col_any;
    }
    return w;
}

