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
    board = { };
    scr.bg_char('.');
    scr.clear();
    launch_thr = std::thread(&Tetris::launch, this);
    input_thr = std::thread(&Tetris::input_loop, this);
}


// TODO: user input.
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
            shape.descend(level);
            auto coords = shape.coords();
            for (auto [x, y] : coords)
                if (y >= H - 1 || board.at(y + 1).test(x))
                    landed = true;
            if (landed)
                for (auto [x, y] : coords)
                    board.at(y).set(x);
        }
        else on_land();

        shape.draw(scr, reset_shape);
        if (reset_shape)
            reset_shape = false;

        std::this_thread::sleep_for(millis(250));
    }
    run = false;
}


void Tetris::hard_drop()
{
    // get lowest active x positions, and the
    //     lowest active y position
    // get highest conflicting landed block
    // teleport to one above

    using size_t = Shape::size_type;

    const std::array<std::pair<size_t, size_t>, 4>
        coords = shape.coords();
    std::map<uint, uint> lows; // lowest y in each col
    uint high_y = 0;
    for (auto [x, y] : coords)
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

    shape.set_y(high_y);
    landed = true;
}


void Tetris::soft_drop()
{
    uint old_speed = speed;
    if (!landed)
        speed = speed * 2;
    while (!landed);
    speed = old_speed;
}


void Tetris::go_left()
{
    if (shape.left() > 0)
        shape.go_left();
}


void Tetris::go_right()
{
    if (shape.right() > 0)
        shape.go_right();
}


void Tetris::rotate_cw()
{
    ;
}


void Tetris::rotate_cc()
{
    while (shape.right() == W && shape.left() + shape.height() >= W)
        shape.go_left();
    while (shape.left() == 0) && shape.right() + shape.height();

    shape.rotate_cc();
}


void Tetris::hold()
{
    Shape temp = queued;
    queued = shape;
    shape = temp;
}


void Tetris::on_land()
{
    clean_rows();
    shape = rand_shape();
    reset_shape = true;
    landed = false;
    landed_count += 1;

    if (landed_count % thresh == 0)
    {
        // thresh -= (thresh > 1 && thresh % accel == 0);
        speed += 1;
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
    std::condition_variable& cv = scr.io_cvar();
    std::future<int> future;

    while (!input_read)
    {
        std::unique_lock lock(scr.get_mutex());
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
    const auto lambda = [input, this]() -> void
    {
        switch (input)
        {
          case 'z': this->rotate_cc(); return;
          case 'x': this->rotate_cw(); return;
          case 'c': this->hold(); return;
          case ' ': this->hard_drop(); return;
          default: return;
        }
    };
    if (input == '\033')
    {
        input = getch();
        if (input == '[')
        {
            input = getch();
            switch (input)
            {
              case 'A': rotate_cw(); return;
              case 'B': soft_drop(); return;
              case 'C': go_right(); return;
              case 'D': go_left(); return;
              default:;
            }
        }
    }
    return lambda();
}


Tetris::~Tetris()
{
    run = false;
    input_thr.join();
    launch_thr.join();
}
