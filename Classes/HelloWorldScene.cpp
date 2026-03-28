#include "HelloWorldScene.h"

USING_NS_CC;

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

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void HelloWorld::onEnter()
{
    Scene::onEnter();
    addEdgeBox();

    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void HelloWorld::addEdgeBox()
{
    auto edgeNode = Node::create();
    edgeNode->setName("edgeBox");
    edgeNode->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2));

    auto body = PhysicsBody::createEdgeBox(
        _visibleSize, PhysicsMaterial(1.0f, EDGE_RESTITUTION, EDGE_FRICTION), 1);
    body->setCategoryBitmask(PHYSICS_CATEGORY_EDGE);
    body->setCollisionBitmask(PHYSICS_CATEGORY_ALL);
    body->setContactTestBitmask(PHYSICS_CATEGORY_ALL);
    edgeNode->setPhysicsBody(body);
    addChild(edgeNode);
}

void HelloWorld::addBall(const Vec2 &position)
{
    auto ball = Sprite::create("ball.png");
    ball->setScale(BALL_SCALE);
    ball->setPosition(position);
    ball->setName(StringUtils::format("ball%02d", ++_ballCounter));

    const float radius = ball->getContentSize().width / 2 - BALL_SPRITE_PADDING;
    auto body = PhysicsBody::createCircle(
        radius, PhysicsMaterial(BALL_DENSITY, BALL_RESTITUTION, BALL_FRICTION));
    body->setCategoryBitmask(PHYSICS_CATEGORY_BALL);
    body->setCollisionBitmask(PHYSICS_CATEGORY_ALL);
    body->setContactTestBitmask(PHYSICS_CATEGORY_ALL);
    ball->setPhysicsBody(body);
    addChild(ball);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    const auto location = touch->getLocation();
    CCLOG("点击目标: %s, 位置: (%.2f, %.2f)",
          event->getCurrentTarget()->getName().c_str(), location.x, location.y);

    addBall(location);
    return true;
}

bool HelloWorld::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB) {
        CCLOG("碰撞检测: %s === %s", nodeA->getName().c_str(), nodeB->getName().c_str());
    }
    return true;
}
