#ifndef __LEVEL_DATA_H__
#define __LEVEL_DATA_H__

#include <string>
#include <vector>

/// Describes one tray shelf and its target balls.
struct TrayData {
    float x;      // center X ratio (0~1)
    float y;      // center Y ratio (0~1)
    float width;  // width ratio (0~1)
    int targets;  // number of target balls on this tray
};

/// Describes a complete level layout.
struct LevelData {
    int id;
    std::string name;
    int maxBalls;                 // max projectile balls allowed
    std::vector<TrayData> trays;  // tray configurations
};

/// Load and cache all levels from Resources/levels/level_NNN.json.
const std::vector<LevelData> &getAllLevels();

#endif  // __LEVEL_DATA_H__
