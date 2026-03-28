#include "HelloWorldScene.h"

#include <cstdlib>

USING_NS_CC;

namespace {

constexpr int TAG_BALL     = 100;
constexpr int TAG_OBSTACLE = 200;
constexpr int TAG_FLOOR    = 300;

constexpr float OBSTACLE_MIN_SIZE = 40.0f;
constexpr float OBSTACLE_MAX_SIZE = 100.0f;

constexpr float HUD_MARGIN    = 20.0f;
constexpr float HUD_FONT_SZ   = 24.0f;
constexpr float COMBO_FONT_SZ = 32.0f;
constexpr float HINT_FONT_SZ  = 20.0f;

constexpr float FLOAT_SCORE_FONT = 28.0f;
constexpr float FLOAT_SCORE_RISE = 60.0f;
constexpr float FLOAT_SCORE_DUR  = 0.8f;

constexpr int PARTICLE_COUNT   = 8;
constexpr float PARTICLE_SIZE  = 4.0f;
constexpr float PARTICLE_SPEED = 120.0f;
constexpr float PARTICLE_LIFE  = 0.4f;

float randomFloat(float min, float max)
{
    return min + static_cast<float>(std::rand()) / RAND_MAX * (max - min);
}

}  // namespace

Scene *HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if (!Scene::initWithPhysics()) {
        return false;
    }

    setName("HelloWorldScene");
    getPhysicsWorld()->setGravity(Vec2(0, GRAVITY_Y));

#if IS_DEBUG
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#else
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
#endif

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    _aimLine = DrawNode::create();
    addChild(_aimLine, 10);

    scheduleUpdate();
    return true;
}

void HelloWorld::onEnter()
{
    Scene::onEnter();

    addEdgeWalls();
    addFloorSensor();
    addObstacles();
    addHUD();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::update(float dt)
{
    if (_combo > 0) {
        _comboTimer -= dt;
        if (_comboTimer <= 0.0f) {
            resetCombo();
        }
    }
}

// ── Boundary setup ──────────────────────────────────────────────

void HelloWorld::addEdgeWalls()
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

void HelloWorld::addFloorSensor()
{
    auto floor = Node::create();
    floor->setName("floor");
    floor->setTag(TAG_FLOOR);
    floor->setPosition(Vec2(_visibleSize.width / 2, -FLOOR_HEIGHT));

    auto body = PhysicsBody::createBox(
        Size(_visibleSize.width + 100, FLOOR_HEIGHT * 2), PhysicsMaterial(0, 0, 0));
    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_FLOOR);
    body->setCollisionBitmask(0);
    body->setContactTestBitmask(CATEGORY_BALL);
    floor->setPhysicsBody(body);
    addChild(floor);
}

// ── Obstacles ───────────────────────────────────────────────────

void HelloWorld::addObstacles()
{
    float marginX = _visibleSize.width * 0.1f;
    float marginBottom = _visibleSize.height * 0.1f;
    float marginTop = _visibleSize.height * 0.75f;

    for (int i = 0; i < OBSTACLE_COUNT; ++i) {
        float x = randomFloat(marginX, _visibleSize.width - marginX);
        float y = randomFloat(marginBottom, marginTop);
        spawnObstacle(Vec2(x, y), i % 2 == 0);
    }
}

void HelloWorld::spawnObstacle(const Vec2 &position, bool isCircle)
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
        Vec2 verts[4] = {
            Vec2(-w / 2, -h / 2), Vec2(w / 2, -h / 2),
            Vec2(w / 2, h / 2), Vec2(-w / 2, h / 2)
        };
        obstacle->drawSolidPoly(verts, 4, color);
        obstacle->drawPoly(verts, 4, true, Color4F::WHITE);
        body = PhysicsBody::createBox(Size(w, h), PhysicsMaterial(1.0f, 1.0f, 0.0f));
    }

    body->setDynamic(false);
    body->setCategoryBitmask(CATEGORY_OBSTACLE);
    body->setCollisionBitmask(CATEGORY_ALL);
    body->setContactTestBitmask(CATEGORY_ALL);
    obstacle->setPhysicsBody(body);
    addChild(obstacle, 1);
}

// ── HUD ─────────────────────────────────────────────────────────

