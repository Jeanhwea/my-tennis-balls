#include "ArenaView.h"

#include "common/GameConstants.h"
#include "view/AmbientParticles.h"

USING_NS_CC;

// ── 创建物理边界墙和墙壁微光 ──

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

    // 墙壁微光
    auto wallDraw = DrawNode::create();
    Color4F wallColor(0.2f, 0.4f, 0.7f, 0.4f);
    wallDraw->drawLine(Vec2(0, 0), Vec2(0, h), wallColor);
    wallDraw->drawLine(Vec2(w, 0), Vec2(w, h), wallColor);
    wallDraw->drawLine(Vec2(0, h), Vec2(w, h), wallColor);
    wallDraw->drawLine(Vec2(0, 0), Vec2(w, 0), wallColor);
    parent->addChild(wallDraw, 1);
}

// ── 创建底部地板传感器（检测出界） ──

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

// ── 绘制竞技场视觉元素（背景渐变、网格、发射区域、装饰） ──

void ArenaView::drawZones(Node *parent, const Size &visibleSize)
{
    float w = visibleSize.width;
    float h = visibleSize.height;
    float launchLeft = w * (1.0f - LAUNCH_ZONE_RATIO);

    // 背景
    auto bg = DrawNode::create();
    Color4F bgBot(0.04f, 0.04f, 0.10f, 1.0f);
    Color4F bgTop(0.08f, 0.06f, 0.16f, 1.0f);
    static constexpr int BG_STRIPS = 20;
    for (int i = 0; i < BG_STRIPS; ++i) {
        float t0 = static_cast<float>(i) / BG_STRIPS;
        float t1 = static_cast<float>(i + 1) / BG_STRIPS;
        float y0 = h * t0;
        float y1 = h * t1;
        Color4F c0(bgBot.r + (bgTop.r - bgBot.r) * t0, bgBot.g + (bgTop.g - bgBot.g) * t0,
                   bgBot.b + (bgTop.b - bgBot.b) * t0, 1.0f);
        Color4F c1(bgBot.r + (bgTop.r - bgBot.r) * t1, bgBot.g + (bgTop.g - bgBot.g) * t1,
                   bgBot.b + (bgTop.b - bgBot.b) * t1, 1.0f);
        Color4F avg((c0.r + c1.r) / 2, (c0.g + c1.g) / 2, (c0.b + c1.b) / 2, 1.0f);
        bg->drawSolidRect(Vec2(0, y0), Vec2(w, y1), avg);
    }
    parent->addChild(bg, -10);

    // 网格
    auto grid = DrawNode::create();
    Color4F gridColor(0.15f, 0.18f, 0.28f, 0.15f);
    static constexpr float GRID_SPACING = 60.0f;
    for (float x = 0; x < launchLeft; x += GRID_SPACING) {
        grid->drawLine(Vec2(x, 0), Vec2(x, h), gridColor);
    }
    for (float y = 0; y < h; y += GRID_SPACING) {
        grid->drawLine(Vec2(0, y), Vec2(launchLeft, y), gridColor);
    }
    parent->addChild(grid, -9);

    // 发射区域 + 分隔线
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

    // 分隔线
    for (int i = 3; i >= 0; --i) {
        float offset = static_cast<float>(i) * 2.0f;
        float alpha = 0.1f + 0.15f * (3 - i);
        zone->drawLine(Vec2(launchLeft - offset, 0), Vec2(launchLeft - offset, h),
                       Color4F(0.4f, 0.6f, 1.0f, alpha));
    }
    zone->drawLine(Vec2(launchLeft, 0), Vec2(launchLeft, h), Color4F(0.5f, 0.75f, 1.0f, 0.7f));

    parent->addChild(zone, -8);

    // 角落
    auto corners = DrawNode::create();
    Color4F cornerColor(0.3f, 0.5f, 0.9f, 0.35f);
    float cLen = 30.0f;
    float cThick = 2.0f;
    float pad = 4.0f;

    corners->drawSolidRect(Vec2(pad, h - pad - cThick), Vec2(pad + cLen, h - pad), cornerColor);
    corners->drawSolidRect(Vec2(pad, h - pad - cLen), Vec2(pad + cThick, h - pad), cornerColor);
    corners->drawSolidRect(Vec2(launchLeft - pad - cLen, h - pad - cThick), Vec2(launchLeft - pad, h - pad),
                           cornerColor);
    corners->drawSolidRect(Vec2(launchLeft - pad - cThick, h - pad - cLen), Vec2(launchLeft - pad, h - pad),
                           cornerColor);
    corners->drawSolidRect(Vec2(pad, pad), Vec2(pad + cLen, pad + cThick), cornerColor);
    corners->drawSolidRect(Vec2(pad, pad), Vec2(pad + cThick, pad + cLen), cornerColor);
    corners->drawSolidRect(Vec2(launchLeft - pad - cLen, pad), Vec2(launchLeft - pad, pad + cThick),
                           cornerColor);
    corners->drawSolidRect(Vec2(launchLeft - pad - cThick, pad), Vec2(launchLeft - pad, pad + cLen),
                           cornerColor);
    parent->addChild(corners, -7);

    // 暗角
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

    // 发射区域标签
    auto label = Label::createWithTTF("LAUNCH", FONT_UI, 14);
    label->setRotation(-90);
    label->setPosition(Vec2(launchLeft + (w - launchLeft) / 2, h / 2));
    label->setTextColor(Color4B(100, 160, 255, 60));
    parent->addChild(label, 0);

    // 准星
    auto reticle = DrawNode::create();
    float rcx = launchLeft + (w - launchLeft) / 2;
    float rcy = h * 0.3f;
    float rSize = 18.0f;
    Color4F retColor(0.4f, 0.6f, 1.0f, 0.15f);
    reticle->drawLine(Vec2(rcx - rSize, rcy), Vec2(rcx + rSize, rcy), retColor);
    reticle->drawLine(Vec2(rcx, rcy - rSize), Vec2(rcx, rcy + rSize), retColor);
    reticle->drawCircle(Vec2(rcx, rcy), rSize * 0.8f, 0, 24, false, retColor);
    parent->addChild(reticle, 0);

    // 浮动光点
    auto ambient = AmbientParticles::create(Size(launchLeft, h));
    parent->addChild(ambient, -5);
    ambient->start();
}
