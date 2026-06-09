/// @file GameScene.h
/// Main game scene: initializes physics world and creates GameController.

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "controller/GameController.h"

/// @class GameScene
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();
    static cocos2d::Scene *createSceneWithLevel(int levelIndex);

    ~GameScene() override;

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
