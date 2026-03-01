#include "HelloWorldScene.h"

#include "SimpleAudioEngine.h"

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

    this->setName("HelloWorldScene");
    this->getPhysicsWorld()->setGravity(Vec2(0, -98));

#if IS_DEBUG
    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#else
    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
#endif

    _visibleSize = Director::getInstance()->getVisibleSize();
    _ballCounter = 0;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void HelloWorld::onEnter()
{
    Scene::onEnter();
    addEdgeBox();
    // addBall(Vec2(_visibleSize.width / 2, _visibleSize.height / 2));

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
    auto body = PhysicsBody::createEdgeBox(_visibleSize, PhysicsMaterial(1.0, 1.0, 0.0), 1);
    body->setCategoryBitmask(PHYSICS_CATEGORY_EDGE);
    body->setCollisionBitmask(PHYSICS_CATEGORY_ALL);
    body->setContactTestBitmask(PHYSICS_CATEGORY_ALL);
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
}

void HelloWorld::removeEdgeBox()
{
    auto edgeNode = this->getChildByName("edgeBox");
    if (edgeNode) {
        this->removeChild(edgeNode);
    }
}

void HelloWorld::updateVisibleSize()
{
    _visibleSize = Director::getInstance()->getVisibleSize();
    removeEdgeBox();
    addEdgeBox();
}

void HelloWorld::addBall(Vec2 position)
{
    auto ball = Sprite::create("ball.png");
    ball->setScale(0.1);
    ball->setPosition(position);
    std::string ballName = StringUtils::format("ball%02d", ++_ballCounter);
    ball->setName(ballName);
    // 计算缩放后的实际半径
    float scaledWidth = ball->getContentSize().width * ball->getScale();
    float radius = scaledWidth / 2;
    auto body = PhysicsBody::createCircle(radius, PhysicsMaterial(0.1, 0.85, 0.0));
    body->setCategoryBitmask(PHYSICS_CATEGORY_BALL);
    body->setCollisionBitmask(PHYSICS_CATEGORY_ALL);
    body->setContactTestBitmask(PHYSICS_CATEGORY_ALL);
    ball->setPhysicsBody(body);
    this->addChild(ball);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    auto target = event->getCurrentTarget();
    auto pos = touch->getLocation();
    CCLOG("点击目标: %s, 位置: (%.2f, %.2f)", target->getName().c_str(), pos.x, pos.y);

    auto location = touch->getLocation();
    addBall(location);
    return true;
}

bool HelloWorld::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    CCLOG("碰撞检测： %s === %s", nodeA->getName().c_str(), nodeB->getName().c_str());
    return true;
}
