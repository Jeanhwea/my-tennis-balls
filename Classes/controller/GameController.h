#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "controller/InputController.h"
#include "model/GameModel.h"
#include "view/AimLineView.h"
#include "view/HUD.h"

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
    InputController _input;
    AimLineView _aimLine;
    HUD *_hud = nullptr;

    int _ballCounter = 0;
    bool _transitioning = false;

    // 关卡
    void loadLevel(int index);
    void clearLevelNodes();
    void onLevelCleared();
    void onLevelFailed();
    void checkFailCondition();

    // 输入与物理
    void setupInput();
    void setupPhysics();
    void refreshHUD();

    // 球
    void spawnBall(const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity);
    void removeBall(cocos2d::Node *ball);
    void removeTarget(cocos2d::Node *target);
    int countBalls() const;
    int countTargets() const;
    void collectOutOfBounds();

    // 碰撞
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    bool handleFloorContact(cocos2d::Node *a, cocos2d::Node *b, cocos2d::PhysicsContact &contact);
    bool handleBallTargetContact(cocos2d::Node *a, cocos2d::Node *b, cocos2d::PhysicsContact &contact);
    bool handleBallBallContact(cocos2d::Node *a, cocos2d::Node *b, cocos2d::PhysicsContact &contact);
};

#endif  // __GAME_CONTROLLER_H__
