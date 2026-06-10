#include "LevelButtonGrid.h"

#include "common/GameConstants.h"
#include "scene/GameScene.h"

USING_NS_CC;

LevelButtonGrid *LevelButtonGrid::create(Node *parent, const Size &visibleSize, int startPage)
{
    auto ret = new (std::nothrow) LevelButtonGrid();
    if (ret && ret->init(parent, visibleSize, startPage)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool LevelButtonGrid::init(Node *parent, const Size &visibleSize, int startPage)
{
    if (!Node::init()) return false;

    _pageContainer = Node::create();
    _pageContainer->setPosition(Vec2::ZERO);
    addChild(_pageContainer, 1);

    const auto &levels = getAllLevels();
    _totalPages = (static_cast<int>(levels.size()) + PER_PAGE - 1) / PER_PAGE;
    _currentPage = 0;

    int start = std::max(0, std::min(startPage, _totalPages - 1));
    showPage(parent, visibleSize, start);

    return true;
}

void LevelButtonGrid::showPage(Node *parent, const Size &size, int page)
{
    _currentPage = page;
    _pageContainer->removeAllChildren();

    const auto &levels = getAllLevels();

    int start = page * PER_PAGE;
    int end = std::min(start + PER_PAGE, static_cast<int>(levels.size()));
    int count = end - start;

    int rows = (count + COLS - 1) / COLS;
    float totalW = COLS * BTN_W + (COLS - 1) * GAP_X;
    float totalH = rows * BTN_H + (rows - 1) * GAP_Y;
    float originX = (size.width - totalW) / 2 + BTN_W / 2;
    float originY = (size.height - totalH) / 2 + totalH - BTN_H / 2 + 10;

    for (int i = 0; i < count; ++i) {
        int col = i % COLS;
        int row = i / COLS;
        float x = originX + col * (BTN_W + GAP_X);
        float y = originY - row * (BTN_H + GAP_Y);
        int idx = start + i;
        createOneButton(parent, _pageContainer, idx, levels[idx].id, levels[idx].name, x, y);
    }

    int animIdx = 0;
    for (auto child : _pageContainer->getChildren()) {
        if (child->getTag() == TAG_BUTTON) {
            float delay = 0.03f * animIdx;
            child->setScale(0);
            child->runAction(Sequence::create(DelayTime::create(delay),
                                              EaseBackOut::create(ScaleTo::create(0.25f, 1.0f)), nullptr));
            ++animIdx;
        }
    }

    if (_onPageChanged) _onPageChanged(_currentPage, _totalPages);
}

void LevelButtonGrid::createOneButton(Node *scene, Node *container, int levelIndex, int levelId,
                                      const std::string &name, float x, float y)
{
    auto btn = Node::create();
    btn->setPosition(Vec2(x, y));
    btn->setTag(TAG_BUTTON);
    container->addChild(btn, 1);

    auto glow = DrawNode::create();
    glow->drawSolidCircle(Vec2::ZERO, BTN_W * 0.35f, 0, 16, Color4F(0.2f, 0.4f, 0.8f, 0.06f));
    btn->addChild(glow, -1);

    auto bg = DrawNode::create();
    for (int i = 0; i < 4; ++i) {
        float t0 = static_cast<float>(i) / 4;
        float t1 = static_cast<float>(i + 1) / 4;
        float bright = 0.10f + 0.06f * (t0 + t1) / 2;
        bg->drawSolidRect(Vec2(-BTN_W / 2, -BTN_H / 2 + BTN_H * t0),
                          Vec2(BTN_W / 2, -BTN_H / 2 + BTN_H * t1),
                          Color4F(bright, bright + 0.03f, bright + 0.12f, 0.92f));
    }
    bg->drawSolidRect(Vec2(-BTN_W / 2, BTN_H / 2 - 2), Vec2(BTN_W / 2, BTN_H / 2),
                      Color4F(0.35f, 0.55f, 0.95f, 0.4f));
    bg->drawRect(Vec2(-BTN_W / 2, -BTN_H / 2), Vec2(BTN_W / 2, BTN_H / 2),
                 Color4F(0.25f, 0.45f, 0.85f, 0.55f));
    btn->addChild(bg, 0);

    auto numLabel = Label::createWithTTF(StringUtils::format("%d", levelId), FONT_TITLE, 28);
    numLabel->setPosition(Vec2(0, 10));
    numLabel->setTextColor(Color4B(200, 230, 255, 255));
    numLabel->enableShadow(Color4B(0, 30, 80, 120), Size(1, -1));
    btn->addChild(numLabel, 1);

    auto nameLabel = Label::createWithTTF(name, FONT_UI, 14);
    nameLabel->setPosition(Vec2(0, -16));
    nameLabel->setTextColor(Color4B(150, 180, 220, 200));
    btn->addChild(nameLabel, 1);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [x, y, btn, glow](Touch *touch, Event *) {
        auto loc = btn->getParent()->convertToNodeSpace(touch->getLocation());
        Rect rect(x - BTN_W / 2, y - BTN_H / 2, BTN_W, BTN_H);
        if (rect.containsPoint(loc)) {
            btn->setScale(0.95f);
            glow->runAction(ScaleTo::create(0.1f, 1.5f));
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [scene, levelIndex, btn, glow](Touch *, Event *) {
        btn->setScale(1.0f);
        glow->runAction(ScaleTo::create(0.1f, 1.0f));
        auto newScene = GameScene::createSceneWithLevel(levelIndex);
        Director::getInstance()->replaceScene(TransitionFade::create(0.4f, newScene, Color3B(10, 10, 30)));
    };
    listener->onTouchCancelled = [btn, glow](Touch *, Event *) {
        btn->setScale(1.0f);
        glow->runAction(ScaleTo::create(0.1f, 1.0f));
    };
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, btn);
}
