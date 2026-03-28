#include "LevelMenuScene.h"

#include "GameScene.h"
#include "common/GameConstants.h"
#include "model/LevelData.h"
#include "view/AmbientParticles.h"

USING_NS_CC;

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
    createQuitButton(size);
}

void LevelMenuScene::drawBackground(const Size &size)
{
    // 渐变背景（深蓝到深紫）
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

    // 装饰网格
    auto grid = DrawNode::create();
    Color4F gridColor(0.12f, 0.15f, 0.25f, 0.08f);
    constexpr float spacing = 80.0f;
    for (float x = 0; x <= size.width; x += spacing)
        grid->drawLine(Vec2(x, 0), Vec2(x, size.height), gridColor);
    for (float y = 0; y <= size.height; y += spacing)
        grid->drawLine(Vec2(0, y), Vec2(size.width, y), gridColor);
    addChild(grid, -1);

    // 角落装饰
    auto corners = DrawNode::create();
    Color4F cColor(0.25f, 0.45f, 0.85f, 0.25f);
    constexpr float cLen = 40.0f;
    constexpr float cThick = 2.0f;
    constexpr float pad = 8.0f;
    float w = size.width;
    float h = size.height;

    // 四角 L 形
    corners->drawSolidRect(Vec2(pad, h - pad - cThick), Vec2(pad + cLen, h - pad), cColor);
    corners->drawSolidRect(Vec2(pad, h - pad - cLen), Vec2(pad + cThick, h - pad), cColor);
    corners->drawSolidRect(Vec2(w - pad - cLen, h - pad - cThick), Vec2(w - pad, h - pad), cColor);
    corners->drawSolidRect(Vec2(w - pad - cThick, h - pad - cLen), Vec2(w - pad, h - pad), cColor);
    corners->drawSolidRect(Vec2(pad, pad), Vec2(pad + cLen, pad + cThick), cColor);
    corners->drawSolidRect(Vec2(pad, pad), Vec2(pad + cThick, pad + cLen), cColor);
    corners->drawSolidRect(Vec2(w - pad - cLen, pad), Vec2(w - pad, pad + cThick), cColor);
    corners->drawSolidRect(Vec2(w - pad - cThick, pad), Vec2(w - pad, pad + cLen), cColor);
    addChild(corners, 0);

    CCLOG("[LevelMenuScene] drawBackground: ambient particles");
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

    // 标题下方装饰线
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
    // 两端光点
    line->drawSolidCircle(Vec2(cx - lineW / 2, lineY), 3.0f, 0, 8, Color4F(0.4f, 0.65f, 1.0f, 0.5f));
    line->drawSolidCircle(Vec2(cx + lineW / 2, lineY), 3.0f, 0, 8, Color4F(0.4f, 0.65f, 1.0f, 0.5f));
    addChild(line, 1);
}

Vec2 LevelMenuScene::calcGridOrigin(const Size &size, int count, int cols, float btnW, float btnH,
                                    float gapX, float gapY) const
{
    int rows = (count + cols - 1) / cols;
    float totalW = cols * btnW + (cols - 1) * gapX;
    float totalH = rows * btnH + (rows - 1) * gapY;
    float startX = (size.width - totalW) / 2 + btnW / 2;
    float startY = (size.height - totalH) / 2 + totalH - btnH / 2;
    return Vec2(startX, startY);
}

void LevelMenuScene::createLevelButtons(const Size &size)
{
    const auto &levels = getAllLevels();
    constexpr int cols = 4;
    constexpr float btnW = 200.0f;
    constexpr float btnH = 80.0f;
    constexpr float gapX = 30.0f;
    constexpr float gapY = 20.0f;

    auto origin = calcGridOrigin(size, static_cast<int>(levels.size()), cols, btnW, btnH, gapX, gapY);

    for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
        int col = i % cols;
        int row = i / cols;
        float x = origin.x + col * (btnW + gapX);
        float y = origin.y - row * (btnH + gapY);
        createOneButton(i, levels[i].id, levels[i].name, x, y, btnW, btnH);
    }

    // 按钮交错入场动画
    int idx = 0;
    for (auto child : getChildren()) {
        if (child->getTag() == 999) {
            float delay = 0.05f * idx;
            child->setScale(0);
            child->runAction(Sequence::create(DelayTime::create(delay),
                                              EaseBackOut::create(ScaleTo::create(0.3f, 1.0f)), nullptr));
            ++idx;
        }
    }
}

