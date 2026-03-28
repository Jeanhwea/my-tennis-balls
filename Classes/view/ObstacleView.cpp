#include "ObstacleView.h"

#include "common/GameConstants.h"
#include "common/Random.h"

USING_NS_CC;
using common::randomFloat;

namespace
{

/// 从冷色调色板中随机选取颜色。
Color4F pickObstacleColor()
{
    // 蓝紫色调
    static const Color4F palette[] = {
        Color4F(0.25f, 0.45f, 0.75f, 0.85f), Color4F(0.35f, 0.30f, 0.70f, 0.85f),
        Color4F(0.20f, 0.55f, 0.65f, 0.85f), Color4F(0.40f, 0.35f, 0.60f, 0.85f),
        Color4F(0.22f, 0.40f, 0.58f, 0.85f),
    };
    int idx = static_cast<int>(randomFloat(0, 4.99f));
    return palette[idx];
}

void spawnOne(Node *parent, const Vec2 &position, bool isCircle)
{
    auto obstacle = DrawNode::create();
    obstacle->setPosition(position);
    obstacle->setTag(TAG_OBSTACLE);
    obstacle->setName("obstacle");

    Color4F color = pickObstacleColor();
    Color4F edgeColor(color.r + 0.2f, color.g + 0.2f, color.b + 0.2f, 0.6f);
    PhysicsBody *body = nullptr;

    if (isCircle) {
        float radius = randomFloat(OBSTACLE_MIN_SIZE / 2, OBSTACLE_MAX_SIZE / 2);
        obstacle->drawSolidCircle(Vec2::ZERO, radius + 4, 0, 32, Color4F(color.r, color.g, color.b, 0.12f));
        obstacle->drawSolidCircle(Vec2::ZERO, radius, 0, 32, color);
        obstacle->drawSolidCircle(Vec2(0, radius * 0.25f), radius * 0.55f, 0, 20,
                                  Color4F(1.0f, 1.0f, 1.0f, 0.08f));
        obstacle->drawCircle(Vec2::ZERO, radius, 0, 32, false, edgeColor);
        body = PhysicsBody::createCircle(
            radius, PhysicsMaterial(OBSTACLE_DENSITY, OBSTACLE_RESTITUTION, OBSTACLE_FRICTION));
    } else {
        float w = randomFloat(OBSTACLE_MIN_SIZE, OBSTACLE_MAX_SIZE);
        float h = randomFloat(OBSTACLE_MIN_SIZE * 0.4f, OBSTACLE_MIN_SIZE * 0.8f);
        Vec2 verts[4] = {Vec2(-w / 2, -h / 2), Vec2(w / 2, -h / 2), Vec2(w / 2, h / 2),
                         Vec2(-w / 2, h / 2)};
        Vec2 glowVerts[4] = {Vec2(-w / 2 - 3, -h / 2 - 3), Vec2(w / 2 + 3, -h / 2 - 3),
                             Vec2(w / 2 + 3, h / 2 + 3), Vec2(-w / 2 - 3, h / 2 + 3)};
        obstacle->drawSolidPoly(glowVerts, 4, Color4F(color.r, color.g, color.b, 0.10f));
        obstacle->drawSolidPoly(verts, 4, color);
        obstacle->drawSolidRect(Vec2(-w / 2 + 2, h / 2 - 3), Vec2(w / 2 - 2, h / 2 - 1),
                                Color4F(1.0f, 1.0f, 1.0f, 0.10f));
        obstacle->drawPoly(verts, 4, true, edgeColor);
        body = PhysicsBody::createBox(
            Size(w, h), PhysicsMaterial(OBSTACLE_DENSITY, OBSTACLE_RESTITUTION, OBSTACLE_FRICTION));
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
    float left = visibleSize.width * OBSTACLE_ZONE_LEFT;
    float right = visibleSize.width * OBSTACLE_ZONE_RIGHT;
    float bottom = visibleSize.height * OBSTACLE_ZONE_BOTTOM;
    float top = visibleSize.height * OBSTACLE_ZONE_TOP;

    for (int i = 0; i < count; ++i) {
        float x = randomFloat(left, right);
        float y = randomFloat(bottom, top);
        spawnOne(parent, Vec2(x, y), i % 2 == 0);
    }
}
