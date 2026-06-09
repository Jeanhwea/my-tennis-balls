#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <algorithm>
#include <functional>

#include "common/GameConstants.h"

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

#endif
