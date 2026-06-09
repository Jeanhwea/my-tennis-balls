/// @file LevelData.h
/// Level data structures: TrayData and LevelData.

#ifndef __LEVEL_DATA_H__
#define __LEVEL_DATA_H__

#include <string>
#include <vector>

/// @struct TrayData
struct TrayData {
    float x;
    float y;
    float width;
    int targets;
};

struct LevelData {
    int id;
    std::string name;
    int maxBalls;
    std::vector<TrayData> trays;
};

const std::vector<LevelData> &getAllLevels();

#endif  // __LEVEL_DATA_H__
