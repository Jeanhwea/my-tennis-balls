#ifndef HUD_TOP_BAR_H
#define HUD_TOP_BAR_H

#include "cocos2d.h"

class HUDTopBar : public cocos2d::Node
{
public:
    static HUDTopBar *create(const cocos2d::Size &visibleSize);

    void updateScore(int score);
    void updateCombo(int combo);
    void updateBallCount(int current, int max);
    void updateTargets(int remaining);
    void updateLevel(int levelId, const std::string &name);
    void hideHint();
    void setOnBack(std::function<void()> cb) { _onBack = std::move(cb); }

    void setOverlayParent(cocos2d::Node *parent) { _overlayParent = parent; }

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
    cocos2d::Node *_overlayParent = nullptr;

    static constexpr float MARGIN = 20.0f;
    static constexpr float FONT_SZ = 24.0f;
    static constexpr float COMBO_SZ = 32.0f;
    static constexpr float HINT_SZ = 20.0f;
    static constexpr float TOP_BAR_HEIGHT = 50.0f;
};

#endif
