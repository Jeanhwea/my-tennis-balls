#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "controller/InputController.h"
#include "model/GameModel.h"
#include "view/AimLineView.h"
#include "view/HUD.h"

/// Orchestrates Model ↔ View communication, level progression, and physics.
class GameController
{
public:
    void init(cocos2d::Scene *scene, const cocos2d::Size &visibleSize);
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

    // Level management
    void loadLevel(int index);
    void clearLevelNodes();
    void onLevelCleared();

    // Wiring
    void setupInput();
    void setupPhysics();
    void refreshHUD();

    // Ball management
    void spawnBall(const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity);
    void removeBall(cocos2d::Node *ball);
    void removeTarget(cocos2d::Node *target);
    int countBalls() const;
    int countTargets() const;

    // Physics
    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif  // __GAME_CONTROLLER_H__
