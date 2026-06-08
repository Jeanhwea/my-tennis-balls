/// @file ScoreManager.h
/// Pure scoring logic, no rendering dependencies. Manages score accumulation, combo counting, and combo timeout reset.

#ifndef __SCORE_MANAGER_H__
#define __SCORE_MANAGER_H__

#include <algorithm>
#include <functional>

#include "common/GameConstants.h"

/// @class ScoreManager
/// Pure scoring logic, no rendering dependencies.
class ScoreManager
{
public:
    using OnChangeCallback = std::function<void()>;

    void setOnChange(OnChangeCallback cb) { _onChange = std::move(cb); }

    void addScore(int basePoints);
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
