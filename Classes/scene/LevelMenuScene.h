#ifndef LEVEL_MENU_SCENE_H
#define LEVEL_MENU_SCENE_H

#include "cocos2d.h"

class LevelMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene *createScene();

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

#endif
