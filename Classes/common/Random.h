#ifndef __COMMON_RANDOM_H__
#define __COMMON_RANDOM_H__

#include <random>

namespace common
{

inline std::mt19937 &rng()
{
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

inline float randomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng());
}

inline int randomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng());
}

}  // namespace common

#endif  // __COMMON_RANDOM_H__
