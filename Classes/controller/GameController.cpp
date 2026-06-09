#include "GameController.h"

#include "builder/GameSceneBuilder.h"
#include "common/GameConstants.h"
#include "controller/InputController.h"
#include "scene/LevelMenuScene.h"
#include "view/TrayView.h"
#include "view/VFXHelper.h"
#include "view/BallView.h"

USING_NS_CC;

void GameController::setupCallbacks()
{
    _ballManager.setDespawnCallback([this]() {
        refreshHUD();
        if (_model.isCleared()) {
            _levelManager.onLevelCleared();
        }
    });

    _collisionSystem.setOnScheduledRemoval([this](Node *node) {
        _ballManager.scheduleRemoval(node);
    });
    _collisionSystem.setOnScore([this](Node *scene, const Vec2 &pos, int basePoints) {
        int points = _model.scoreManager().addScore(basePoints);
        VFXHelper::showFloatingScore(scene ? scene : _scene, pos, points);
    });
    _collisionSystem.setOnHitParticle([this](Node *scene, const Vec2 &pos) {
        VFXHelper::spawnHitParticle(scene ? scene : _scene, pos);
    });
    _collisionSystem.setOnComboReset([this]() {
        _model.resetCombo();
    });
    _collisionSystem.setOnTargetRemoved([this]() {
        _model.removeTarget();
    });
}

void GameController::init(Scene *scene, const Size &visibleSize, int startLevel)
{
    _scene = scene;
    _visibleSize = visibleSize;

    _ballManager.clear();
    setupCallbacks();

    // Scene setup delegated to GameSceneBuilder
    GameSceneBuilder::setupArena(_scene, _visibleSize);

    _hud = static_cast<HUD *>(GameSceneBuilder::createHUD(_scene, _visibleSize, [this]() {
        LevelMenuScene::setInitialLevelIndex(_model.levelIndex());
        auto menuScene = LevelMenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.4f, menuScene, Color3B(10, 10, 30)));
    }));

    _model.scoreManager().setOnChange([this]() { refreshHUD(); });

    auto input = GameSceneBuilder::createInputController(_scene, _visibleSize);
    input->setOnLaunch([this](const InputController::LaunchCommand &cmd) {
        if (_levelManager.isTransitioning()) return;
        const auto &level = _model.currentLevel();
        if (_levelManager.ballCounter() >= level.maxBalls) return;
        _ballManager.spawnBall(_scene, cmd.position, cmd.velocity, _levelManager.ballCounter());
        _levelManager.incrementBallCounter();
        _model.useBall();
        _hud->hideHint();
        refreshHUD();
    });
    input->setOnDrag([this](const Vec2 &start, const Vec2 &delta) { /* aim line handled via callback */ });
    input->setOnDragEnd([this]() { /* aim line handled via callback */ });

    GameSceneBuilder::setupPhysics(_scene,
        [this](PhysicsContact &c) { return _collisionSystem.onContactBegin(c); });

    _levelManager.init(_scene, _visibleSize, _model, _ballManager, startLevel);
}

void GameController::update(float dt)
{
    _model.tick(dt);
    _levelManager.update(dt);
    if (!_levelManager.isTransitioning()) {
        for (auto ball : _ballManager.activeBalls()) {
            BallView::updateEffects(ball);
        }
    }
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
