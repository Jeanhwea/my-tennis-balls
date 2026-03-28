#include "LevelMenuScene.h"

#include "common/GameConstants.h"
#include "GameScene.h"
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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto &levels = getAllLevels();

    // 背景渐变
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2::ZERO, Vec2(visibleSize.width, visibleSize.height),
                      Color4F(0.05f, 0.05f, 0.12f, 1.0f));
    addChild(bg, -1);

    // 标题
    auto title = Label::createWithTTF("SELECT LEVEL", FONT_TITLE, 42);
    title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 60));
    title->setTextColor(Color4B(180, 220, 255, 255));
    title->enableShadow(Color4B(0, 40, 100, 150), Size(2, -2));
    addChild(title, 1);

    // 关卡按钮网格
    int cols = 4;
    int rows = (static_cast<int>(levels.size()) + cols - 1) / cols;
    float btnW = 200.0f;
    float btnH = 80.0f;
    float gapX = 30.0f;
    float gapY = 20.0f;
    float totalW = cols * btnW + (cols - 1) * gapX;
    float totalH = rows * btnH + (rows - 1) * gapY;
    float startX = (visibleSize.width - totalW) / 2 + btnW / 2;
    float startY = (visibleSize.height - totalH) / 2 + totalH - btnH / 2;

    for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
        const auto &level = levels[i];
        int col = i % cols;
        int row = i / cols;
        float x = startX + col * (btnW + gapX);
        float y = startY - row * (btnH + gapY);

        // 按钮背景
        auto btnBg = DrawNode::create();
        btnBg->drawSolidRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2),
                             Color4F(0.12f, 0.18f, 0.3f, 0.8f));
        btnBg->drawRect(Vec2(-btnW / 2, -btnH / 2), Vec2(btnW / 2, btnH / 2),
                        Color4F(0.3f, 0.5f, 0.9f, 0.5f));
        btnBg->setPosition(Vec2(x, y));
        addChild(btnBg, 0);

        // 关卡编号
        auto numLabel = Label::createWithTTF(StringUtils::format("%d", level.id), FONT_TITLE, 28);
        numLabel->setPosition(Vec2(x, y + 12));
        numLabel->setTextColor(Color4B(220, 240, 255, 255));
        addChild(numLabel, 1);

        // 关卡名称
        auto nameLabel = Label::createWithTTF(level.name, FONT_UI, 14);
        nameLabel->setPosition(Vec2(x, y - 16));
        nameLabel->setTextColor(Color4B(160, 190, 220, 180));
        addChild(nameLabel, 1);

        // 触摸监听
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        int levelIdx = i;

        listener->onTouchBegan = [x, y, btnW, btnH](Touch *touch, Event *) {
            auto loc = touch->getLocation();
            Rect rect(x - btnW / 2, y - btnH / 2, btnW, btnH);
            return rect.containsPoint(loc);
        };

        listener->onTouchEnded = [levelIdx, btnBg](Touch *, Event *) {
            // 按钮点击动画
            btnBg->runAction(Sequence::create(
                ScaleTo::create(0.05f, 0.95f), ScaleTo::create(0.05f, 1.0f),
                CallFunc::create([levelIdx]() {
                    auto scene = GameScene::createSceneWithLevel(levelIdx);
                    Director::getInstance()->replaceScene(
                        TransitionFade::create(0.3f, scene, Color3B(10, 10, 30)));
                }),
                nullptr));
        };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, btnBg);
    }
}
