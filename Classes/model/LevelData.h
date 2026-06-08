/// @file LevelData.h
/// Level data structure definitions. Contains TrayData tray configuration and LevelData level layout.

#ifndef __LEVEL_DATA_H__
#define __LEVEL_DATA_H__

#include <string>
#include <vector>

/// @struct TrayData
/// Describes a single tray and its target balls.
struct TrayData {
    float x;      ///< Center X (proportion of screen width 0~1)
    float y;      ///< Center Y (proportion of screen height 0~1)
    float width;  ///< Width (proportion of screen width 0~1)
    int targets;  ///< Number of target balls on this tray
};

/// Describes a complete level layout.
struct LevelData {
    int id;
    std::string name;
    int maxBalls;                 ///< Maximum allowed launch balls
    std::vector<TrayData> trays;  ///< Tray configurations
};

/// Load and cache all levels from Resources/levels/level_NNN.json.
const std::vector<LevelData> &getAllLevels();

#endif  // __LEVEL_DATA_H__
