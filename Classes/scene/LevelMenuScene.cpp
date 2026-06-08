#include "LevelMenuScene.h"

#include "GameScene.h"
#include "common/GameConstants.h"
#include "model/LevelData.h"
#include "view/AmbientParticles.h"

USING_NS_CC;

int LevelMenuScene::s_initialPage = 0;

void LevelMenuScene::setInitialLevelIndex(int levelIndex)
{
    s_initialPage = levelIndex / PER_PAGE;
}

Scene *LevelMenuScene::createScene()
{
    return LevelMenuScene::create();
}

bool LevelMenuScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    setName("LevelMenuScene");
    return true;
}

void LevelMenuScene::onEnter()
{
    Scene::onEnter();
    auto size = Director::getInstance()->getVisibleSize();
    drawBackground(size);
    drawTitle(size);
    createLevelButtons(size);
    createNavButtons(size);
    createQuitButton(size);
}

void LevelMenuScene::drawBackground(const Size &size)
{
    auto bg = DrawNode::create();
    constexpr int strips = 24;
    Color4F bot(0.02f, 0.02f, 0.08f, 1.0f);
    Color4F top(0.06f, 0.04f, 0.14f, 1.0f);
    for (int i = 0; i < strips; ++i) {
        float t0 = static_cast<float>(i) / strips;
        float t1 = static_cast<float>(i + 1) / strips;
        Color4F c((bot.r + (top.r - bot.r) * (t0 + t1) / 2), (bot.g + (top.g - bot.g) * (t0 + t1) / 2),
                  (bot.b + (top.b - bot.b) * (t0 + t1) / 2), 1.0f);
        bg->drawSolidRect(Vec2(0, size.height * t0), Vec2(size.width, size.height * t1), c);
    }
    addChild(bg, -2);

    auto grid = DrawNode::create();
    Color4F gridColor(0.12f, 0.15f, 0.25f, 0.08f);
    constexpr float spacing = 80.0f;
    for (float x = 0; x <= size.width; x += spacing)
        grid->drawLine(Vec2(x, 0), Vec2(x, size.height), gridColor);
    for (float y = 0; y <= size.height; y += spacing)
        grid->drawLine(Vec2(0, y), Vec2(size.width, y), gridColor);
    addChild(grid, -1);

    auto corners = DrawNode::create();
    Color4F cColor(0.25f, 0.45f, 0.85f, 0.25f);
    constexpr float cLen = 40.0f;
    constexpr float cThick = 2.0f;
    constexpr float pad = 8.0f;
    float w = size.width;
    float h = size.height;

    corners->drawSolidRect(Vec2(pad, h - pad - cThick), Vec2(pad + cLen, h - pad), cColor);
    corners->drawSolidRect(Vec2(pad, h - pad - cLen), Vec2(pad + cThick, h - pad), cColor);
    corners->drawSolidRect(Vec2(w - pad - cLen, h - pad - cThick), Vec2(w - pad, h - pad), cColor);
    corners->drawSolidRect(Vec2(w - pad - cThick, h - pad - cLen), Vec2(w - pad, h - pad), cColor);
    corners->drawSolidRect(Vec2(pad, pad), Vec2(pad + cLen, pad + cThick), cColor);
    corners->drawSolidRect(Vec2(pad, pad), Vec2(pad + cThick, pad + cLen), cColor);
    corners->drawSolidRect(Vec2(w - pad - cLen, pad), Vec2(w - pad, pad + cThick), cColor);
    corners->drawSolidRect(Vec2(w - pad - cThick, pad), Vec2(w - pad, pad + cLen), cColor);
    addChild(corners, 0);

    auto ambient = AmbientParticles::create(size);
    addChild(ambient, 0);
    ambient->start();
}

