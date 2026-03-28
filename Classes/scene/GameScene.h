#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "controller/GameController.h"

/// 游戏场景 - 支持指定起始关卡
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
