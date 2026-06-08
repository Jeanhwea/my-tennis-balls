#include "ScoreManager.h"

int ScoreManager::addScore(int basePoints)
{
    int points = basePoints * std::max(1, _combo);
    _score += points;
    _combo++;
    _comboTimer = COMBO_TIMEOUT;
    notifyChange();
    return points;
}

void ScoreManager::resetCombo()
{
    _combo = 0;
    _comboTimer = 0.0f;
    notifyChange();
}

void ScoreManager::tick(float dt)
{
    if (_combo > 0) {
        _comboTimer -= dt;
        if (_comboTimer <= 0.0f) {
            resetCombo();
        }
    }
}

void ScoreManager::notifyChange()
{
    if (_onChange) {
        _onChange();
    }
}
