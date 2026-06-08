#include "GameController.h"

#include <algorithm>

#include "common/GameConstants.h"
#include "scene/LevelMenuScene.h"
#include "view/ArenaView.h"
#include "view/BallView.h"
#include "view/TrayView.h"
#include "view/VFXHelper.h"

USING_NS_CC;

void GameController::init(Scene *scene, const Size &visibleSize, int startLevel)
{
    _scene = scene;
    _visibleSize = visibleSize;

    _activeBalls.clear();
    _activeTargets.clear();

    ArenaView::addEdgeWalls(_scene, _visibleSize);
    ArenaView::addFloorSensor(_scene, _visibleSize);
    ArenaView::drawZones(_scene, _visibleSize);

    _aimLine.init(_scene);

    _hud = HUD::create(_visibleSize);
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
        updateBallEffects();
        collectOutOfBounds();
        processPendingRemovals();
    }
}

void GameController::updateBallEffects()
{
    for (auto ball : _activeBalls) {
        auto name = ball->getName();
        if (name.empty()) continue;

        auto parent = ball->getParent();
        if (!parent) continue;

        auto shadow = parent->getChildByName(name + "_shadow");
        auto glow = parent->getChildByName(name + "_glow");
        auto blur = parent->getChildByName(name + "_blur");

        Vec2 ballPos = ball->getPosition();
        if (shadow) {
            shadow->setPosition(ballPos.x, ballPos.y + BALL_SHADOW_OFFSET_Y);
        }
        if (glow) {
            glow->setPosition(ballPos);
        }

        BallView::updateMotionBlur(ball, blur);
        BallView::updateHighlights(ball);
    }
}

void GameController::loadLevel(int index)
{
    if (index < 0 || index >= static_cast<int>(getAllLevels().size())) {
        CCLOG("[GameController] ERROR: level index %d out of range!", index);
        return;
    }
    _transitioning = false;
    _model.setLevelIndex(index);
    _ballCounter = 0;

    clearLevelNodes();

    const auto &level = _model.currentLevel();
    int totalTargets = TrayView::createFromLevel(_scene, _visibleSize, level, _activeTargets);
    _model.setTargetsRemaining(totalTargets);

    _hud->updateLevel(level.id, level.name);
    _hud->showLevelIntro(level.id, level.name);
    refreshHUD();
}

void GameController::clearLevelNodes()
{
    _activeBalls.clear();
    _activeTargets.clear();

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
    if (_ballCounter >= level.maxBalls && _activeBalls.empty()) {
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
        spawnBall(cmd.position, cmd.velocity);
        _hud->hideHint();
    });

    _input.init(_scene);
}

void GameController::setupPhysics()
{
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = [this](PhysicsContact &c) { return onContactBegin(c); };
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

void GameController::spawnBall(const Vec2 &position, const Vec2 &velocity)
{
    auto ball = BallView::spawn(_scene, position, velocity, ++_ballCounter);
    _activeBalls.pushBack(ball);
    _model.setBallCount(static_cast<int>(_activeBalls.size()));
    refreshHUD();
}

void GameController::removeBall(Node *ball)
{
    _activeBalls.eraseObject(ball);
    BallView::despawn(ball, [this]() {
        _model.setBallCount(static_cast<int>(_activeBalls.size()));
        refreshHUD();
        checkFailCondition();
    });
}

void GameController::removeTarget(Node *target)
{
    _activeTargets.eraseObject(target);
    BallView::despawn(target, [this]() {
        _model.setTargetsRemaining(static_cast<int>(_activeTargets.size()));
        refreshHUD();
        if (_model.isCleared()) {
            onLevelCleared();
        }
    });
}

void GameController::collectOutOfBounds()
{
    if (_activeTargets.empty() && _activeBalls.empty()) {
        return;
    }

    for (auto target : _activeTargets) {
        if (target->getPositionY() < OOB_BOTTOM) {
            _pendingRemoval.push_back(target);
        }
    }

    for (auto ball : _activeBalls) {
        float x = ball->getPositionX();
        float y = ball->getPositionY();
        if (y < OOB_BOTTOM || y > _visibleSize.height + OOB_TOP_MARGIN || x < -OOB_SIDE_MARGIN ||
            x > _visibleSize.width + OOB_SIDE_MARGIN) {
            _pendingRemoval.push_back(ball);
        }
    }
}

void GameController::processPendingRemovals()
{
    for (auto node : _pendingRemoval) {
        int tag = node->getTag();
        if (tag == TAG_TARGET) {
            removeTarget(node);
        } else if (tag == TAG_BALL) {
            removeBall(node);
        }
    }
    _pendingRemoval.clear();
}

namespace
{

void applyScoreVFX(Node *parent, ScoreManager &score, int basePoints, const Vec2 &pos)
{
    int points = basePoints * std::max(1, score.combo());
    score.addScore(basePoints);
    VFXHelper::showFloatingScore(parent, pos, points);
}

}  // namespace

bool GameController::handleFloorContact(Node * /*floor*/, Node *other, PhysicsContact &contact)
{
    auto cp = Vec2(contact.getContactData()->points[0].x, contact.getContactData()->points[0].y);

    if (other->getTag() == TAG_TARGET) {
        applyScoreVFX(_scene, _model.scoreManager(), SCORE_TARGET_FALL, cp);
        VFXHelper::spawnHitParticle(_scene, cp);
        removeTarget(other);
    } else if (other->getTag() == TAG_BALL) {
        removeBall(other);
        _model.scoreManager().resetCombo();
    }
    return false;
}

bool GameController::handleBallTargetContact(Node * /*ball*/, Node * /*target*/, PhysicsContact &contact)
{
    auto cp = Vec2(contact.getContactData()->points[0].x, contact.getContactData()->points[0].y);
    applyScoreVFX(_scene, _model.scoreManager(), SCORE_PER_HIT, cp);
    VFXHelper::spawnHitParticle(_scene, cp);
    return true;
}

bool GameController::handleBallBallContact(Node * /*a*/, Node * /*b*/, PhysicsContact &contact)
{
    auto cp = Vec2(contact.getContactData()->points[0].x, contact.getContactData()->points[0].y);
    applyScoreVFX(_scene, _model.scoreManager(), SCORE_PER_HIT / 2, cp);
    return true;
}

bool GameController::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return true;

    if (nodeA->getTag() == TAG_FLOOR || nodeB->getTag() == TAG_FLOOR) {
        auto floor = (nodeA->getTag() == TAG_FLOOR) ? nodeA : nodeB;
        auto other = (nodeA->getTag() == TAG_FLOOR) ? nodeB : nodeA;
        return handleFloorContact(floor, other, contact);
    }

    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_TARGET) ||
        (nodeA->getTag() == TAG_TARGET && nodeB->getTag() == TAG_BALL)) {
        return handleBallTargetContact(nodeA, nodeB, contact);
    }

    if (nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_BALL) {
        return handleBallBallContact(nodeA, nodeB, contact);
    }

    return true;
}
