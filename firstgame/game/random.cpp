//
// Created by shepherd on 6.4.23.
//

#include <firstgame/game/random.hpp>

namespace firstgame {

std::random_device Random::device_;
std::mt19937 Random::generator_;
std::uniform_int_distribution<std::mt19937::result_type> Random::distribution_;

void Random::Init() {
    generator_.seed(device_());
}

float Random::Get() {
    return (float) distribution_(generator_) / std::numeric_limits<uint64_t>::max();
}

} // firstgame