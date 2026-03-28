#include "ArenaView.h"

#include "Classes/GameConstants.h"

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

    body->setCategoryBitmask(CATEGORY_EDGE);
    body->setCollisionBitmask(CATEGORY_ALL);
    body->setContactTestBitmask(CATEGORY_ALL);
    edgeNode->setPhysicsBody(body);
    parent->addChild(edgeNode);
}

void ArenaView::addFloorSensor(Node *parent, const Size &visibleSize)
{
    auto floor = Node::create();
    floor->setName("floor");
    floor->setTag(TAG_FLOOR);
    floor->setPosition(Vec2(visibleSize.width / 2, -FLOOR_HEIGHT));

    auto body =
        PhysicsBody::createBox(Size(visibleSize.width + 100, FLOOR_HEIGHT * 2), PhysicsMaterial(0, 0, 0));
    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_FLOOR);
    body->setCollisionBitmask(0);
    body->setContactTestBitmask(CATEGORY_BALL);
    floor->setPhysicsBody(body);
    parent->addChild(floor);
}
