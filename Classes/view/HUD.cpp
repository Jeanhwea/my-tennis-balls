#include "HUD.h"

#include "common/GameConstants.h"

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

    createTopBar(visibleSize);
    createScoreLabel(visibleSize);
    createBallCountLabel(visibleSize);
    createComboLabel(visibleSize);
    createTargetLabel(visibleSize);
    createLevelLabel(visibleSize);
    createHintLabel(visibleSize);
    createBackButton(visibleSize);

    return true;
}

void HUD::createTopBar(const Size &visibleSize)
{
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, visibleSize.height - 50),
                          Vec2(visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO), visibleSize.height),
                          Color4F(0.0f, 0.0f, 0.0f, 0.35f));
    addChild(topBar, -1);
}

void HUD::createScoreLabel(const Size &visibleSize)
{
    _scoreLabel = Label::createWithTTF("Score: 0", FONT_UI, FONT_SZ);
    _scoreLabel->setAnchorPoint(Vec2(0, 1));
    _scoreLabel->setPosition(Vec2(MARGIN, visibleSize.height - MARGIN + 4));
    _scoreLabel->setTextColor(Color4B(220, 240, 255, 255));
    _scoreLabel->enableShadow(Color4B(0, 0, 0, 120), Size(1, -1));
    addChild(_scoreLabel);
}

void HUD::createBallCountLabel(const Size &visibleSize)
{
    _ballCountLabel = Label::createWithTTF("Balls: 0", FONT_UI, FONT_SZ - 2);
    _ballCountLabel->setAnchorPoint(Vec2(1, 1));
    float ballLabelX = visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO) - MARGIN;
    _ballCountLabel->setPosition(Vec2(ballLabelX, visibleSize.height - MARGIN + 4));
    _ballCountLabel->setTextColor(Color4B(180, 200, 220, 220));
    _ballCountLabel->enableShadow(Color4B(0, 0, 0, 120), Size(1, -1));
    addChild(_ballCountLabel);
}

void HUD::createComboLabel(const Size &visibleSize)
{
    _comboLabel = Label::createWithTTF("", FONT_TITLE, COMBO_SZ);
    _comboLabel->setPosition(Vec2(visibleSize.width * 0.35f, visibleSize.height / 2 + 80));
    _comboLabel->setTextColor(Color4B(255, 220, 50, 255));
    _comboLabel->enableShadow(Color4B(200, 150, 0, 100), Size(2, -2));
    _comboLabel->setVisible(false);
    addChild(_comboLabel);
}

void HUD::createTargetLabel(const Size &visibleSize)
{
    _targetLabel = Label::createWithTTF("Targets: 0", FONT_UI, FONT_SZ - 2);
    _targetLabel->setAnchorPoint(Vec2(0, 1));
    _targetLabel->setPosition(Vec2(MARGIN, visibleSize.height - MARGIN - 24));
    _targetLabel->setTextColor(Color4B(255, 200, 80, 230));
    _targetLabel->enableShadow(Color4B(0, 0, 0, 120), Size(1, -1));
    addChild(_targetLabel);
}

void HUD::createLevelLabel(const Size &visibleSize)
{
    _levelLabel = Label::createWithTTF("Level 1", FONT_TITLE, FONT_SZ);
    _levelLabel->setAnchorPoint(Vec2(0.5f, 1));
    float levelX = visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO) / 2;
    _levelLabel->setPosition(Vec2(levelX, visibleSize.height - MARGIN + 4));
    _levelLabel->setTextColor(Color4B(160, 200, 255, 200));
    _levelLabel->enableShadow(Color4B(0, 0, 0, 120), Size(1, -1));
    addChild(_levelLabel);
}

void HUD::createHintLabel(const Size &visibleSize)
{
    float levelX = visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO) / 2;
    _hintLabel = Label::createWithTTF("Drag in LAUNCH zone to shoot!", FONT_UI, HINT_SZ);
    _hintLabel->setPosition(Vec2(levelX, visibleSize.height - MARGIN * 3));
    _hintLabel->setTextColor(Color4B(200, 220, 255, 140));
    addChild(_hintLabel);
}

void HUD::createBackButton(const Size &visibleSize)
{
    static constexpr float BTN_W = 80.0f;
    static constexpr float BTN_H = 32.0f;

    float x = visibleSize.width - MARGIN - BTN_W / 2;
    float y = visibleSize.height - MARGIN - BTN_H / 2;

    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(-BTN_W / 2, -BTN_H / 2), Vec2(BTN_W / 2, BTN_H / 2),
                      Color4F(0.15f, 0.2f, 0.35f, 0.85f));
    bg->drawRect(Vec2(-BTN_W / 2, -BTN_H / 2), Vec2(BTN_W / 2, BTN_H / 2), Color4F(0.4f, 0.6f, 1.0f, 0.5f));
    bg->setPosition(Vec2(x, y));
    addChild(bg, 1);

    auto label = Label::createWithTTF("< BACK", FONT_UI, 16);
    label->setPosition(Vec2(x, y));
    label->setTextColor(Color4B(180, 210, 255, 220));
    addChild(label, 2);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [x, y](Touch *touch, Event *) {
        Rect rect(x - BTN_W / 2, y - BTN_H / 2, BTN_W, BTN_H);
        return rect.containsPoint(touch->getLocation());
    };
    listener->onTouchEnded = [this](Touch *, Event *) {
        if (_onBack) _onBack();
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bg);
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
    auto cleared = Label::createWithTTF("ALL CLEAR!", FONT_TITLE, 52);
    cleared->setPosition(
        Vec2(getParent()->getContentSize().width * 0.35f, getParent()->getContentSize().height / 2));
    cleared->setTextColor(Color4B(255, 230, 50, 255));
    cleared->enableShadow(Color4B(200, 150, 0, 150), Size(2, -2));
    cleared->setScale(0);
    getParent()->addChild(cleared, 30);

    cleared->runAction(Sequence::create(
        EaseBackOut::create(ScaleTo::create(0.4f, 1.2f)), EaseInOut::create(ScaleTo::create(0.3f, 1.0f), 2),
        DelayTime::create(1.2f), FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
}

void HUD::updateLevel(int levelId, const std::string &name)
{
    _levelLabel->setString(StringUtils::format("Level %d - %s", levelId, name.c_str()));
}

void HUD::showLevelIntro(int levelId, const std::string &name)
{
    float cx = getParent()->getContentSize().width * 0.35f;
    float cy = getParent()->getContentSize().height / 2;

    auto intro =
        Label::createWithTTF(StringUtils::format("Level %d\n%s", levelId, name.c_str()), FONT_TITLE, 44);
    intro->setPosition(Vec2(cx, cy));
    intro->setTextColor(Color4B(160, 210, 255, 255));
    intro->enableShadow(Color4B(0, 40, 100, 150), Size(2, -2));
    intro->setAlignment(TextHAlignment::CENTER);
    intro->setScale(0);
    getParent()->addChild(intro, 30);

    intro->runAction(
        Sequence::create(EaseBackOut::create(ScaleTo::create(0.35f, 1.1f)),
                         EaseInOut::create(ScaleTo::create(0.2f, 1.0f), 2), DelayTime::create(1.0f),
                         Spawn::create(FadeOut::create(0.4f), MoveBy::create(0.4f, Vec2(0, 30)), nullptr),
                         RemoveSelf::create(), nullptr));
}
