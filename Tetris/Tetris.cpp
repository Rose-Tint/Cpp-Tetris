#include "Tetris/Tetris.hpp"

#include <thread>
#include <chrono>
#include <random>


Tetris::Tetris(Screen& screen, millis spd, millis acc)
    : acc(acc),
      shape(get_new_shape()),
      screen(screen)
{
    board = { };
    speed = spd;
}


void Tetris::operator () ()
{
    using namespace std::chrono_literals;

    static const auto rot = [](Shape& shape)
    {
        while (true)
        {
            std::this_thread::sleep_for(millis(500));
            shape.rotate();
        }
    };

    std::thread rot_thr(rot, std::ref(shape));
    bool reset = false;

    while (board[0].none())
    {
        if (!landed)
        {
            shape.descend();
            for (auto crd : shape.coords())
            {
                if (board.at(crd.second).test(crd.first))
                    landed = true;
            }
        }
        else
        {
            reset = true;
            landed = false;
            // detects full rows, starting at the bottom,
            // and shifts the board down when one is found,
            // deleting the full rows
            for (int i = 19; i > 0; i--)
            {
                if (board[i].all())
                {
                    for (int j = i; j > 0; j--)
                        board[j] = board[j - 1];
                    board[0].reset();
                }
            }
            shape = get_new_shape();
        }

        shape.draw(screen, reset);
        reset = false;

        std::this_thread::sleep_for(speed);
        speed -= acc;
    }
    rot_thr.join();
}


Shape Tetris::get_new_shape()
{
    std::random_device rd { };
    std::mt19937 engine(rd());
    std::uniform_int_distribution pos_gen(0, 10);
    std::uniform_int_distribution id_gen(0, 6);
    size_type x_pos = pos_gen(engine);
    ShapeID id = ShapeID(id_gen(engine));
    return Shape(id, x_pos);
}
