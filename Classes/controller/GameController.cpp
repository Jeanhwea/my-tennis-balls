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

    _ballMgr.clear();

    _collision.setModel(&_model);
    _collision.setSceneNode(_scene);

    _ballMgr.setDespawnCallback([this]() {
        refreshHUD();
        if (_model.isCleared()) {
            onLevelCleared();
        }
    });

    _collision.setScheduledRemovalCallback([this](Node *node) {
        _ballMgr.scheduleRemoval(node);
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

    loadLevel(startLevel);
}

void GameController::update(float dt)
{
    _model.tick(dt);
    if (!_transitioning) {
        _ballMgr.updateEffects();
        _ballMgr.collectOutOfBounds(_scene, _visibleSize);
        _ballMgr.processPendingRemovals();
    }
}

void GameController::loadLevel(int index)
{
    if (index < 0 || index >= static_cast<int>(getAllLevels().size())) {
        CCLOG("[GameController] ERROR: level index %d out of range!", index);
        return;
    }
    _transitioning = false;
    _ballCounter = 0;

    _scene->stopAllActions();
    clearLevelNodes();

    _model.loadLevel(index);
    const auto &level = _model.currentLevel();

    cocos2d::Vector<Node *> targets;
    int totalTargets = TrayView::createFromLevel(_scene, _visibleSize, level, targets);
    _model.setTargetsRemaining(totalTargets);
    _ballMgr.initFromLevel();

    _hud->updateLevel(level.id, level.name);
    _hud->showLevelIntro(level.id, level.name);
    refreshHUD();
}

void GameController::clearLevelNodes()
{
    _ballMgr.clear();

    std::vector<Node *> toRemove;
    for (auto child : _scene->getChildren()) {
        int tag = child->getTag();
        if (tag == TAG_BALL || tag == TAG_TARGET || tag == TAG_TRAY) {
            toRemove.push_back(child);
        }
    }
    for (auto node : toRemove) node->removeFromParent();
}

void GameController::onLevelCleared()
{
    if (_transitioning) return;
    _transitioning = true;

    _hud->showCleared();

    if (_model.hasNextLevel()) {
        int nextIdx = _model.levelIndex() + 1;
        _scene->runAction(Sequence::create(
            DelayTime::create(2.5f), CallFunc::create([this, nextIdx]() { loadLevel(nextIdx); }), nullptr));
    } else {
        _scene->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([this]() {
                                               LevelMenuScene::setInitialLevelIndex(_model.levelIndex());
                                               auto menuScene = LevelMenuScene::createScene();
                                               Director::getInstance()->replaceScene(TransitionFade::create(
                                                   0.5f, menuScene, Color3B(10, 10, 30)));
                                           }),
                                           nullptr));
    }
}

void GameController::onLevelFailed()
{
    if (_transitioning) return;
    _transitioning = true;

    _hud->showFailed();

    int curIdx = _model.levelIndex();
    _scene->runAction(Sequence::create(DelayTime::create(2.5f),
                                       CallFunc::create([this, curIdx]() { loadLevel(curIdx); }), nullptr));
}

void GameController::checkFailCondition()
{
    if (_transitioning) return;
    if (_model.isCleared()) return;
    const auto &level = _model.currentLevel();
    if (_ballCounter >= level.maxBalls && _ballMgr.ballCount() == 0) {
        onLevelFailed();
    }
}

void GameController::setupInput()
{
    _input.setLaunchZoneMinX(_visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO));

    _input.setOnDrag([this](const Vec2 &start, const Vec2 &delta) { _aimLine.draw(start, delta); });

    _input.setOnDragEnd([this]() { _aimLine.clear(); });

    _input.setOnLaunch([this](const InputController::LaunchCommand &cmd) {
        if (_transitioning) return;
        const auto &level = _model.currentLevel();
        if (_ballCounter >= level.maxBalls) return;
        _ballCounter++;
        _ballMgr.spawnBall(_scene, cmd.position, cmd.velocity, _ballCounter);
        _model.useBall();
        _hud->hideHint();
        refreshHUD();
    });

    _input.init(_scene);
}

void GameController::setupPhysics()
{
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = [this](PhysicsContact &c) { return _collision.onContactBegin(c); };
    _scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _scene);
}

void GameController::refreshHUD()
{
    if (!_hud) return;
    const auto &level = _model.currentLevel();
    _hud->updateScore(_model.scoreManager().score());
    _hud->updateCombo(_model.scoreManager().combo());
    _hud->updateBallCount(_ballCounter, level.maxBalls);
    _hud->updateTargets(_model.targetsRemaining());
}
