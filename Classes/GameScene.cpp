#include "GameScene.h"

#include "GameConstants.h"

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

    scheduleUpdate();
    return true;
}

void GameScene::onEnter()
{
    Scene::onEnter();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _controller.init(this, visibleSize);
}

void GameScene::update(float dt)
{
    _controller.update(dt);
}
