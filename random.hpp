#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <string>
#include <random>


namespace tetris
{
    bool randbool()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> distrib(0, 1);
        return distrib(gen);
    }


    /**
    * creates a line `W` chars long of random 1s and 0s
    */
    template < std::size_t W >
    std::string make_rand_line()
    {
        std::stringstream ss;
        for (int i = 0; i < W; i++)
            ss << randbool();
        return ss.str();
    }
}

#endif
