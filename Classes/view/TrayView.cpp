#include "TrayView.h"

#include "GameConstants.h"

USING_NS_CC;

namespace
{

Color3B targetColor(int index)
{
    static const Color3B colors[] = {
        Color3B(255, 60, 60),   Color3B(60, 220, 255),  Color3B(255, 200, 40), Color3B(60, 255, 130),
        Color3B(255, 120, 220), Color3B(180, 100, 255), Color3B(255, 160, 80), Color3B(100, 255, 200),
    };
    return colors[index % 8];
}

void createOneTray(Node *parent, const Size &visibleSize, const TrayData &tray, int &targetIndex)
{
    float trayX = visibleSize.width * tray.x;
    float trayY = visibleSize.height * tray.y;
    float trayW = visibleSize.width * tray.width;

    // Visual shelf
    auto draw = DrawNode::create();
    draw->drawSolidRect(Vec2(trayX - trayW / 2, trayY - TRAY_THICKNESS / 2),
                        Vec2(trayX + trayW / 2, trayY + TRAY_THICKNESS / 2),
                        Color4F(0.6f, 0.6f, 0.7f, 0.9f));
    draw->drawLine(Vec2(trayX - trayW / 2, trayY + TRAY_THICKNESS / 2),
                   Vec2(trayX + trayW / 2, trayY + TRAY_THICKNESS / 2), Color4F::WHITE);
    draw->setTag(TAG_TRAY);
    parent->addChild(draw, 2);

    // Physics shelf
    auto trayNode = Node::create();
    trayNode->setName("tray");
    trayNode->setTag(TAG_TRAY);
    trayNode->setPosition(Vec2(trayX, trayY));

    auto body = PhysicsBody::createBox(Size(trayW, TRAY_THICKNESS), PhysicsMaterial(1.0f, 0.2f, 0.8f));
    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_TRAY);
    body->setCollisionBitmask(CATEGORY_TARGET | CATEGORY_BALL);
    body->setContactTestBitmask(CATEGORY_ALL);
    trayNode->setPhysicsBody(body);
    parent->addChild(trayNode, 2);

    // Target balls
    float spacing = trayW / (tray.targets + 1);
    float startX = trayX - trayW / 2;
    float ballY = trayY + TRAY_THICKNESS / 2 + 20.0f;

    for (int i = 0; i < tray.targets; ++i) {
        float bx = startX + spacing * (i + 1);

        auto target = Sprite::create("ball.png");
        target->setScale(TARGET_SCALE);
        target->setPosition(Vec2(bx, ballY));
        target->setTag(TAG_TARGET);
        target->setColor(targetColor(targetIndex));
        target->setName(StringUtils::format("target%02d", targetIndex));
        ++targetIndex;

        const float radius = target->getContentSize().width / 2 - BALL_SPRITE_PADDING;
        auto tbody = PhysicsBody::createCircle(
            radius, PhysicsMaterial(TARGET_DENSITY, TARGET_RESTITUTION, TARGET_FRICTION));
        tbody->setCategoryBitmask(CATEGORY_TARGET);
        tbody->setCollisionBitmask(CATEGORY_TRAY | CATEGORY_BALL | CATEGORY_TARGET);
        tbody->setContactTestBitmask(CATEGORY_ALL);
        target->setPhysicsBody(tbody);
        parent->addChild(target, 5);
    }
}

}  // namespace

int TrayView::createFromLevel(Node *parent, const Size &visibleSize, const LevelData &level)
{
    int targetIndex = 0;
    for (const auto &tray : level.trays) {
        createOneTray(parent, visibleSize, tray, targetIndex);
    }
    return targetIndex;
}
