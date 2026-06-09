#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <functional>
#include "cocos2d.h"
#include "model/GameModel.h"
#include "model/BallManager.h"

class LevelManager
{
public:
    void init(cocos2d::Scene *scene, const cocos2d::Size &visibleSize, GameModel &model, BallManager &ballManager, int startLevel = 0);

    void update(float dt);
    bool loadLevel(int index);
    void clearLevelNodes();
    void onLevelCleared();
    void onLevelFailed();
    void checkFailCondition();

    bool isTransitioning() const { return _transitioning; }
    int ballCounter() const { return _ballCounter; }
    void incrementBallCounter() { _ballCounter++; }

    void setOnLevelChanged(std::function<void(int levelIndex)> cb) { _onLevelChanged = std::move(cb); }

private:
    cocos2d::Scene *_scene = nullptr;
    cocos2d::Size _visibleSize;
    GameModel *_model = nullptr;
    BallManager *_ballManager = nullptr;

    int _ballCounter = 0;
    bool _transitioning = false;
    std::function<void(int levelIndex)> _onLevelChanged;
};

#endif