void HelloWorld::addHUD()
{
    _scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", HUD_FONT_SZ);
    _scoreLabel->setAnchorPoint(Vec2(0, 1));
    _scoreLabel->setPosition(Vec2(HUD_MARGIN, _visibleSize.height - HUD_MARGIN));
    _scoreLabel->setTextColor(Color4B::WHITE);
    addChild(_scoreLabel, 20);

    _ballCountLabel = Label::createWithSystemFont("Balls: 0", "Arial", HUD_FONT_SZ);
    _ballCountLabel->setAnchorPoint(Vec2(1, 1));
    _ballCountLabel->setPosition(Vec2(_visibleSize.width - HUD_MARGIN, _visibleSize.height - HUD_MARGIN));
    _ballCountLabel->setTextColor(Color4B::WHITE);
    addChild(_ballCountLabel, 20);

    _comboLabel = Label::createWithSystemFont("", "Arial", COMBO_FONT_SZ);
    _comboLabel->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - HUD_MARGIN - 10));
    _comboLabel->setTextColor(Color4B::YELLOW);
    _comboLabel->setVisible(false);
    addChild(_comboLabel, 20);

    _hintLabel = Label::createWithSystemFont("Drag to launch a ball!", "Arial", HINT_FONT_SZ);
    _hintLabel->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - HUD_MARGIN * 3));
    _hintLabel->setTextColor(Color4B(255, 255, 255, 160));
    addChild(_hintLabel, 20);
}

void HelloWorld::updateHUD()
{
    _scoreLabel->setString(StringUtils::format("Score: %d", _score));

    int ballCount = 0;
    for (auto child : getChildren()) {
        if (child->getTag() == TAG_BALL) {
            ++ballCount;
        }
    }
    _ballCountLabel->setString(StringUtils::format("Balls: %d/%d", ballCount, MAX_BALLS));

    if (_combo > 1) {
        _comboLabel->setString(StringUtils::format("Combo x%d", _combo));
        _comboLabel->setVisible(true);
        _comboLabel->stopAllActions();
        _comboLabel->setScale(1.3f);
        _comboLabel->runAction(EaseBackOut::create(ScaleTo::create(0.2f, 1.0f)));
    } else {
        _comboLabel->setVisible(false);
    }
}

// ── Ball management ─────────────────────────────────────────────

Color3B HelloWorld::randomBallColor() const
{
    static const Color3B palette[] = {
        Color3B(255, 80, 80),   Color3B(80, 200, 255),
        Color3B(80, 255, 120),  Color3B(255, 220, 50),
        Color3B(255, 140, 50),  Color3B(200, 100, 255),
        Color3B(255, 100, 200), Color3B(100, 255, 220),
    };
    return palette[std::rand() % 8];
}

void HelloWorld::addBall(const Vec2 &position, const Vec2 &velocity)
{
    int ballCount = 0;
    for (auto child : getChildren()) {
        if (child->getTag() == TAG_BALL) {
            ++ballCount;
        }
    }
    if (ballCount >= MAX_BALLS) return;

    auto ball = Sprite::create("ball.png");
    ball->setScale(BALL_SCALE);
    ball->setPosition(position);
    ball->setTag(TAG_BALL);
    ball->setColor(randomBallColor());
    ball->setName(StringUtils::format("ball%02d", ++_ballCounter));

    const float radius = ball->getContentSize().width / 2 - BALL_SPRITE_PADDING;
    auto body = PhysicsBody::createCircle(
        radius, PhysicsMaterial(BALL_DENSITY, BALL_RESTITUTION, BALL_FRICTION));
    body->setVelocity(velocity);
    body->setCategoryBitmask(CATEGORY_BALL);
    body->setCollisionBitmask(CATEGORY_ALL);
    body->setContactTestBitmask(CATEGORY_ALL);
    ball->setPhysicsBody(body);
    addChild(ball, 5);

    ball->setScale(0);
    ball->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE)));

    if (_hintLabel && _hintLabel->isVisible()) {
        _hintLabel->runAction(Sequence::create(FadeOut::create(0.5f), Hide::create(), nullptr));
    }

    updateHUD();
}

void HelloWorld::removeBall(Node *ball)
{
    if (!ball) return;
    ball->getPhysicsBody()->setEnabled(false);
    ball->runAction(Sequence::create(
        Spawn::create(ScaleTo::create(0.2f, 0), FadeOut::create(0.2f), nullptr),
        RemoveSelf::create(),
        CallFunc::create([this]() { updateHUD(); }),
        nullptr));
}

// ── Scoring ─────────────────────────────────────────────────────

