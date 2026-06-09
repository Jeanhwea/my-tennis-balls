#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "cocos2d.h"
#include "model/BallManager.h"
#include "model/CollisionSystem.h"
#include "controller/LevelManager.h"
#include "controller/InputController.h"
#include "model/GameModel.h"
#include "view/AimLineView.h"
#include "view/HUD.h"

class GameController
{
public:
    void init(cocos2d::Scene *scene, const cocos2d::Size &visibleSize, int startLevel = 0);
    void update(float dt);

private:
    cocos2d::Scene *_scene = nullptr;
    cocos2d::Size _visibleSize;

    GameModel _model;
    BallManager _ballManager;
    CollisionSystem _collisionSystem;
    LevelManager _levelManager;
    InputController _input;
    AimLineView _aimLine;
    HUD *_hud = nullptr;

    void refreshHUD();
};

#endif  // GAME_CONTROLLER_H