#include "TrayView.h"

#include "GameConstants.h"

USING_NS_CC;

namespace
{

Color3B targetColor(int index)
{
    static const Color3B colors[] = {
        Color3B(255, 60, 60),  Color3B(60, 220, 255),  Color3B(255, 200, 40),
        Color3B(60, 255, 130), Color3B(255, 120, 220),
    };
    return colors[index % 5];
}

}  // namespace

void TrayView::create(Node *parent, const Size &visibleSize, int targetCount)
{
    float trayX = visibleSize.width * TRAY_X;
    float trayY = visibleSize.height * TRAY_Y;
    float trayW = visibleSize.width * TRAY_WIDTH_RATIO;

    // Draw tray shelf visual
    auto trayDraw = DrawNode::create();
    trayDraw->drawSolidRect(Vec2(trayX - trayW / 2, trayY - TRAY_THICKNESS / 2),
                            Vec2(trayX + trayW / 2, trayY + TRAY_THICKNESS / 2),
                            Color4F(0.6f, 0.6f, 0.7f, 0.9f));
    // Shelf edge highlight
    trayDraw->drawLine(Vec2(trayX - trayW / 2, trayY + TRAY_THICKNESS / 2),
                       Vec2(trayX + trayW / 2, trayY + TRAY_THICKNESS / 2), Color4F::WHITE);
    parent->addChild(trayDraw, 2);

    // Tray physics body (static)
    auto trayNode = Node::create();
    trayNode->setName("tray");
    trayNode->setTag(TAG_TRAY);
    trayNode->setPosition(Vec2(trayX, trayY));

    auto trayBody = PhysicsBody::createBox(Size(trayW, TRAY_THICKNESS), PhysicsMaterial(1.0f, 0.2f, 0.8f));
    trayBody->setDynamic(false);
    trayBody->setCategoryBitmask(CATEGORY_TRAY);
    trayBody->setCollisionBitmask(CATEGORY_TARGET | CATEGORY_BALL);
    trayBody->setContactTestBitmask(CATEGORY_ALL);
    trayNode->setPhysicsBody(trayBody);
    parent->addChild(trayNode, 2);

    // Spawn target balls on top of tray
    float spacing = trayW / (targetCount + 1);
    float startX = trayX - trayW / 2;
    float ballY = trayY + TRAY_THICKNESS / 2 + 20.0f;

    for (int i = 0; i < targetCount; ++i) {
        float bx = startX + spacing * (i + 1);

        auto target = Sprite::create("ball.png");
        target->setScale(TARGET_SCALE);
        target->setPosition(Vec2(bx, ballY));
        target->setTag(TAG_TARGET);
        target->setColor(targetColor(i));
        target->setName(StringUtils::format("target%02d", i));

        const float radius = target->getContentSize().width / 2 - BALL_SPRITE_PADDING;
        auto body = PhysicsBody::createCircle(
            radius, PhysicsMaterial(TARGET_DENSITY, TARGET_RESTITUTION, TARGET_FRICTION));
        body->setCategoryBitmask(CATEGORY_TARGET);
        body->setCollisionBitmask(CATEGORY_TRAY | CATEGORY_BALL | CATEGORY_TARGET | CATEGORY_EDGE);
        body->setContactTestBitmask(CATEGORY_ALL);
        target->setPhysicsBody(body);
        parent->addChild(target, 5);
    }
}
