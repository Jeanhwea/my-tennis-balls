#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include "controller/GameController.h"

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

#endif
