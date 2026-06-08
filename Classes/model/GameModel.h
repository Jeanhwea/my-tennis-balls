/// @file GameModel.h
/// 核心游戏状态模型，纯数据无渲染。维护关卡索引、计分、球数、目标剩余等运行时状态。

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "LevelData.h"
#include "ScoreManager.h"

/// @class GameModel
/// 核心游戏状态，纯数据无渲染。
class GameModel
{
public:
    ScoreManager &scoreManager() { return _scoreMgr; }

    const ScoreManager &scoreManager() const { return _scoreMgr; }

    int ballCount() const { return _ballCount; }

    void setBallCount(int count) { _ballCount = count; }

    int targetsRemaining() const { return _targetsRemaining; }

    void setTargetsRemaining(int count) { _targetsRemaining = count; }

    bool isCleared() const { return _targetsRemaining <= 0; }

    int levelIndex() const { return _levelIndex; }

    void setLevelIndex(int idx) { _levelIndex = idx; }

    bool hasNextLevel() const;
    const LevelData &currentLevel() const;

    void tick(float dt) { _scoreMgr.tick(dt); }

private:
    ScoreManager _scoreMgr;
    int _ballCount = 0;
    int _targetsRemaining = 0;
    int _levelIndex = 0;
};

#endif  // __GAME_MODEL_H__
