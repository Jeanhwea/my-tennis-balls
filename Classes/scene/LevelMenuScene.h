#ifndef __LEVEL_MENU_SCENE_H__
#define __LEVEL_MENU_SCENE_H__

#include "cocos2d.h"

/// 关卡选择菜单场景
class LevelMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void onEnter() override;

    CREATE_FUNC(LevelMenuScene);
};

#endif  // __LEVEL_MENU_SCENE_H__
