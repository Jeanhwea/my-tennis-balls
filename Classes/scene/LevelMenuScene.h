/// @file LevelMenuScene.h
/// Level selection menu scene, supports paginated browsing of level buttons with entry animations and navigation controls.

#ifndef __LEVEL_MENU_SCENE_H__
#define __LEVEL_MENU_SCENE_H__

#include "cocos2d.h"

/// @class LevelMenuScene
/// Level selection menu scene, supports paginated browsing.
class LevelMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

    /// Set the auto-navigate page for the level index when opening the menu next time.
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
