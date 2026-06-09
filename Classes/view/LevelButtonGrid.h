#ifndef LEVEL_BUTTON_GRID_H
#define LEVEL_BUTTON_GRID_H

#include "cocos2d.h"

class LevelButtonGrid : public cocos2d::Node
{
public:
    static LevelButtonGrid *create(cocos2d::Node *parent, const cocos2d::Size &visibleSize, int startPage);

    void setOnPageChanged(std::function<void(int current, int total)> cb) { _onPageChanged = std::move(cb); }

private:
    bool init(cocos2d::Node *parent, const cocos2d::Size &visibleSize, int startPage);

    void showPage(cocos2d::Node *parent, const cocos2d::Size &size, int page);

    void createOneButton(cocos2d::Node *scene, cocos2d::Node *container, int levelIndex, int levelId,
                        const std::string &name, float x, float y);

    cocos2d::Node *_pageContainer = nullptr;
    int _currentPage = 0;
    int _totalPages = 1;

    std::function<void(int current, int total)> _onPageChanged;

    static constexpr int TAG_BUTTON = 999;
    static constexpr int COLS = 4;
    static constexpr int ROWS = 4;
    static constexpr int PER_PAGE = COLS * ROWS;
    static constexpr float BTN_W = 120.0f;
    static constexpr float BTN_H = 65.0f;
    static constexpr float GAP_X = 14.0f;
    static constexpr float GAP_Y = 14.0f;
};

#endif  // LEVEL_BUTTON_GRID_H
