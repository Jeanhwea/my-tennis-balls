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

    _targetLabel = Label::createWithSystemFont("Targets: 0", "Arial", FONT_SZ);
    _targetLabel->setAnchorPoint(Vec2(0.5f, 1));
    _targetLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - MARGIN));
    _targetLabel->setTextColor(Color4B(255, 200, 80, 255));
    addChild(_targetLabel);

    _hintLabel = Label::createWithSystemFont("Drag in LAUNCH zone to shoot!", "Arial", HINT_SZ);
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

void HUD::updateTargets(int remaining)
{
    _targetLabel->setString(StringUtils::format("Targets: %d", remaining));
}

void HUD::hideHint()
{
    if (_hintLabel && _hintLabel->isVisible()) {
        _hintLabel->runAction(Sequence::create(FadeOut::create(0.5f), Hide::create(), nullptr));
    }
}

void HUD::showCleared()
{
    auto cleared = Label::createWithSystemFont("ALL CLEAR!", "Arial", 48);
    cleared->setPosition(getParent()->getContentSize() / 2);
    cleared->setTextColor(Color4B(255, 220, 50, 255));
    cleared->setScale(0);
    getParent()->addChild(cleared, 30);

    cleared->runAction(Sequence::create(
        EaseBackOut::create(ScaleTo::create(0.4f, 1.2f)),
        DelayTime::create(1.5f),
        FadeOut::create(0.5f),
        RemoveSelf::create(),
        nullptr));
}
