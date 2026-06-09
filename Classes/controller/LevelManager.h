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

    bool isTransitioning() const { return _transitioning; }

private:
    cocos2d::Scene *_scene = nullptr;
    cocos2d::Size _visibleSize;
    GameModel *_model = nullptr;
    BallManager *_ballManager = nullptr;

    bool _transitioning = false;
};

#endif  // LEVEL_MANAGER_H
