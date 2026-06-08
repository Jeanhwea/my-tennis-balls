#include "GameModel.h"

#include "common/GameConstants.h"

bool GameModel::hasNextLevel() const
{
    return _levelIndex + 1 < static_cast<int>(getAllLevels().size());
}

const LevelData &GameModel::currentLevel() const
{
    return getAllLevels().at(_levelIndex);
}

void GameModel::loadLevel(int index)
{
    _levelIndex = index;
    _ballCount = 0;
    _maxBalls = currentLevel().maxBalls;
    _targetsRemaining = 0;
}

bool GameModel::canLaunchBall() const
{
    return _ballCount < _maxBalls;
}

void GameModel::useBall()
{
    _ballCount++;
}

void GameModel::removeTarget()
{
    _targetsRemaining--;
}

int GameModel::scoreHit()
{
    return _scoreMgr.addScore(SCORE_PER_HIT);
}

int GameModel::scoreTargetFall()
{
    return _scoreMgr.addScore(SCORE_TARGET_FALL);
}

void GameModel::resetCombo()
{
    _scoreMgr.resetCombo();
}
