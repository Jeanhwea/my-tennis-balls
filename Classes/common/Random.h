#ifndef __COMMON_RANDOM_H__
#define __COMMON_RANDOM_H__

#include <cstdlib>

namespace common
{

inline float randomFloat(float min, float max)
{
    return min + static_cast<float>(std::rand()) / RAND_MAX * (max - min);
}

}  // namespace common

#endif  // __COMMON_RANDOM_H__
