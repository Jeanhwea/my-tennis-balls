#include "HUD.h"

USING_NS_CC;

HUD *HUD::create(const Size &visibleSize)
{
    auto hud = new (std::nothrow) HUD();
    if (hud && hud->initWithSize(visibleSize)) {
        hud->autorelease();
        return hud;
    }
    CC_SAFE_DELETE(hud);
    return nullptr;
}

bool HUD::initWithSize(const Size &visibleSize)
{
    if (!Node::init()) return false;

    _scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", FONT_SZ);
    _scoreLabel->setAnchorPoint(Vec2(0, 1));
    _scoreLabel->setPosition(Vec2(MARGIN, visibleSize.height - MARGIN));
    _scoreLabel->setTextColor(Color4B::WHITE);
    addChild(_scoreLabel);

    _ballCountLabel = Label::createWithSystemFont("Balls: 0", "Arial", FONT_SZ);
    _ballCountLabel->setAnchorPoint(Vec2(1, 1));
    _ballCountLabel->setPosition(Vec2(visibleSize.width - MARGIN, visibleSize.height - MARGIN));
    _ballCountLabel->setTextColor(Color4B::WHITE);
    addChild(_ballCountLabel);

    _comboLabel = Label::createWithSystemFont("", "Arial", COMBO_SZ);
    _comboLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - MARGIN - 10));
    _comboLabel->setTextColor(Color4B::YELLOW);
    _comboLabel->setVisible(false);
    addChild(_comboLabel);

    _hintLabel = Label::createWithSystemFont("Drag to launch a ball!", "Arial", HINT_SZ);
    _hintLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - MARGIN * 3));
    _hintLabel->setTextColor(Color4B(255, 255, 255, 160));
    addChild(_hintLabel);

    return true;
}

void HUD::updateScore(int score)
{
    _scoreLabel->setString(StringUtils::format("Score: %d", score));
}

void HUD::updateCombo(int combo)
{
    if (combo > 1) {
        _comboLabel->setString(StringUtils::format("Combo x%d", combo));
        _comboLabel->setVisible(true);
        _comboLabel->stopAllActions();
        _comboLabel->setScale(1.3f);
        _comboLabel->runAction(EaseBackOut::create(ScaleTo::create(0.2f, 1.0f)));
    } else {
        _comboLabel->setVisible(false);
    }
}

void HUD::updateBallCount(int current, int max)
{
    _ballCountLabel->setString(StringUtils::format("Balls: %d/%d", current, max));
}

void HUD::hideHint()
{
    if (_hintLabel && _hintLabel->isVisible()) {
        _hintLabel->runAction(Sequence::create(FadeOut::create(0.5f), Hide::create(), nullptr));
    }
}