void LevelMenuScene::drawTitle(const Size &size)
{
    auto title = Label::createWithTTF("SELECT LEVEL", FONT_TITLE, 44);
    title->setPosition(Vec2(size.width / 2, size.height - 50));
    title->setTextColor(Color4B(180, 220, 255, 255));
    title->enableShadow(Color4B(0, 40, 120, 180), Size(2, -2));
    addChild(title, 1);

    auto line = DrawNode::create();
    float lineW = 200.0f;
    float lineY = size.height - 78;
    float cx = size.width / 2;
    for (int i = 0; i < 3; ++i) {
        float offset = static_cast<float>(i);
        float alpha = 0.4f - 0.12f * offset;
        line->drawLine(Vec2(cx - lineW / 2, lineY - offset), Vec2(cx + lineW / 2, lineY - offset),
                       Color4F(0.3f, 0.55f, 1.0f, alpha));
    }
    line->drawSolidCircle(Vec2(cx - lineW / 2, lineY), 3.0f, 0, 8, Color4F(0.4f, 0.65f, 1.0f, 0.5f));
    line->drawSolidCircle(Vec2(cx + lineW / 2, lineY), 3.0f, 0, 8, Color4F(0.4f, 0.65f, 1.0f, 0.5f));
    addChild(line, 1);
}

void LevelMenuScene::createLevelButtons(const Size &size)
{
    const auto &levels = getAllLevels();
    _totalPages = (static_cast<int>(levels.size()) + PER_PAGE - 1) / PER_PAGE;
    _currentPage = 0;

    _pageContainer = Node::create();
    _pageContainer->setPosition(Vec2::ZERO);
    addChild(_pageContainer, 1);

    int startPage = std::max(0, std::min(s_initialPage, _totalPages - 1));
    showPage(startPage);
}

void LevelMenuScene::showPage(int page)
{
    _currentPage = page;
    s_initialPage = page;
    _pageContainer->removeAllChildren();

    auto size = Director::getInstance()->getVisibleSize();
    const auto &levels = getAllLevels();

    constexpr float btnW = 200.0f;
    constexpr float btnH = 80.0f;
    constexpr float gapX = 30.0f;
    constexpr float gapY = 20.0f;

    int start = page * PER_PAGE;
    int end = std::min(start + PER_PAGE, static_cast<int>(levels.size()));
    int count = end - start;

    int cols = COLS;
    int rows = (count + cols - 1) / cols;
    float totalW = cols * btnW + (cols - 1) * gapX;
    float totalH = rows * btnH + (rows - 1) * gapY;
    float startX = (size.width - totalW) / 2 + btnW / 2;
    float startY = (size.height - totalH) / 2 + totalH - btnH / 2 + 10;

    for (int i = 0; i < count; ++i) {
        int col = i % cols;
        int row = i / cols;
        float x = startX + col * (btnW + gapX);
        float y = startY - row * (btnH + gapY);
        int idx = start + i;
        createOneButton(_pageContainer, idx, levels[idx].id, levels[idx].name, x, y, btnW, btnH);
    }

    // Entry animation
    int animIdx = 0;
    for (auto child : _pageContainer->getChildren()) {
        if (child->getTag() == 999) {
            float delay = 0.03f * animIdx;
            child->setScale(0);
            child->runAction(Sequence::create(DelayTime::create(delay),
                                              EaseBackOut::create(ScaleTo::create(0.25f, 1.0f)), nullptr));
            ++animIdx;
        }
    }

    updateNavButtons();
}

