#include <random>
#include <future>
#include <thread>
#include <chrono>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <fstream>

#include "../Tetris/Tetris.hpp"
#include "../Screen.hpp"


using millis = std::chrono::milliseconds;


Tetris::Tetris(Screen& scr)
    : board({}), scr(scr)
{
    scr.BgChar('.');
    scr.Clear();
    launch_thr = std::thread(&Tetris::launch, this);
    input_thr = std::thread(&Tetris::input_loop, this);
}


void Tetris::launch()
{
    while (board[3].none())
    {
        std::thread sleep_thr([]{std::this_thread::sleep_for(millis(100));});
        if (!landed)
        {
            shape.Descend(level);
            auto Coords = shape.Coords();
            for (auto [x, y] : Coords)
            {
                if (y >= HEIGHT - 1 || board.at(y + 1).test(x))
                {
                    landed = true;
                    break;
                }
            }
            if (landed)
                for (auto [x, y] : Coords)
                    board.at(y).set(x);
        }
        else on_land(); 
        print();
        sleep_thr.join();
    }
    run = false;
}


void Tetris::print() const
{
    using std::ref;

    if (erase_shape)
        shape.EraseLast(scr);
    else const_cast<bool&>(erase_shape) = true;
    queued.EraseLast(scr);
    std::thread shape_thr(&Shape::Draw, ref(shape), ref(scr));
    std::thread queue_thr(&Shape::Draw, ref(queued), ref(scr));
    shape_thr.join();
    scr.FillLn(2, '=');
    queue_thr.join();
}


void Tetris::HardDrop()
{
    std::map<UIntFast, UIntFast> lows; // lowest y in each col
    UIntFast bottom = 0;
    for (auto [x, y] : shape.Coords())
    {
        auto [iter, assigned] = lows.try_emplace(x, y);
        if (!assigned && iter->second < y)
            iter->second = y;
    }

    for (auto [shape_x, shape_y] : lows)
    {
        for (UIntFast y = shape_y; y > 0; y--)
        {
            if (y < bottom && board.at(y).test(shape_x))
            {
                bottom = y;
                break;
            }
        }
    }

    shape.SetY(bottom - 1);
    landed = true;
}


void Tetris::SoftDrop()
{
    // UIntFast old_speed = speed;
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
    shape.swap(queued);
    shape.FullReset();
    queued.FullReset();
    shape.SetY(2);
    can_hold = false;
}


void Tetris::on_land()
{
    clean_rows();
    shape = queued;
    queued = rand_shape();
    shape.SetY(2);
    erase_shape = false;
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
    return Shape(id);
}


int Tetris::getch() const
{
    using namespace std::chrono_literals;
    using fs = std::future_status;

    [[maybe_unused]]
    int sys_stat = 0;
    int input = 0;
    std::condition_variable& cv = scr.IoCondition();
    std::future<int> future;

    while (input == 0)
    {
        std::unique_lock lock(scr.Mutex());
        cv.wait(lock);

        sys_stat = std::system("/bin/stty raw");
        future = std::async(std::getchar);
        if (future.wait_for(1ms) == fs::ready)
            input = future.get();
        sys_stat = std::system("/bin/stty cooked");

        lock.unlock();
        cv.notify_one();
        std::this_thread::sleep_for(1ms);
    };

    return input;
}


void Tetris::get_input()
{
    std::ofstream of("./input.log");

    of << "getting input...:" << std::endl;
    int input = getch();
    of.put(input);
    if (input == '\033')
    {
        input = getch();
        if (input == '[')
        {
            input = getch();
            of.put(input);
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
