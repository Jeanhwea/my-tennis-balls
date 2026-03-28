#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "Controller/InputController.h"
#include "Model/GameModel.h"
#include "View/AimLineView.h"
#include "View/HUD.h"
#include "cocos2d.h"

/// Orchestrates Model ↔ View communication and handles physics events.
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

    // Wiring
    void setupInput();
    void setupPhysics();
    void refreshHUD();

    // Ball management
    void spawnBall(const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity);
    void removeBall(cocos2d::Node *ball);
    int countBalls() const;

    // Physics
    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif  // __GAME_CONTROLLER_H__
