#include <random>

#include "../Tetris/Tetris.hpp"

#include <thread>
#include <chrono>


Tetris::Tetris(Screen& scr)
    : shape(get_new_shape()), scr(scr)
{
    board = { };
    scr.bg_char('.');
    scr.clear();
}


// BUG: game does not stop when top is reached.
// TODO: user input.
// BUG: after reaching the top, pieces overlap,
//     inching downwards, and after a while, when
//     the bottom is reached, throws out_of_range
void Tetris::operator () ()
{
    while (board[0].none())
    {
        if (!landed)
        {
            shape.descend(level);
            auto coords = shape.coords();
            for (auto [x, y] : coords)
            {
                if (y >= H - 1 || board.at(y + 1).test(x))
                    landed = true;
            }
            if (landed)
            {
                for (auto [x, y] : coords)
                    board.at(y).set(x);
            }
        }
        else on_land();

        shape.draw(scr, reset_shape);
        if (reset_shape)
            reset_shape = false;

        std::this_thread::sleep_for(millis(10));
    }
}


void Tetris::on_land()
{
    clean_rows();
    shape = get_new_shape();
    reset_shape = true;
    landed = false;
    landed_count++;

    if (landed_count % thresh == 0)
    {
        thresh -= (thresh % accel == 0);
        level += 1;
    }
}


/**
* detects full rows, starting at the bottom,
* and shifts the board down when one is found,
* deleting the full rows
*/
void Tetris::clean_rows()
{
    for (int i = H - 1; i > 0; i--)
    {
        if (board[i].all())
        {
            for (int j = i; j > 0; j--)
                board[j] = board[j - 1];
            board[0].reset();
        }
    }
}


Shape Tetris::get_new_shape()
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<char> id_gen(0, 6);
    ShapeID id = ShapeID(id_gen(engine));
    return Shape(id, 4, 0);
}
