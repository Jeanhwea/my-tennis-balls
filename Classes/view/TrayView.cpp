#include "TrayView.h"

#include "common/GameConstants.h"

USING_NS_CC;

namespace
{

void drawTrayVisual(Node *parent, float trayX, float trayY, float trayW)
{
    auto draw = DrawNode::create();
    float halfW = trayW / 2;
    float halfH = TRAY_THICKNESS / 2;

    // 阴影
    draw->drawSolidRect(Vec2(trayX - halfW + 3, trayY - halfH - 3),
                        Vec2(trayX + halfW + 3, trayY + halfH - 3), Color4F(0.0f, 0.0f, 0.0f, 0.25f));

    // 主体
    static constexpr int T_STRIPS = 3;
    for (int i = 0; i < T_STRIPS; ++i) {
        float t0 = static_cast<float>(i) / T_STRIPS;
        float t1 = static_cast<float>(i + 1) / T_STRIPS;
        float bright = 0.38f + 0.18f * (t0 + t1) / 2;
        draw->drawSolidRect(Vec2(trayX - halfW, trayY - halfH + TRAY_THICKNESS * t0),
                            Vec2(trayX + halfW, trayY - halfH + TRAY_THICKNESS * t1),
                            Color4F(bright - 0.05f, bright, bright + 0.08f, 0.95f));
    }

    // 高光
    draw->drawSolidRect(Vec2(trayX - halfW + 1, trayY + halfH - 2), Vec2(trayX + halfW - 1, trayY + halfH),
                        Color4F(0.75f, 0.80f, 0.92f, 0.85f));

    // 暗线
    draw->drawLine(Vec2(trayX - halfW, trayY - halfH), Vec2(trayX + halfW, trayY - halfH),
                   Color4F(0.22f, 0.24f, 0.30f, 0.9f));

    // 端盖
    draw->drawSolidCircle(Vec2(trayX - halfW, trayY), halfH, 0, 8, Color4F(0.50f, 0.54f, 0.62f, 0.7f));
    draw->drawSolidCircle(Vec2(trayX + halfW, trayY), halfH, 0, 8, Color4F(0.38f, 0.40f, 0.48f, 0.7f));

    // 支架
    static constexpr float BRACKET_H = 12.0f;
    static constexpr float BRACKET_W = 6.0f;
    Color4F bracketColor(0.30f, 0.35f, 0.50f, 0.6f);
    Vec2 lb[3] = {Vec2(trayX - halfW + 2, trayY - halfH),
                  Vec2(trayX - halfW + 2, trayY - halfH - BRACKET_H),
                  Vec2(trayX - halfW + 2 + BRACKET_W, trayY - halfH)};
    draw->drawSolidPoly(lb, 3, bracketColor);
    Vec2 rb[3] = {Vec2(trayX + halfW - 2, trayY - halfH),
                  Vec2(trayX + halfW - 2, trayY - halfH - BRACKET_H),
                  Vec2(trayX + halfW - 2 - BRACKET_W, trayY - halfH)};
    draw->drawSolidPoly(rb, 3, bracketColor);

    draw->setTag(TAG_TRAY);
    parent->addChild(draw, 2);

    // 底部柔光
    auto glow = DrawNode::create();
    for (int g = 5; g >= 0; --g) {
        float gy = static_cast<float>(g) * 2.5f;
        float alpha = 0.025f * (6 - g);
        glow->drawSolidRect(Vec2(trayX - halfW + 6, trayY - halfH - gy - 2.5f),
                            Vec2(trayX + halfW - 6, trayY - halfH - gy),
                            Color4F(0.25f, 0.45f, 0.85f, alpha));
    }
    glow->setTag(TAG_TRAY);
    parent->addChild(glow, 1);
}

void createTrayPhysics(Node *parent, float trayX, float trayY, float trayW)
{
    auto trayNode = Node::create();
    trayNode->setName("tray");
    trayNode->setTag(TAG_TRAY);
    trayNode->setPosition(Vec2(trayX, trayY));

    auto body = PhysicsBody::createBox(Size(trayW, TRAY_THICKNESS),
                                       PhysicsMaterial(TRAY_DENSITY, TRAY_RESTITUTION, TRAY_FRICTION));
    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_TRAY);
    body->setCollisionBitmask(CATEGORY_TARGET | CATEGORY_BALL);
    body->setContactTestBitmask(CATEGORY_BALL | CATEGORY_TARGET);
    trayNode->setPhysicsBody(body);
    parent->addChild(trayNode, 2);
}

void spawnTargets(Node *parent, float trayX, float trayY, float trayW, int count, int &targetIndex,
                  Vector<Node *> &outTargets)
{
    float spacing = trayW / (count + 1);
    float startX = trayX - trayW / 2;
    float ballY = trayY + TRAY_THICKNESS / 2 + 20.0f;

    for (int i = 0; i < count; ++i) {
        float bx = startX + spacing * (i + 1);

        auto target = Sprite::create("ball.png");
        target->setScale(TARGET_SCALE);
        target->setPosition(Vec2(bx, ballY));
        target->setTag(TAG_TARGET);
        target->setName(StringUtils::format("target%02d", targetIndex));
        ++targetIndex;

        const float radius = target->getContentSize().width / 2 - BALL_SPRITE_PADDING;
        auto tbody = PhysicsBody::createCircle(
            radius, PhysicsMaterial(TARGET_DENSITY, TARGET_RESTITUTION, TARGET_FRICTION));
        tbody->setCategoryBitmask(CATEGORY_TARGET);
        tbody->setCollisionBitmask(CATEGORY_EDGE | CATEGORY_TRAY | CATEGORY_BALL | CATEGORY_TARGET |
                                   CATEGORY_OBSTACLE);
        tbody->setContactTestBitmask(CATEGORY_BALL | CATEGORY_FLOOR);
        target->setPhysicsBody(tbody);
        parent->addChild(target, 5);

        outTargets.pushBack(target);
    }
}

void createOneTray(Node *parent, const Size &visibleSize, const TrayData &tray, int &targetIndex,
                   Vector<Node *> &outTargets)
{
    float trayX = visibleSize.width * tray.x;
    float trayY = visibleSize.height * tray.y;
    float trayW = visibleSize.width * tray.width;

    drawTrayVisual(parent, trayX, trayY, trayW);
    createTrayPhysics(parent, trayX, trayY, trayW);
    spawnTargets(parent, trayX, trayY, trayW, tray.targets, targetIndex, outTargets);
}

}  // namespace

int TrayView::createFromLevel(Node *parent, const Size &visibleSize, const LevelData &level,
                              Vector<Node *> &outTargets)
{
    int targetIndex = 0;
    for (const auto &tray : level.trays) {
        createOneTray(parent, visibleSize, tray, targetIndex, outTargets);
    }
    return targetIndex;
}
