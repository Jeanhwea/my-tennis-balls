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

private:
    void drawBackground(const cocos2d::Size &size);
    void drawTitle(const cocos2d::Size &size);
    void createLevelButtons(const cocos2d::Size &size);

    /// 计算网格布局的起始坐标。
    cocos2d::Vec2 calcGridOrigin(const cocos2d::Size &size, int count, int cols,
                                 float btnW, float btnH, float gapX, float gapY) const;

    void createOneButton(int levelIdx, int levelId, const std::string &name,
                         float x, float y, float btnW, float btnH);
};

#endif  // __LEVEL_MENU_SCENE_H__
