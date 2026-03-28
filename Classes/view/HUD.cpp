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
    float barTop = visibleSize.height;
    float barBot = visibleSize.height - 50;
    float barRight = visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO);

    // 渐变半透明背景
    constexpr int strips = 5;
    for (int i = 0; i < strips; ++i) {
        float t0 = static_cast<float>(i) / strips;
        float t1 = static_cast<float>(i + 1) / strips;
        float alpha = 0.45f - 0.15f * (t0 + t1) / 2;
        topBar->drawSolidRect(Vec2(0, barBot + (barTop - barBot) * t0),
                              Vec2(barRight, barBot + (barTop - barBot) * t1),
                              Color4F(0.02f, 0.03f, 0.08f, alpha));
    }

    // 底部高光线
    topBar->drawSolidRect(Vec2(0, barBot), Vec2(barRight, barBot + 1), Color4F(0.3f, 0.5f, 0.9f, 0.35f));
    topBar->drawSolidRect(Vec2(0, barBot + 1), Vec2(barRight, barBot + 2),
                          Color4F(0.2f, 0.35f, 0.7f, 0.15f));
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
    // 渐变填充
    constexpr int strips = 3;
    for (int i = 0; i < strips; ++i) {
        float t0 = static_cast<float>(i) / strips;
        float t1 = static_cast<float>(i + 1) / strips;
        float bright = 0.12f + 0.06f * (t0 + t1) / 2;
        bg->drawSolidRect(Vec2(-BTN_W / 2, -BTN_H / 2 + BTN_H * t0),
                          Vec2(BTN_W / 2, -BTN_H / 2 + BTN_H * t1),
                          Color4F(bright, bright + 0.04f, bright + 0.18f, 0.9f));
    }
    // 顶部高光
    bg->drawSolidRect(Vec2(-BTN_W / 2, BTN_H / 2 - 1), Vec2(BTN_W / 2, BTN_H / 2),
                      Color4F(0.4f, 0.6f, 1.0f, 0.3f));
    bg->drawRect(Vec2(-BTN_W / 2, -BTN_H / 2), Vec2(BTN_W / 2, BTN_H / 2), Color4F(0.3f, 0.5f, 0.9f, 0.5f));
    bg->setPosition(Vec2(x, y));
    addChild(bg, 1);

    auto label = Label::createWithTTF("\xe2\x97\x80 BACK", FONT_UI, 14);
    label->setPosition(Vec2(x, y));
    label->setTextColor(Color4B(180, 210, 255, 230));
    label->enableShadow(Color4B(0, 0, 0, 80), Size(1, -1));
    addChild(label, 2);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [x, y, bg](Touch *touch, Event *) {
        Rect rect(x - BTN_W / 2, y - BTN_H / 2, BTN_W, BTN_H);
        if (rect.containsPoint(touch->getLocation())) {
            bg->setScale(0.93f);
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [this, bg](Touch *, Event *) {
        bg->setScale(1.0f);
        if (_onBack) _onBack();
    };
    listener->onTouchCancelled = [bg](Touch *, Event *) { bg->setScale(1.0f); };
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
    auto parentSize = getParent()->getContentSize();
    float cx = parentSize.width * 0.35f;
    float cy = parentSize.height / 2;

    // 背景闪光
    auto flash = DrawNode::create();
    flash->drawSolidCircle(Vec2(cx, cy), 120.0f, 0, 24, Color4F(0.4f, 0.6f, 1.0f, 0.15f));
    flash->setScale(0);
    getParent()->addChild(flash, 29);
    flash->runAction(Sequence::create(
        Spawn::create(EaseOut::create(ScaleTo::create(0.5f, 3.0f), 2), FadeOut::create(0.6f), nullptr),
        RemoveSelf::create(), nullptr));

    auto cleared = Label::createWithTTF("ALL CLEAR!", FONT_TITLE, 52);
    cleared->setPosition(Vec2(cx, cy));
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
