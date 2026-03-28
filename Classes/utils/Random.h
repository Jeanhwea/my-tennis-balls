#ifndef __UTILS_RANDOM_H__
#define __UTILS_RANDOM_H__

#include <cstdlib>

namespace utils {

inline float randomFloat(float min, float max)
{
    return min + static_cast<float>(std::rand()) / RAND_MAX * (max - min);
}

}  // namespace utils

#endif  // __UTILS_RANDOM_H__
