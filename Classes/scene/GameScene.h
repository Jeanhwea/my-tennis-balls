/// @file GameScene.h
/// Main game scene. Initializes the physics world and creates GameController to coordinate game logic.

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "controller/GameController.h"

/// @class GameScene
/// Game scene, supports specifying a starting level.
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();
    static cocos2d::Scene *createSceneWithLevel(int levelIndex);

    bool init() override;
    void onEnter() override;
    void update(float dt) override;

    CREATE_FUNC(GameScene);

    void setStartLevel(int index) { _startLevel = index; }

private:
    GameController _controller;
    int _startLevel = 0;
};

#endif  // __GAME_SCENE_H__
