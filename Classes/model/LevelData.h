
#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

#include <string>
#include <vector>

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

#endif
