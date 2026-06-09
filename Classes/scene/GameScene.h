/// @file GameScene.h
/// 主游戏场景。初始化物理世界并创建 GameController 来协调游戏逻辑。

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "controller/GameController.h"

/// @class GameScene
/// 游戏场景，支持指定起始关卡。
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
