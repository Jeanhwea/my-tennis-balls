#include "GameScene.h"

#include <algorithm>

#include "GameConstants.h"
#include "ObstacleManager.h"
#include "VFXHelper.h"

USING_NS_CC;

Scene *GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::initWithPhysics()) {
        return false;
    }

    setName("GameScene");
    getPhysicsWorld()->setGravity(Vec2(0, GRAVITY_Y));

#if IS_DEBUG
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#else
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
#endif

    _visibleSize = Director::getInstance()->getVisibleSize();

    // Contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Aim line
    _aimLine = DrawNode::create();
    addChild(_aimLine, 10);

    // Sub-systems
    _ballMgr = std::make_unique<BallManager>(this);
    _ballMgr->setOnCountChange([this](int) { refreshHUD(); });
    _scoreMgr.setOnChange([this]() { refreshHUD(); });

    scheduleUpdate();
    return true;
}

void GameScene::onEnter()
{
    Scene::onEnter();

    addEdgeWalls();
    addFloorSensor();
    ObstacleManager::spawnObstacles(this, _visibleSize, OBSTACLE_COUNT);

    _hud = HUD::create(_visibleSize);
    addChild(_hud, 20);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::update(float dt)
{
    _scoreMgr.tick(dt);
}

// ── Setup ───────────────────────────────────────────────────────

void GameScene::addEdgeWalls()
{
    auto edgeNode = Node::create();
    edgeNode->setName("edgeWalls");
    edgeNode->setPosition(Vec2::ZERO);

    auto body = PhysicsBody::create();
    body->setDynamic(false);

    float w = _visibleSize.width;
    float h = _visibleSize.height;

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
    addChild(edgeNode);
}

void GameScene::addFloorSensor()
{
    auto floor = Node::create();
    floor->setName("floor");
    floor->setTag(TAG_FLOOR);
    floor->setPosition(Vec2(_visibleSize.width / 2, -FLOOR_HEIGHT));

    auto body =
        PhysicsBody::createBox(Size(_visibleSize.width + 100, FLOOR_HEIGHT * 2), PhysicsMaterial(0, 0, 0));
    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_FLOOR);
    body->setCollisionBitmask(0);
    body->setContactTestBitmask(CATEGORY_BALL);
    floor->setPhysicsBody(body);
    addChild(floor);
}

void GameScene::refreshHUD()
{
    if (!_hud) return;
    _hud->updateScore(_scoreMgr.score());
    _hud->updateCombo(_scoreMgr.combo());
    _hud->updateBallCount(_ballMgr->countBalls(), MAX_BALLS);
}

// ── Input ───────────────────────────────────────────────────────

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    _isDragging = true;
    _dragStart = touch->getLocation();
    _aimLine->clear();
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event)
{
    if (!_isDragging) return;

    auto current = touch->getLocation();
    Vec2 delta = _dragStart - current;
    float distance = delta.length();

    _aimLine->clear();

    if (distance > MIN_DRAG_DISTANCE) {
        Vec2 launchDir = delta.getNormalized();
        float speed = std::min(distance * LAUNCH_FORCE_SCALE, MAX_LAUNCH_SPEED);

        int dots = static_cast<int>(distance / 8);
        for (int i = 0; i < dots; ++i) {
            float t = static_cast<float>(i) / dots;
            Vec2 p = _dragStart + launchDir * (distance * t * 0.5f);
            float alpha = 1.0f - t * 0.6f;
            _aimLine->drawSolidCircle(p, 2.5f, 0, 8, Color4F(1, 1, 1, alpha));
        }

        Vec2 tip = _dragStart + launchDir * (distance * 0.5f);
        _aimLine->drawSolidCircle(tip, 5.0f, 0, 8, Color4F(1, 1, 0, 0.9f));

        float powerRatio = speed / MAX_LAUNCH_SPEED;
        Color4F powerColor(1.0f, 1.0f - powerRatio, 0, 0.8f);
        _aimLine->drawSolidCircle(_dragStart, 8.0f, 0, 16, powerColor);
    }
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
    _aimLine->clear();
    if (!_isDragging) return;
    _isDragging = false;

    auto endPos = touch->getLocation();
    Vec2 delta = _dragStart - endPos;
    float distance = delta.length();

    if (distance < MIN_DRAG_DISTANCE) {
        _ballMgr->addBall(_dragStart, Vec2::ZERO);
    } else {
        Vec2 launchDir = delta.getNormalized();
        float speed = std::min(distance * LAUNCH_FORCE_SCALE, MAX_LAUNCH_SPEED);
        _ballMgr->addBall(_dragStart, launchDir * speed);
    }

    _hud->hideHint();
}

// ── Physics callbacks ───────────────────────────────────────────

bool GameScene::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return true;

    // Ball fell through floor
    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_FLOOR) ||
        (nodeA->getTag() == TAG_FLOOR && nodeB->getTag() == TAG_BALL)) {
        auto ball = (nodeA->getTag() == TAG_BALL) ? nodeA : nodeB;
        _ballMgr->removeBall(ball);
        _scoreMgr.resetCombo();
        return false;
    }

    // Ball hits obstacle
    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_OBSTACLE) ||
        (nodeA->getTag() == TAG_OBSTACLE && nodeB->getTag() == TAG_BALL)) {
        auto obstacle = (nodeA->getTag() == TAG_OBSTACLE) ? nodeA : nodeB;
        auto cp = contact.getContactData()->points[0];
        int points = SCORE_PER_HIT * std::max(1, _scoreMgr.combo());

        _scoreMgr.addScore(SCORE_PER_HIT);
        VFXHelper::showFloatingScore(this, Vec2(cp.x, cp.y), points);
        VFXHelper::spawnHitParticle(this, Vec2(cp.x, cp.y));
        VFXHelper::flashNode(obstacle);
        return true;
    }

    // Ball hits ball
    if (nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_BALL) {
        auto cp = contact.getContactData()->points[0];
        int points = SCORE_PER_HIT / 2 * std::max(1, _scoreMgr.combo());

        _scoreMgr.addScore(SCORE_PER_HIT / 2);
        VFXHelper::showFloatingScore(this, Vec2(cp.x, cp.y), points);
        VFXHelper::spawnHitParticle(this, Vec2(cp.x, cp.y));
        return true;
    }

    return true;
}
