#include <random>
#include <future>
#include <thread>
#include <chrono>
#include <map>
#include <cstdlib>
#include <cstdio>

#include "../Tetris/Tetris.hpp"
#include "../Screen.hpp"


using millis = std::chrono::milliseconds;


Tetris::Tetris(Screen& scr)
    : shape(rand_shape()), scr(scr)
{
    landed = reset_shape = can_hold = true;
    board = { };
    scr.BgChar('.');
    scr.Clear();
    launch_thr = std::thread(&Tetris::launch, this);
    input_thr = std::thread(&Tetris::input_loop, this);
}


// BUG: game does not stop when top is reached.
// BUG: after reaching the top, pieces overlap,
//     inching downwards, and after a while, when
//     the bottom is reached, throws out_of_range
void Tetris::launch()
{
    while (board.front().none())
    {
        if (!landed)
        {
            shape.Descend(level);
            auto Coords = shape.Coords();
            for (auto [x, y] : Coords)
                if (y >= HEIGHT - 1 || board.at(y + 1).test(x))
                    landed = true;
            if (landed)
                for (auto [x, y] : Coords)
                    board.at(y).set(x);
        }
        else on_land();

        shape.Draw(scr, reset_shape);
        if (reset_shape)
            reset_shape = false;

        std::this_thread::sleep_for(millis(250));
    }
    run = false;
}


void Tetris::HardDrop()
{
    // get lowest active x positions, and the
    //     lowest active y position
    // get highest conflicting landed block
    // teleport to one above

    std::map<uint, uint> lows; // lowest y in each col
    uint high_y = 0;
    for (auto [x, y] : shape.Coords())
    {
        if (lows.count(x) == 0)
            lows.insert(std::make_pair(x, y));
        else if (lows.at(x) < y)
            lows.at(x) = y;
    }

    for (auto [shape_x, shape_y] : lows)
    {
        // NOTE: i suspect some weird behavior when
        //           shape_y starts at 0

        // start y at shape_y because shape_y (hopefully)
        // cannot be lower than the highest block in
        // that column because of colision
        for (uint y = shape_y; y > 0; y--)
            if (y < high_y && board.at(y).test(shape_x))
                high_y = y;
    }

    shape.SetY(high_y);
    landed = true;
}


void Tetris::SoftDrop()
{
    // uint old_speed = speed;
    // if (!landed)
    //     speed = speed * 2;
    // while (!landed);
    // speed = old_speed;
}


void Tetris::MoveLeft()
{
    if (shape.X() > 0)
        shape.MoveLeft();
}


void Tetris::MoveRight()
{
    if (shape.X() + shape.Width() < WIDTH)
        shape.MoveRight();
}


void Tetris::contain_shape()
{
    const UIntFast shape_w = shape.Width();
    if (shape.X() + shape_w >= WIDTH)
        shape.MoveLeft(shape.X() + shape_w - WIDTH - 1);
    if (shape.X() - shape_w < 0)
        shape.MoveLeft(shape_w - shape.X());
}


void Tetris::RotateCW()
{
    shape.RotateCW();
    contain_shape();
}


void Tetris::RotateCC()
{
    shape.RotateCC();
    contain_shape();
}


void Tetris::Hold()
{
    if (!can_hold)
        return;
    Shape temp = queued;
    queued = shape;
    shape = temp;
    can_hold = false;
}


void Tetris::on_land()
{
    clean_rows();
    shape = rand_shape();
    reset_shape = true;
    landed = false;
    can_hold = true;
}


/**
* detects full rows, starting at the bottom,
* and shifts the board down when one is found,
* deleting the full rows
*/
void Tetris::clean_rows()
{
    for (int i = HEIGHT - 1; i > 0; i--)
    {
        if (board[i].all())
        {
            for (int j = i; j > 0; j--)
                board[j] = board[j - 1];
            board[0].reset();
        }
    }
}


Shape Tetris::rand_shape()
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<char> id_gen(0, 6);
    ShapeID id = ShapeID(id_gen(engine));
    return Shape(id, 4, 0);
}


int Tetris::getch() const
{
    using namespace std::chrono_literals;
    using fs = std::future_status;

    [[maybe_unused]]
    int sys_stat = 0;
    bool input_read = false;
    std::condition_variable& cv = scr.IoCondition();
    std::future<int> future;

    while (!input_read)
    {
        std::unique_lock lock(scr.Mutex());
        cv.wait(lock);

        sys_stat = std::system("/bin/stty raw");
        future = std::async(std::getchar);
        input_read = future.wait_for(1ms) == fs::ready;
        sys_stat = std::system("/bin/stty cooked");

        lock.unlock();
        cv.notify_one();
        std::this_thread::sleep_for(1ms);
    };

    return future.get();
}


void Tetris::get_input()
{
    int input = getch();
    if (input == '\033')
    {
        input = getch();
        if (input == '[')
        {
            input = getch();
            switch (input)
            {
              case 'A':return RotateCW();
            //   case 'B':return SoftDrop();
              case 'C':return MoveRight();
              case 'D':return MoveLeft();
              default:;
            }
        }
    }
    switch (input)
    {
      case 'z': return RotateCC();
      case 'x': return RotateCW();
      case 'c': return Hold();
      case ' ': return HardDrop();
      default: return;
    }
}


Tetris::~Tetris()
{
    run = false;
    input_thr.join();
    launch_thr.join();
}
