/// @file ScoreManager.h
/// 纯计分逻辑，无渲染依赖。管理分数累计、连击计数和连击超时重置。

#ifndef __SCORE_MANAGER_H__
#define __SCORE_MANAGER_H__

#include <algorithm>
#include <functional>

#include "common/GameConstants.h"

/// @class ScoreManager
/// 纯计分逻辑，无渲染依赖。
class ScoreManager
{
public:
    using OnChangeCallback = std::function<void()>;

    void setOnChange(OnChangeCallback cb) { _onChange = std::move(cb); }

    int addScore(int basePoints);
    void resetCombo();
    void tick(float dt);

    int score() const { return _score; }

    int combo() const { return _combo; }

private:
    int _score = 0;
    int _combo = 0;
    float _comboTimer = 0.0f;
    OnChangeCallback _onChange;

    void notifyChange();
};

#endif  // __SCORE_MANAGER_H__
