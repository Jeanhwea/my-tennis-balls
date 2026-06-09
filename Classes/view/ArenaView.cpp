#include "ArenaView.h"

#include "common/GameConstants.h"
#include "view/AmbientParticles.h"
#include "view/VisualUtil.h"

USING_NS_CC;

namespace
{

void drawBackground(Node *parent, const Size &size)
{
    VisualUtil::drawGradientBackground(parent, size,
        {Color4F(0.04f, 0.04f, 0.10f, 1.0f), Color4F(0.08f, 0.06f, 0.16f, 1.0f), 20}, -10);
}

void drawGrid(Node *parent, const Size &size, float launchLeft)
{
    VisualUtil::drawGrid(parent, size, {60.0f, Color4F(0.15f, 0.18f, 0.28f, 0.15f), launchLeft}, -9);
}

void drawCorners(Node *parent, const Size &size, float launchLeft)
{
    VisualUtil::drawCorners(parent, size, {30.0f, 2.0f, 4.0f, Color4F(0.3f, 0.5f, 0.9f, 0.35f), launchLeft}, -7);
}

}  // namespace

void ArenaView::addEdgeWalls(Node *parent, const Size &visibleSize)
{
    auto edgeNode = Node::create();
    edgeNode->setName("edgeWalls");
    edgeNode->setPosition(Vec2::ZERO);

    auto body = PhysicsBody::create();
    body->setDynamic(false);

    float w = visibleSize.width;
    float h = visibleSize.height;

    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(0, 0), Vec2(0, h), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));
    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(w, 0), Vec2(w, h), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));
    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(0, h), Vec2(w, h), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));
    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(0, 0), Vec2(w, 0), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));

    body->setCategoryBitmask(CATEGORY_EDGE);
    body->setCollisionBitmask(CATEGORY_BALL | CATEGORY_TARGET | CATEGORY_TRAY | CATEGORY_OBSTACLE);
    body->setContactTestBitmask(CATEGORY_BALL | CATEGORY_TARGET);
    edgeNode->setPhysicsBody(body);
    parent->addChild(edgeNode);

    auto wallDraw = DrawNode::create();
    Color4F wallColor(0.2f, 0.4f, 0.7f, 0.4f);
    wallDraw->drawLine(Vec2(0, 0), Vec2(0, h), wallColor);
    wallDraw->drawLine(Vec2(w, 0), Vec2(w, h), wallColor);
    wallDraw->drawLine(Vec2(0, h), Vec2(w, h), wallColor);
    wallDraw->drawLine(Vec2(0, 0), Vec2(w, 0), wallColor);
    parent->addChild(wallDraw, 1);
}

void ArenaView::addFloorSensor(Node *parent, const Size &visibleSize)
{
    auto floor = Node::create();
    floor->setName("floor");
    floor->setTag(TAG_FLOOR);
    floor->setPosition(Vec2(visibleSize.width / 2, -200.0f));

    auto body = PhysicsBody::createBox(Size(visibleSize.width + 200, 400.0f), PhysicsMaterial(0, 0, 0));
    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_FLOOR);
    body->setCollisionBitmask(0);
    body->setContactTestBitmask(CATEGORY_BALL | CATEGORY_TARGET);
    floor->setPhysicsBody(body);
    parent->addChild(floor);
}

void ArenaView::drawZones(Node *parent, const Size &visibleSize)
{
    float w = visibleSize.width;
    float h = visibleSize.height;
    float launchLeft = w * (1.0f - LAUNCH_ZONE_RATIO);

    drawBackground(parent, visibleSize);

    drawGrid(parent, visibleSize, launchLeft);

    auto zone = DrawNode::create();

    static constexpr int ZONE_STRIPS = 8;
    for (int i = 0; i < ZONE_STRIPS; ++i) {
        float t0 = static_cast<float>(i) / ZONE_STRIPS;
        float t1 = static_cast<float>(i + 1) / ZONE_STRIPS;
        float x0 = launchLeft + (w - launchLeft) * t0;
        float x1 = launchLeft + (w - launchLeft) * t1;
        float alpha = 0.08f + 0.12f * t0;
        zone->drawSolidRect(Vec2(x0, 0), Vec2(x1, h), Color4F(0.15f, 0.25f, 0.5f, alpha));
    }

    for (int i = 3; i >= 0; --i) {
        float offset = static_cast<float>(i) * 2.0f;
        float alpha = 0.1f + 0.15f * (3 - i);
        zone->drawLine(Vec2(launchLeft - offset, 0), Vec2(launchLeft - offset, h),
                       Color4F(0.4f, 0.6f, 1.0f, alpha));
    }
    zone->drawLine(Vec2(launchLeft, 0), Vec2(launchLeft, h), Color4F(0.5f, 0.75f, 1.0f, 0.7f));

    parent->addChild(zone, -8);

    drawCorners(parent, visibleSize, launchLeft);

    auto vignette = DrawNode::create();
    float vSize = 80.0f;
    for (int i = 0; i < 8; ++i) {
        float t = static_cast<float>(i) / 8;
        float alpha = 0.15f * (1.0f - t);
        vignette->drawSolidRect(Vec2(0, vSize * t), Vec2(launchLeft, vSize * (t + 1.0f / 8)),
                                Color4F(0, 0, 0, alpha));
    }
    for (int i = 0; i < 8; ++i) {
        float t = static_cast<float>(i) / 8;
        float alpha = 0.15f * t;
        vignette->drawSolidRect(Vec2(0, h - vSize + vSize * t),
                                Vec2(launchLeft, h - vSize + vSize * (t + 1.0f / 8)),
                                Color4F(0, 0, 0, alpha));
    }
    parent->addChild(vignette, -6);

    auto label = Label::createWithTTF("LAUNCH", FONT_UI, 14);
    label->setRotation(-90);
    label->setPosition(Vec2(launchLeft + (w - launchLeft) / 2, h / 2));
    label->setTextColor(Color4B(100, 160, 255, 60));
    parent->addChild(label, 0);

    auto reticle = DrawNode::create();
    float rcx = launchLeft + (w - launchLeft) / 2;
    float rcy = h * 0.3f;
    float rSize = 18.0f;
    Color4F retColor(0.4f, 0.6f, 1.0f, 0.15f);
    reticle->drawLine(Vec2(rcx - rSize, rcy), Vec2(rcx + rSize, rcy), retColor);
    reticle->drawLine(Vec2(rcx, rcy - rSize), Vec2(rcx, rcy + rSize), retColor);
    reticle->drawCircle(Vec2(rcx, rcy), rSize * 0.8f, 0, 24, false, retColor);
    parent->addChild(reticle, 0);

    VisualUtil::addAmbientParticles(parent, Size(launchLeft, h), -5);
}
