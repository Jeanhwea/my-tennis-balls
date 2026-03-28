#include "ScoreManager.h"

void ScoreManager::addScore(int basePoints)
{
    _score += basePoints * std::max(1, _combo);
    _combo++;
    _comboTimer = COMBO_TIMEOUT;
    notifyChange();
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