void LevelMenuScene::createOneButton(Node *parent, int levelIdx, int levelId, const std::string &name,
                                     float x, float y, float btnW, float btnH)
{
    auto container = Node::create();
    container->setPosition(Vec2(x, y));
    container->setTag(999);
    parent->addChild(container, 1);

    auto glow = DrawNode::create();
    glow->drawSolidCircle(Vec2::ZERO, btnW * 0.35f, 0, 16, Color4F(0.2f, 0.4f, 0.8f, 0.06f));
    container->addChild(glow, -1);

    auto bg = DrawNode::create();
    constexpr int bStrips = 4;
    for (int i = 0; i < bStrips; ++i) {
        float t0 = static_cast<float>(i) / bStrips;
        float t1 = static_cast<float>(i + 1) / bStrips;
        float bright = 0.10f + 0.06f * (t0 + t1) / 2;
        bg->drawSolidRect(Vec2(-btnW / 2, -btnH / 2 + btnH * t0), Vec2(btnW / 2, -btnH / 2 + btnH * t1),
                          Color4F(bright, bright + 0.03f, bright + 0.12f, 0.92f));
    }
    bg->drawSolidRect(Vec2(-btnW / 2, btnH / 2 - 2), Vec2(btnW / 2, btnH / 2),
                      Color4F(0.35f, 0.55f, 0.95f, 0.4f));
    bg->drawRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2), Color4F(0.25f, 0.45f, 0.85f, 0.55f));
    container->addChild(bg, 0);

    auto numLabel = Label::createWithTTF(StringUtils::format("%d", levelId), FONT_TITLE, 28);
    numLabel->setPosition(Vec2(0, 10));
    numLabel->setTextColor(Color4B(200, 230, 255, 255));
    numLabel->enableShadow(Color4B(0, 30, 80, 120), Size(1, -1));
    container->addChild(numLabel, 1);

    auto nameLabel = Label::createWithTTF(name, FONT_UI, 14);
    nameLabel->setPosition(Vec2(0, -16));
    nameLabel->setTextColor(Color4B(150, 180, 220, 200));
    container->addChild(nameLabel, 1);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [x, y, btnW, btnH, container, glow](Touch *touch, Event *) {
        auto loc = container->getParent()->convertToNodeSpace(touch->getLocation());
        Rect rect(x - btnW / 2, y - btnH / 2, btnW, btnH);
        if (rect.containsPoint(loc)) {
            container->setScale(0.95f);
            glow->runAction(ScaleTo::create(0.1f, 1.5f));
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [levelIdx, container, glow](Touch *, Event *) {
        container->setScale(1.0f);
        glow->runAction(ScaleTo::create(0.1f, 1.0f));
        auto scene = GameScene::createSceneWithLevel(levelIdx);
        Director::getInstance()->replaceScene(TransitionFade::create(0.4f, scene, Color3B(10, 10, 30)));
    };
    listener->onTouchCancelled = [container, glow](Touch *, Event *) {
        container->setScale(1.0f);
        glow->runAction(ScaleTo::create(0.1f, 1.0f));
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, container);
}

void LevelMenuScene::createNavButtons(const Size &size)
{
    constexpr float arrowSize = 36.0f;
    constexpr float margin = 50.0f;
    float cy = 80.0f;

    // Previous page
    _prevBtn = Node::create();
    _prevBtn->setPosition(Vec2(margin, cy));
    addChild(_prevBtn, 2);

    auto prevBg = DrawNode::create();
    prevBg->drawSolidCircle(Vec2::ZERO, arrowSize, 0, 16, Color4F(0.12f, 0.14f, 0.22f, 0.9f));
    prevBg->drawCircle(Vec2::ZERO, arrowSize, 0, 16, false, Color4F(0.25f, 0.45f, 0.85f, 0.6f));
    _prevBtn->addChild(prevBg, 0);

    auto prevLabel = Label::createWithTTF("L", FONT_TITLE, 32);
    prevLabel->setTextColor(Color4B(180, 220, 255, 255));
    _prevBtn->addChild(prevLabel, 1);

    auto prevListener = EventListenerTouchOneByOne::create();
    prevListener->setSwallowTouches(true);
    prevListener->onTouchBegan = [this](Touch *touch, Event *) {
        if (_currentPage <= 0) return false;
        auto loc = _prevBtn->getParent()->convertToNodeSpace(touch->getLocation());
        if (loc.distance(_prevBtn->getPosition()) < 40.0f) {
            _prevBtn->setScale(0.9f);
            return true;
        }
        return false;
    };
    prevListener->onTouchEnded = [this](Touch *, Event *) {
        _prevBtn->setScale(1.0f);
        if (_currentPage > 0) showPage(_currentPage - 1);
    };
    prevListener->onTouchCancelled = [this](Touch *, Event *) { _prevBtn->setScale(1.0f); };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(prevListener, _prevBtn);

    // Next page
    _nextBtn = Node::create();
    _nextBtn->setPosition(Vec2(size.width - margin, cy));
    addChild(_nextBtn, 2);

    auto nextBg = DrawNode::create();
    nextBg->drawSolidCircle(Vec2::ZERO, arrowSize, 0, 16, Color4F(0.12f, 0.14f, 0.22f, 0.9f));
    nextBg->drawCircle(Vec2::ZERO, arrowSize, 0, 16, false, Color4F(0.25f, 0.45f, 0.85f, 0.6f));
    _nextBtn->addChild(nextBg, 0);

    auto nextLabel = Label::createWithTTF("R", FONT_TITLE, 32);
    nextLabel->setTextColor(Color4B(180, 220, 255, 255));
    _nextBtn->addChild(nextLabel, 1);

    auto nextListener = EventListenerTouchOneByOne::create();
    nextListener->setSwallowTouches(true);
    nextListener->onTouchBegan = [this](Touch *touch, Event *) {
        if (_currentPage >= _totalPages - 1) return false;
        auto loc = _nextBtn->getParent()->convertToNodeSpace(touch->getLocation());
        if (loc.distance(_nextBtn->getPosition()) < 40.0f) {
            _nextBtn->setScale(0.9f);
            return true;
        }
        return false;
    };
    nextListener->onTouchEnded = [this](Touch *, Event *) {
        _nextBtn->setScale(1.0f);
        if (_currentPage < _totalPages - 1) showPage(_currentPage + 1);
    };
    nextListener->onTouchCancelled = [this](Touch *, Event *) { _nextBtn->setScale(1.0f); };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(nextListener, _nextBtn);

    // Page number
    _pageLabel = Label::createWithTTF("1 / 1", FONT_UI, 20);
    _pageLabel->setPosition(Vec2(size.width / 2, cy));
    _pageLabel->setTextColor(Color4B(150, 180, 220, 200));
    addChild(_pageLabel, 2);

    updateNavButtons();
}

void LevelMenuScene::updateNavButtons()
{
    if (_pageLabel) {
        _pageLabel->setString(StringUtils::format("%d / %d", _currentPage + 1, _totalPages));
    }
    if (_prevBtn) {
        _prevBtn->setVisible(_currentPage > 0);
    }
    if (_nextBtn) {
        _nextBtn->setVisible(_currentPage < _totalPages - 1);
    }
}

void LevelMenuScene::createQuitButton(const Size &size)
{
    constexpr float btnW = 160.0f;
    constexpr float btnH = 50.0f;
    float x = size.width / 2;
    float y = 40.0f;

    auto container = Node::create();
    container->setPosition(Vec2(x, y));
    container->setTag(999);
    addChild(container, 1);

    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2),
                      Color4F(0.35f, 0.08f, 0.08f, 0.85f));
    bg->drawRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2), Color4F(0.75f, 0.25f, 0.25f, 0.6f));
    container->addChild(bg, 0);

    auto label = Label::createWithTTF("QUIT", FONT_TITLE, 24);
    label->setTextColor(Color4B(255, 180, 180, 255));
    container->addChild(label, 1);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [x, y, btnW, btnH, container](Touch *touch, Event *) {
        Rect rect(x - btnW / 2, y - btnH / 2, btnW, btnH);
        if (rect.containsPoint(touch->getLocation())) {
            container->setScale(0.93f);
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [container](Touch *, Event *) {
        container->setScale(1.0f);
        Director::getInstance()->end();
    };
    listener->onTouchCancelled = [container](Touch *, Event *) { container->setScale(1.0f); };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, container);
}
