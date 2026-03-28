#ifndef __HUD_H__
#define __HUD_H__

#include "cocos2d.h"

/// 平视显示界面 - 分数、连击、球数、提示。
class HUD : public cocos2d::Node
{
public:
    static HUD *create(const cocos2d::Size &visibleSize);

    void updateScore(int score);
    void updateCombo(int combo);
    void updateBallCount(int current, int max);
    void updateTargets(int remaining);
    void updateLevel(int levelId, const std::string &name);
    void hideHint();
    void showCleared();
    void showLevelIntro(int levelId, const std::string &name);

private:
    bool initWithSize(const cocos2d::Size &visibleSize);

    cocos2d::Label *_scoreLabel = nullptr;
    cocos2d::Label *_comboLabel = nullptr;
    cocos2d::Label *_ballCountLabel = nullptr;
    cocos2d::Label *_targetLabel = nullptr;
    cocos2d::Label *_levelLabel = nullptr;
    cocos2d::Label *_hintLabel = nullptr;

    static constexpr float MARGIN = 20.0f;
    static constexpr float FONT_SZ = 24.0f;
    static constexpr float COMBO_SZ = 32.0f;
    static constexpr float HINT_SZ = 20.0f;
};

#endif  // __HUD_H__
