#include "GameScene.h"

#include "common/GameConstants.h"

USING_NS_CC;

Scene *GameScene::createScene()
{
    return GameScene::create();
}

Scene *GameScene::createSceneWithLevel(int levelIndex)
{
    auto scene = GameScene::create();
    scene->setStartLevel(levelIndex);
    return scene;
}

bool GameScene::init()
{
    CCLOG("[GameScene] init begin");
    if (!Scene::initWithPhysics()) {
        return false;
    }

    setName("GameScene");
    getPhysicsWorld()->setGravity(Vec2(0, GRAVITY_Y));
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

    scheduleUpdate();
    return true;
}

void GameScene::onEnter()
{
    CCLOG("[GameScene] onEnter begin");
    Scene::onEnter();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _controller.init(this, visibleSize, _startLevel);
    CCLOG("[GameScene] onEnter done");
}

void GameScene::update(float dt)
{
    CCLOG("[GameScene] update dt=%.4f", dt);
    _controller.update(dt);
}