void HelloWorld::addScore(int points)
{
    _score += points * std::max(1, _combo);
    _combo++;
    _comboTimer = COMBO_TIMEOUT;
    updateHUD();
}

void HelloWorld::resetCombo()
{
    _combo = 0;
    _comboTimer = 0.0f;
    updateHUD();
}

// ── Visual effects ──────────────────────────────────────────────

void HelloWorld::flashNode(Node *node)
{
    if (!node) return;
    node->runAction(Sequence::create(
        TintTo::create(0.05f, 255, 255, 255),
        TintTo::create(0.15f, 200, 200, 200),
        nullptr));
}

void HelloWorld::spawnHitParticle(const Vec2 &position)
{
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        auto dot = DrawNode::create();
        Color4F c(randomFloat(0.7f, 1.0f), randomFloat(0.7f, 1.0f), randomFloat(0.2f, 0.6f), 1.0f);
        dot->drawSolidCircle(Vec2::ZERO, PARTICLE_SIZE, 0, 8, c);
        dot->setPosition(position);
        addChild(dot, 15);

        float angle = randomFloat(0, static_cast<float>(M_PI) * 2);
        float speed = randomFloat(PARTICLE_SPEED * 0.5f, PARTICLE_SPEED);
        Vec2 dir(cosf(angle) * speed, sinf(angle) * speed);

        dot->runAction(Sequence::create(
            Spawn::create(
                MoveBy::create(PARTICLE_LIFE, dir * PARTICLE_LIFE),
                FadeOut::create(PARTICLE_LIFE),
                ScaleTo::create(PARTICLE_LIFE, 0.1f),
                nullptr),
            RemoveSelf::create(),
            nullptr));
    }
}

void HelloWorld::showFloatingScore(const Vec2 &position, int points)
{
    auto label = Label::createWithSystemFont(
        StringUtils::format("+%d", points), "Arial", FLOAT_SCORE_FONT);
    label->setPosition(position);
    label->setTextColor(Color4B::YELLOW);
    addChild(label, 20);

    label->runAction(Sequence::create(
        Spawn::create(
            MoveBy::create(FLOAT_SCORE_DUR, Vec2(0, FLOAT_SCORE_RISE)),
            FadeOut::create(FLOAT_SCORE_DUR),
            nullptr),
        RemoveSelf::create(),
        nullptr));
}

// ── Input handling ──────────────────────────────────────────────

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    _isDragging = true;
    _dragStart = touch->getLocation();
    _aimLine->clear();
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
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

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
    _aimLine->clear();
    if (!_isDragging) return;
    _isDragging = false;

    auto endPos = touch->getLocation();
    Vec2 delta = _dragStart - endPos;
    float distance = delta.length();

    if (distance < MIN_DRAG_DISTANCE) {
        addBall(_dragStart, Vec2::ZERO);
    } else {
        Vec2 launchDir = delta.getNormalized();
        float speed = std::min(distance * LAUNCH_FORCE_SCALE, MAX_LAUNCH_SPEED);
        addBall(_dragStart, launchDir * speed);
    }
}

// ── Physics callbacks ───────────────────────────────────────────

bool HelloWorld::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return true;

    // Ball fell through floor
    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_FLOOR) ||
        (nodeA->getTag() == TAG_FLOOR && nodeB->getTag() == TAG_BALL)) {
        auto ball = (nodeA->getTag() == TAG_BALL) ? nodeA : nodeB;
        removeBall(ball);
        resetCombo();
        return false;
    }

    // Ball hits obstacle
    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_OBSTACLE) ||
        (nodeA->getTag() == TAG_OBSTACLE && nodeB->getTag() == TAG_BALL)) {
        auto obstacle = (nodeA->getTag() == TAG_OBSTACLE) ? nodeA : nodeB;
        auto cp = contact.getContactData()->points[0];
        int points = SCORE_PER_HIT * std::max(1, _combo);
        addScore(SCORE_PER_HIT);
        showFloatingScore(Vec2(cp.x, cp.y), points);
        spawnHitParticle(Vec2(cp.x, cp.y));
        flashNode(obstacle);
        return true;
    }

    // Ball hits ball
    if (nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_BALL) {
        auto cp = contact.getContactData()->points[0];
        addScore(SCORE_PER_HIT / 2);
        showFloatingScore(Vec2(cp.x, cp.y), SCORE_PER_HIT / 2 * std::max(1, _combo));
        spawnHitParticle(Vec2(cp.x, cp.y));
        return true;
    }

    return true;
}
