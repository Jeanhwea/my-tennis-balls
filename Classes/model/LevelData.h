/// @file LevelData.h
/// 关卡数据结构定义。包含 TrayData 托盘配置和 LevelData 关卡布局。

#ifndef __LEVEL_DATA_H__
#define __LEVEL_DATA_H__

#include <string>
#include <vector>

/// @struct TrayData
/// 描述单个托盘及其上的目标球。
struct TrayData {
    float x;      ///< 中心 X（屏幕宽度比例 0~1）
    float y;      ///< 中心 Y（屏幕高度比例 0~1）
    float width;  ///< 宽度（屏幕宽度比例 0~1）
    int targets;  ///< 此托盘上的目标球数量
};

/// 描述完整的关卡布局。
struct LevelData {
    int id;
    std::string name;
    int maxBalls;                 ///< 允许的最大发射球数
    std::vector<TrayData> trays;  ///< 托盘配置
};

/// 从 Resources/levels/level_NNN.json 加载并缓存所有关卡。
const std::vector<LevelData> &getAllLevels();

#endif  // __LEVEL_DATA_H__
