#pragma once
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

struct rand
{
    template < class T >
    using lims = std::numeric_limits<T>;

    template < class T >
    using if_int = typename std::enable_if<std::is_integral<T>::value, bool>::type;

    template < class T >
    using if_flop = typename std::enable_if<std::is_floating_point<T>::value, bool>::type;

  public:
    template < class Int, if_int<Int> = true >
    static Int random(Int min = lims<Int>::min, Int max = lims<Int>::max)
    {
        std::uniform_int_distribution<Int> distrib(min, max - 1);
        return distrib(mt_gen);
    }
    template < class Int, if_int<Int> = true >
    static Int rand_fast(Int min = lims<Int>::min, Int max = lims<Int>::max)
    {
        std::uniform_int_distribution<Int> distrib(min, max - 1);
        return distrib(lin_gen);
    }
    template < class Flt, if_flop<Flt> = true >
    static Flt random(Flt min = lims<Flt>::min, Flt max = lims<Flt>::max)
    {
        std::uniform_int_distribution<Flt> distrib(min, max - 1);
        return distrib(mt_gen);
    }
    template < class Flt, if_flop<Flt> = true >
    static Flt rand_fast(Flt min = lims<Flt>::min, Flt max = lims<Flt>::max)
    {
        std::uniform_int_distribution<Flt> distrib(min, max - 1);
        return distrib(lin_gen);
    }

    rand() = delete;

  private:
    static std::random_device rd;
    static std::mt19937_64 mt_gen;
    static std::minstd_rand lin_gen;
};


std::random_device rand::rd { };
std::mt19937_64 rand::mt_gen { rd() };
std::minstd_rand rand::lin_gen { rd() };


#endif
