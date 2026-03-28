#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "ScoreManager.h"

/// Central game state — pure data, no rendering.
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

    void tick(float dt) { _scoreMgr.tick(dt); }

private:
    ScoreManager _scoreMgr;
    int _ballCount = 0;
    int _targetsRemaining = 0;
};

#endif  // __GAME_MODEL_H__