void LevelMenuScene::createOneButton(int levelIdx, int levelId, const std::string &name, float x, float y,
                                     float btnW, float btnH)
{
    // 按钮容器节点（用于入场动画）
    auto container = Node::create();
    container->setPosition(Vec2(x, y));
    container->setTag(999);
    addChild(container, 1);

    // 按钮背后柔光
    auto glow = DrawNode::create();
    glow->drawSolidCircle(Vec2::ZERO, btnW * 0.35f, 0, 16, Color4F(0.2f, 0.4f, 0.8f, 0.06f));
    container->addChild(glow, -1);

    auto bg = DrawNode::create();

    // 按钮主体渐变（从下到上微亮）
    constexpr int bStrips = 4;
    for (int i = 0; i < bStrips; ++i) {
        float t0 = static_cast<float>(i) / bStrips;
        float t1 = static_cast<float>(i + 1) / bStrips;
        float bright = 0.10f + 0.06f * (t0 + t1) / 2;
        bg->drawSolidRect(Vec2(-btnW / 2, -btnH / 2 + btnH * t0), Vec2(btnW / 2, -btnH / 2 + btnH * t1),
                          Color4F(bright, bright + 0.03f, bright + 0.12f, 0.92f));
    }

    // 顶部高光线
    bg->drawSolidRect(Vec2(-btnW / 2, btnH / 2 - 2), Vec2(btnW / 2, btnH / 2),
                      Color4F(0.35f, 0.55f, 0.95f, 0.4f));
    // 边框
    bg->drawRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2), Color4F(0.25f, 0.45f, 0.85f, 0.55f));
    container->addChild(bg, 0);

    // 关卡编号（大字）
    auto numLabel = Label::createWithTTF(StringUtils::format("%d", levelId), FONT_TITLE, 28);
    numLabel->setPosition(Vec2(0, 10));
    numLabel->setTextColor(Color4B(200, 230, 255, 255));
    numLabel->enableShadow(Color4B(0, 30, 80, 120), Size(1, -1));
    container->addChild(numLabel, 1);

    // 关卡名称（小字）
    auto nameLabel = Label::createWithTTF(name, FONT_UI, 14);
    nameLabel->setPosition(Vec2(0, -16));
    nameLabel->setTextColor(Color4B(150, 180, 220, 200));
    container->addChild(nameLabel, 1);

    // 触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [x, y, btnW, btnH, container, glow](Touch *touch, Event *) {
        Rect rect(x - btnW / 2, y - btnH / 2, btnW, btnH);
        if (rect.containsPoint(touch->getLocation())) {
            container->setScale(0.95f);
            // 按下时发光增强
            glow->runAction(ScaleTo::create(0.1f, 1.5f));
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [this, levelIdx, container, glow](Touch *, Event *) {
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

void LevelMenuScene::createQuitButton(const Size &size)
{
    constexpr float btnW = 160.0f;
    constexpr float btnH = 50.0f;
    float x = size.width / 2;
    float y = 40.0f;

    auto container = Node::create();
    container->setPosition(Vec2(x, y));
    container->setTag(999);  // 入场动画复用
    addChild(container, 1);

    // 按钮背景
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2),
                      Color4F(0.35f, 0.08f, 0.08f, 0.85f));
    bg->drawRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2), Color4F(0.75f, 0.25f, 0.25f, 0.6f));
    container->addChild(bg, 0);

    // 标签
    auto label = Label::createWithTTF("QUIT", FONT_TITLE, 24);
    label->setTextColor(Color4B(255, 180, 180, 255));
    container->addChild(label, 1);

    // 触摸
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
