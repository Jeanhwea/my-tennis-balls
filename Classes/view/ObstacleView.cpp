#include "ObstacleView.h"

#include <cstdlib>

#include "GameConstants.h"

USING_NS_CC;

namespace
{

float randomFloat(float min, float max)
{
    return min + static_cast<float>(std::rand()) / RAND_MAX * (max - min);
}

void spawnOne(Node *parent, const Vec2 &position, bool isCircle)
{
    auto obstacle = DrawNode::create();
    obstacle->setPosition(position);
    obstacle->setTag(TAG_OBSTACLE);
    obstacle->setName("obstacle");

    PhysicsBody *body = nullptr;

    if (isCircle) {
        float radius = randomFloat(OBSTACLE_MIN_SIZE / 2, OBSTACLE_MAX_SIZE / 2);
        Color4F color(randomFloat(0.3f, 0.8f), randomFloat(0.3f, 0.8f), randomFloat(0.3f, 0.8f), 0.9f);
        obstacle->drawSolidCircle(Vec2::ZERO, radius, 0, 32, color);
        obstacle->drawCircle(Vec2::ZERO, radius, 0, 32, false, Color4F::WHITE);
        body = PhysicsBody::createCircle(radius, PhysicsMaterial(1.0f, 1.0f, 0.0f));
    } else {
        float w = randomFloat(OBSTACLE_MIN_SIZE, OBSTACLE_MAX_SIZE);
        float h = randomFloat(OBSTACLE_MIN_SIZE * 0.4f, OBSTACLE_MIN_SIZE * 0.8f);
        Color4F color(randomFloat(0.3f, 0.8f), randomFloat(0.3f, 0.8f), randomFloat(0.3f, 0.8f), 0.9f);
        Vec2 verts[4] = {Vec2(-w / 2, -h / 2), Vec2(w / 2, -h / 2), Vec2(w / 2, h / 2),
                         Vec2(-w / 2, h / 2)};
        obstacle->drawSolidPoly(verts, 4, color);
        obstacle->drawPoly(verts, 4, true, Color4F::WHITE);
        body = PhysicsBody::createBox(Size(w, h), PhysicsMaterial(1.0f, 1.0f, 0.0f));
    }

    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_OBSTACLE);
    body->setCollisionBitmask(CATEGORY_ALL);
    body->setContactTestBitmask(CATEGORY_ALL);
    obstacle->setPhysicsBody(body);
    parent->addChild(obstacle, 1);
}

}  // namespace

void ObstacleView::spawnObstacles(Node *parent, const Size &visibleSize, int count)
{
    float marginX = visibleSize.width * 0.1f;
    float marginBottom = visibleSize.height * 0.1f;
    float marginTop = visibleSize.height * 0.75f;

    for (int i = 0; i < count; ++i) {
        float x = randomFloat(marginX, visibleSize.width - marginX);
        float y = randomFloat(marginBottom, marginTop);
        spawnOne(parent, Vec2(x, y), i % 2 == 0);
    }
}
