#include "GameController.h"

#include "common/GameConstants.h"
#include "scene/LevelMenuScene.h"
#include "view/ArenaView.h"
#include "view/TrayView.h"

USING_NS_CC;

void GameController::init(Scene *scene, const Size &visibleSize, int startLevel)
{
    _scene = scene;
    _visibleSize = visibleSize;

    _ballManager.clear();

    _collisionSystem.setModel(&_model);
    _collisionSystem.setSceneNode(_scene);

    _ballManager.setDespawnCallback([this]() {
        refreshHUD();
        if (_model.isCleared()) {
            _levelManager.onLevelCleared();
        }
    });

    _collisionSystem.setScheduledRemovalCallback([this](Node *node) {
        _ballManager.scheduleRemoval(node);
    });

    ArenaView::addEdgeWalls(_scene, _visibleSize);
    ArenaView::addFloorSensor(_scene, _visibleSize);
    ArenaView::drawZones(_scene, _visibleSize);

    _aimLine.init(_scene);

    _hud = HUD::create(_visibleSize);
    _hud->setOverlayParent(_scene);
    _scene->addChild(_hud, 20);

    _model.scoreManager().setOnChange([this]() { refreshHUD(); });

    _hud->setOnBack([this]() {
        LevelMenuScene::setInitialLevelIndex(_model.levelIndex());
        auto menuScene = LevelMenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.4f, menuScene, Color3B(10, 10, 30)));
    });

    setupInput();
    setupPhysics();

    _levelManager.init(_scene, _visibleSize, _model, _ballManager, startLevel);
}

void GameController::update(float dt)
{
    _model.tick(dt);
    _levelManager.update(dt);
}

void GameController::setupInput()
{
    _input.setLaunchZoneMinX(_visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO));

    _input.setOnDrag([this](const Vec2 &start, const Vec2 &delta) { _aimLine.draw(start, delta); });

    _input.setOnDragEnd([this]() { _aimLine.clear(); });

    _input.setOnLaunch([this](const InputController::LaunchCommand &cmd) {
        if (_levelManager.isTransitioning()) return;
        const auto &level = _model.currentLevel();
        if (_levelManager.ballCounter() >= level.maxBalls) return;
        _ballManager.spawnBall(_scene, cmd.position, cmd.velocity, _levelManager.ballCounter());
        _levelManager.incrementBallCounter();
        _model.useBall();
        _hud->hideHint();
        refreshHUD();
    });

    _input.init(_scene);
}

void GameController::setupPhysics()
{
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = [this](PhysicsContact &c) { return _collisionSystem.onContactBegin(c); };
    _scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _scene);
}

void GameController::refreshHUD()
{
    if (!_hud) return;
    const auto &level = _model.currentLevel();
    _hud->updateScore(_model.scoreManager().score());
    _hud->updateCombo(_model.scoreManager().combo());
    _hud->updateBallCount(_levelManager.ballCounter(), level.maxBalls);
    _hud->updateTargets(_model.targetsRemaining());
}
