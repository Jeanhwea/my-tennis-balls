#include "LevelMenuScene.h"

#include "GameScene.h"
#include "common/GameConstants.h"
#include "model/LevelData.h"

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
}

void LevelMenuScene::drawBackground(const Size &size)
{
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2::ZERO, Vec2(size.width, size.height), Color4F(0.05f, 0.05f, 0.12f, 1.0f));
    addChild(bg, -1);
}

void LevelMenuScene::drawTitle(const Size &size)
{
    auto title = Label::createWithTTF("SELECT LEVEL", FONT_TITLE, 42);
    title->setPosition(Vec2(size.width / 2, size.height - 60));
    title->setTextColor(Color4B(180, 220, 255, 255));
    title->enableShadow(Color4B(0, 40, 100, 150), Size(2, -2));
    addChild(title, 1);
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
}

void LevelMenuScene::createOneButton(int levelIdx, int levelId, const std::string &name, float x, float y,
                                     float btnW, float btnH)
{
    // 按钮背景
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2),
                      Color4F(0.12f, 0.15f, 0.25f, 0.9f));
    bg->drawRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2), Color4F(0.3f, 0.5f, 0.9f, 0.6f));
    bg->setPosition(Vec2(x, y));
    addChild(bg, 1);

    // 关卡标题
    auto label = Label::createWithTTF(StringUtils::format("%d - %s", levelId, name.c_str()), FONT_UI, 18);
    label->setPosition(Vec2(x, y));
    label->setTextColor(Color4B(200, 220, 255, 255));
    addChild(label, 2);

    // 触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [x, y, btnW, btnH](Touch *touch, Event *) {
        auto pos = touch->getLocation();
        Rect rect(x - btnW / 2, y - btnH / 2, btnW, btnH);
        return rect.containsPoint(pos);
    };
    listener->onTouchEnded = [this, levelIdx](Touch *, Event *) {
        auto scene = GameScene::createSceneWithLevel(levelIdx);
        Director::getInstance()->replaceScene(TransitionFade::create(0.4f, scene, Color3B(10, 10, 30)));
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bg);
}
