//
// Created by shepherd on 6.4.23.
//

#ifndef FIRSTGAME_RANDOM_HPP
#define FIRSTGAME_RANDOM_HPP

#include <random>

namespace firstgame {

// https://stackoverflow.com/questions/5019393/random-number-generator-c
class Random {
public:
    static void Init();

    static float Get();

private:
    static std::random_device device_;
    static std::mt19937 generator_;
    // https://stackoverflow.com/questions/32860654/what-are-the-advantages-of-using-uniform-int-distribution-vs-a-modulus-operation
    static std::uniform_int_distribution<std::mt19937::result_type> distribution_;
};

} // firstgame

#endif //FIRSTGAME_RANDOM_HPP
