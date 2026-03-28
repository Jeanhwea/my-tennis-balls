#include "ArenaView.h"

#include "GameConstants.h"

USING_NS_CC;

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
    body->setCollisionBitmask(CATEGORY_BALL | CATEGORY_TRAY);
    body->setContactTestBitmask(CATEGORY_ALL);
    edgeNode->setPhysicsBody(body);
    parent->addChild(edgeNode);

    // Visual wall borders (subtle glow lines)
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

    // Background gradient (dark blue to dark purple, bottom to top)
    auto bg = DrawNode::create();
    Color4F bgBot(0.04f, 0.04f, 0.10f, 1.0f);
    Color4F bgTop(0.08f, 0.06f, 0.16f, 1.0f);
    int strips = 20;
    for (int i = 0; i < strips; ++i) {
        float t0 = static_cast<float>(i) / strips;
        float t1 = static_cast<float>(i + 1) / strips;
        float y0 = h * t0;
        float y1 = h * t1;
        Color4F c0(bgBot.r + (bgTop.r - bgBot.r) * t0, bgBot.g + (bgTop.g - bgBot.g) * t0,
                   bgBot.b + (bgTop.b - bgBot.b) * t0, 1.0f);
        Color4F c1(bgBot.r + (bgTop.r - bgBot.r) * t1, bgBot.g + (bgTop.g - bgBot.g) * t1,
                   bgBot.b + (bgTop.b - bgBot.b) * t1, 1.0f);
        // Use average color for the strip
        Color4F avg((c0.r + c1.r) / 2, (c0.g + c1.g) / 2, (c0.b + c1.b) / 2, 1.0f);
        bg->drawSolidRect(Vec2(0, y0), Vec2(w, y1), avg);
    }
    parent->addChild(bg, -10);

    // Subtle grid pattern
    auto grid = DrawNode::create();
    Color4F gridColor(0.15f, 0.18f, 0.28f, 0.15f);
    float gridSpacing = 60.0f;
    for (float x = 0; x < launchLeft; x += gridSpacing) {
        grid->drawLine(Vec2(x, 0), Vec2(x, h), gridColor);
    }
    for (float y = 0; y < h; y += gridSpacing) {
        grid->drawLine(Vec2(0, y), Vec2(launchLeft, y), gridColor);
    }
    parent->addChild(grid, -9);

    // Launch zone
    auto zone = DrawNode::create();

    // Launch zone gradient background
    int zoneStrips = 8;
    for (int i = 0; i < zoneStrips; ++i) {
        float t0 = static_cast<float>(i) / zoneStrips;
        float t1 = static_cast<float>(i + 1) / zoneStrips;
        float x0 = launchLeft + (w - launchLeft) * t0;
        float x1 = launchLeft + (w - launchLeft) * t1;
        float alpha = 0.08f + 0.12f * t0;
        zone->drawSolidRect(Vec2(x0, 0), Vec2(x1, h), Color4F(0.15f, 0.25f, 0.5f, alpha));
    }

    // Divider line with glow effect
    for (int i = 3; i >= 0; --i) {
        float offset = static_cast<float>(i) * 2.0f;
        float alpha = 0.1f + 0.15f * (3 - i);
        zone->drawLine(Vec2(launchLeft - offset, 0), Vec2(launchLeft - offset, h),
                       Color4F(0.4f, 0.6f, 1.0f, alpha));
    }
    zone->drawLine(Vec2(launchLeft, 0), Vec2(launchLeft, h), Color4F(0.5f, 0.75f, 1.0f, 0.7f));

    parent->addChild(zone, -8);

    // Launch zone label
    auto label = Label::createWithTTF("LAUNCH", FONT_UI, 14);
    label->setRotation(-90);
    label->setPosition(Vec2(launchLeft + (w - launchLeft) / 2, h / 2));
    label->setTextColor(Color4B(100, 160, 255, 60));
    parent->addChild(label, 0);
}
