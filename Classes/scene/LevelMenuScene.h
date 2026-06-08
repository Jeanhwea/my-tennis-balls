/// @file LevelMenuScene.h
/// 关卡选择菜单场景，支持分页浏览关卡按钮，包含入场动画和导航控件。

#ifndef __LEVEL_MENU_SCENE_H__
#define __LEVEL_MENU_SCENE_H__

#include "cocos2d.h"

/// @class LevelMenuScene
/// 关卡选择菜单场景，支持分页浏览。
class LevelMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    /// 设置下次打开菜单时自动导航到该关卡索引所在的页面。
    static void setInitialLevelIndex(int levelIndex);

    bool init() override;
    void onEnter() override;

    CREATE_FUNC(LevelMenuScene);

private:
    static int s_initialPage;

    static constexpr int COLS = 4;
    static constexpr int ROWS = 4;
    static constexpr int PER_PAGE = COLS * ROWS;

    void drawBackground(const cocos2d::Size &size);
    void drawTitle(const cocos2d::Size &size);
    void createLevelButtons(const cocos2d::Size &size);
    void showPage(int page);

    void createOneButton(cocos2d::Node *parent, int levelIdx, int levelId, const std::string &name, float x,
                         float y, float btnW, float btnH);

    void createNavButtons(const cocos2d::Size &size);
    void updateNavButtons();
    void createQuitButton(const cocos2d::Size &size);

    cocos2d::Node *_pageContainer = nullptr;
    cocos2d::Label *_pageLabel = nullptr;
    cocos2d::Node *_prevBtn = nullptr;
    cocos2d::Node *_nextBtn = nullptr;
    int _currentPage = 0;
    int _totalPages = 1;
};

#endif  // __LEVEL_MENU_SCENE_H__
