/// @file HUD.h
/// 平视显示界面。管理分数、连击、球数、关卡提示、过关/失败动画及返回按钮。

#ifndef __HUD_H__
#define __HUD_H__

#include "cocos2d.h"

/// @class HUD
/// 平视显示界面：分数、连击、球数、关卡提示。
class HUD : public cocos2d::Node
{
public:
    static HUD *create(const cocos2d::Size &visibleSize);

    /// 返回按钮回调。
    void setOnBack(std::function<void()> cb) { _onBack = std::move(cb); }

    void updateScore(int score);
    void updateCombo(int combo);
    void updateBallCount(int current, int max);
    void updateTargets(int remaining);
    void updateLevel(int levelId, const std::string &name);
    void hideHint();
    void showCleared();
    void showFailed();
    void showLevelIntro(int levelId, const std::string &name);

private:
    bool initWithSize(const cocos2d::Size &visibleSize);

    void createTopBar(const cocos2d::Size &size);
    void createScoreLabel(const cocos2d::Size &size);
    void createBallCountLabel(const cocos2d::Size &size);
    void createComboLabel(const cocos2d::Size &size);
    void createTargetLabel(const cocos2d::Size &size);
    void createLevelLabel(const cocos2d::Size &size);
    void createHintLabel(const cocos2d::Size &size);
    void createBackButton(const cocos2d::Size &size);

    cocos2d::Label *_scoreLabel = nullptr;
    cocos2d::Label *_comboLabel = nullptr;
    cocos2d::Label *_ballCountLabel = nullptr;
    cocos2d::Label *_targetLabel = nullptr;
    cocos2d::Label *_levelLabel = nullptr;
    cocos2d::Label *_hintLabel = nullptr;

    std::function<void()> _onBack;

    static constexpr float MARGIN = 20.0f;
    static constexpr float FONT_SZ = 24.0f;
    static constexpr float COMBO_SZ = 32.0f;
    static constexpr float HINT_SZ = 20.0f;
    static constexpr float TOP_BAR_HEIGHT = 50.0f;
};

#endif  // __HUD_H__
