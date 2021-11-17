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


    char randchar()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> distrib(32, 126);
        return char(distrib(gen));
    }


    char randof(const std::vector<char>& chars)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, chars.size() - 1);
        return chars.at(distrib(gen));
    }


    template < std::size_t W >
    std::string make_rand_line(const std::vector<char>& chars)
    {
        std::stringstream ss;
        for (int i = 0; i < W; i++)
            ss << randof(chars);
        return ss.str();
    }
}

#endif
