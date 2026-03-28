#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "controller/GameController.h"

/// 轻量场景外壳 - 将所有逻辑委托给 GameController。
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void onEnter() override;
    void update(float dt) override;

    CREATE_FUNC(GameScene);

private:
    GameController _controller;
};

#endif  // __GAME_SCENE_H__
