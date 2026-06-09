#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "LevelData.h"
#include "ScoreManager.h"

class GameModel
{
public:
    ScoreManager &scoreManager() { return _scoreMgr; }

    const ScoreManager &scoreManager() const { return _scoreMgr; }

    int ballCount() const { return _ballCount; }

    int targetsRemaining() const { return _targetsRemaining; }

    bool isCleared() const { return _targetsRemaining <= 0; }

    int levelIndex() const { return _levelIndex; }

    bool hasNextLevel() const;
    const LevelData &currentLevel() const;

    void tick(float dt) { _scoreMgr.tick(dt); }

    void loadLevel(int index);

    void setTargetsRemaining(int count) { _targetsRemaining = count; }

    bool canLaunchBall() const;
    void useBall();
    void removeTarget();
    int scoreHit();
    int scoreTargetFall();
    void resetCombo();

private:
    ScoreManager _scoreMgr;
    int _ballCount = 0;
    int _targetsRemaining = 0;
    int _levelIndex = 0;
    int _maxBalls = 0;
};

#endif
