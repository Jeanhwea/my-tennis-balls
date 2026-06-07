#include "GameScene.h"

#include "common/GameConstants.h"

USING_NS_CC;

// ── 创建默认场景（使用关卡索引0） ──

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

// ── 场景初始化（开启物理世界） ──

bool GameScene::init()
{
    if (!Scene::initWithPhysics()) {
        return false;
    }

    setName("GameScene");
    getPhysicsWorld()->setGravity(Vec2(0, GRAVITY_Y));
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

    scheduleUpdate();
    return true;
}

// ── 进入场景时初始化控制器 ──

void GameScene::onEnter()
{
    Scene::onEnter();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _controller.init(this, visibleSize, _startLevel);
}

// ── 每帧将更新委托给控制器 ──

void GameScene::update(float dt)
{
    _controller.update(dt);
}
