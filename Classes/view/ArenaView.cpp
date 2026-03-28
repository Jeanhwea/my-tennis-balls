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

    // Left wall
    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(0, 0), Vec2(0, h), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));
    // Right wall
    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(w, 0), Vec2(w, h), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));
    // Top wall
    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(0, h), Vec2(w, h), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));
    // Bottom wall — blocks projectile balls but not targets
    body->addShape(PhysicsShapeEdgeSegment::create(
        Vec2(0, 0), Vec2(w, 0), PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1));

    body->setCategoryBitmask(CATEGORY_EDGE);
    // Collide with balls and tray, but NOT targets (so targets can fall through)
    body->setCollisionBitmask(CATEGORY_BALL | CATEGORY_TRAY);
    body->setContactTestBitmask(CATEGORY_ALL);
    edgeNode->setPhysicsBody(body);
    parent->addChild(edgeNode);
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
    auto draw = DrawNode::create();
    float launchLeft = visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO);

    // Launch zone background tint (right strip)
    draw->drawSolidRect(Vec2(launchLeft, 0), Vec2(visibleSize.width, visibleSize.height),
                        Color4F(0.15f, 0.25f, 0.4f, 0.3f));

    // Divider line between play area and launch zone
    draw->drawLine(Vec2(launchLeft, 0), Vec2(launchLeft, visibleSize.height),
                   Color4F(0.5f, 0.7f, 1.0f, 0.5f));

    // Launch zone label (rotated vertically)
    auto label = Label::createWithSystemFont("LAUNCH", "Arial", 16);
    label->setRotation(-90);
    label->setPosition(Vec2(launchLeft + (visibleSize.width - launchLeft) / 2, visibleSize.height / 2));
    label->setTextColor(Color4B(130, 180, 255, 100));
    parent->addChild(label, 0);

    parent->addChild(draw, 0);
}
