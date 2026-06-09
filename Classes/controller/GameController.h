/// @file GameController.h
/// 游戏核心控制器，协调 Model ↔ View 通信、关卡生命周期、输入事件和物理碰撞回调。

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "controller/InputController.h"
#include "controller/BallManager.h"
#include "controller/CollisionSystem.h"
#include "model/GameModel.h"
#include "view/AimLineView.h"
#include "view/HUD.h"

/// @class GameController
/// 协调 Model ↔ View 通信、关卡推进和物理系统。
class GameController
{
public:
    void init(cocos2d::Scene *scene, const cocos2d::Size &visibleSize, int startLevel = 0);
    void update(float dt);

private:
    cocos2d::Scene *_scene = nullptr;
    cocos2d::Size _visibleSize;

    GameModel _model;
    BallManager _ballMgr;
    CollisionSystem _collision;
    InputController _input;
    AimLineView _aimLine;
    HUD *_hud = nullptr;

    int _ballCounter = 0;
    bool _transitioning = false;

    void loadLevel(int index);
    void clearLevelNodes();
    void onLevelCleared();
    void onLevelFailed();
    void checkFailCondition();

    void setupInput();
    void setupPhysics();
    void refreshHUD();
};

#endif  // __GAME_CONTROLLER_H__
