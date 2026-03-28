#include "GameController.h"

#include <algorithm>

#include "GameConstants.h"
#include "view/ArenaView.h"
#include "view/BallView.h"
#include "view/TrayView.h"
#include "view/VFXHelper.h"

USING_NS_CC;

void GameController::init(Scene *scene, const Size &visibleSize)
{
    _scene = scene;
    _visibleSize = visibleSize;

    // Static arena (persists across levels)
    ArenaView::addEdgeWalls(_scene, _visibleSize);
    ArenaView::addFloorSensor(_scene, _visibleSize);
    ArenaView::drawZones(_scene, _visibleSize);

    _aimLine.init(_scene);

    _hud = HUD::create(_visibleSize);
    _scene->addChild(_hud, 20);

    _model.scoreManager().setOnChange([this]() { refreshHUD(); });

    setupInput();
    setupPhysics();

    // Load first level
    loadLevel(0);
}

void GameController::update(float dt)
{
    _model.tick(dt);

    // Check for targets that fell off screen (below y=0 or off sides)
    if (!_transitioning) {
        std::vector<Node *> fallen;
        for (auto child : _scene->getChildren()) {
            if (child->getTag() == TAG_TARGET && child->getPositionY() < -50.0f) {
                fallen.push_back(child);
            }
        }
        for (auto target : fallen) {
            removeTarget(target);
        }
    }
}

// ── Level management ────────────────────────────────────────────

void GameController::loadLevel(int index)
{
    _transitioning = false;
    _model.setLevelIndex(index);
    _ballCounter = 0;

    clearLevelNodes();

    const auto &level = _model.currentLevel();
    int totalTargets = TrayView::createFromLevel(_scene, _visibleSize, level);
    _model.setTargetsRemaining(totalTargets);

    _hud->updateLevel(level.id, level.name);
    _hud->showLevelIntro(level.id, level.name);
    refreshHUD();
}

void GameController::clearLevelNodes()
{
    // Remove all balls, targets, and trays from previous level
    std::vector<Node *> toRemove;
    for (auto child : _scene->getChildren()) {
        int tag = child->getTag();
        if (tag == TAG_BALL || tag == TAG_TARGET || tag == TAG_TRAY) {
            toRemove.push_back(child);
        }
    }
    for (auto node : toRemove) {
        node->removeFromParent();
    }
}

void GameController::onLevelCleared()
{
    if (_transitioning) return;
    _transitioning = true;

    _hud->showCleared();

    if (_model.hasNextLevel()) {
        int nextIdx = _model.levelIndex() + 1;
        // Delay before loading next level
        _scene->runAction(Sequence::create(
            DelayTime::create(2.5f), CallFunc::create([this, nextIdx]() { loadLevel(nextIdx); }), nullptr));
    }
    // If no next level, game stays on "ALL CLEAR!" — player wins
}

// ── Wiring ──────────────────────────────────────────────────────

void GameController::setupInput()
{
    _input.setLaunchZoneMinX(_visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO));

    _input.setOnDrag([this](const Vec2 &start, const Vec2 &delta) { _aimLine.draw(start, delta); });

    _input.setOnDragEnd([this]() { _aimLine.clear(); });

    _input.setOnLaunch([this](const InputController::LaunchCommand &cmd) {
        if (_transitioning) return;
        const auto &level = _model.currentLevel();
        if (countBalls() >= level.maxBalls) return;
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
    _hud->updateBallCount(countBalls(), level.maxBalls);
    _hud->updateTargets(_model.targetsRemaining());
}

// ── Ball management ─────────────────────────────────────────────

int GameController::countBalls() const
{
    int count = 0;
    for (auto child : _scene->getChildren()) {
        if (child->getTag() == TAG_BALL) ++count;
    }
    return count;
}

int GameController::countTargets() const
{
    int count = 0;
    for (auto child : _scene->getChildren()) {
        if (child->getTag() == TAG_TARGET) ++count;
    }
    return count;
}

void GameController::spawnBall(const Vec2 &position, const Vec2 &velocity)
{
    BallView::spawn(_scene, position, velocity, ++_ballCounter);
    _model.setBallCount(countBalls());
    refreshHUD();
}

void GameController::removeBall(Node *ball)
{
    BallView::despawn(ball, [this]() {
        _model.setBallCount(countBalls());
        refreshHUD();
    });
}

void GameController::removeTarget(Node *target)
{
    BallView::despawn(target, [this]() {
        _model.setTargetsRemaining(countTargets());
        refreshHUD();
        if (_model.isCleared()) {
            onLevelCleared();
        }
    });
}

// ── Physics callbacks ───────────────────────────────────────────

bool GameController::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return true;

    auto &score = _model.scoreManager();

    // Floor sensor
    bool aFloor = nodeA->getTag() == TAG_FLOOR;
    bool bFloor = nodeB->getTag() == TAG_FLOOR;
    if (aFloor || bFloor) {
        auto other = aFloor ? nodeB : nodeA;
        if (other->getTag() == TAG_TARGET) {
            auto cp = contact.getContactData()->points[0];
            int points = SCORE_TARGET_FALL * std::max(1, score.combo());
            score.addScore(SCORE_TARGET_FALL);
            VFXHelper::showFloatingScore(_scene, Vec2(cp.x, cp.y), points);
            VFXHelper::spawnHitParticle(_scene, Vec2(cp.x, cp.y));
            removeTarget(other);
        } else if (other->getTag() == TAG_BALL) {
            removeBall(other);
            score.resetCombo();
        }
        return false;
    }

    // Ball hits target
    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_TARGET) ||
        (nodeA->getTag() == TAG_TARGET && nodeB->getTag() == TAG_BALL)) {
        auto target = (nodeA->getTag() == TAG_TARGET) ? nodeA : nodeB;
        auto cp = contact.getContactData()->points[0];
        int points = SCORE_PER_HIT * std::max(1, score.combo());

        score.addScore(SCORE_PER_HIT);
        VFXHelper::showFloatingScore(_scene, Vec2(cp.x, cp.y), points);
        VFXHelper::spawnHitParticle(_scene, Vec2(cp.x, cp.y));
        VFXHelper::flashNode(target);
        return true;
    }

    // Ball hits ball
    if (nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_BALL) {
        auto cp = contact.getContactData()->points[0];
        int points = SCORE_PER_HIT / 2 * std::max(1, score.combo());
        score.addScore(SCORE_PER_HIT / 2);
        VFXHelper::showFloatingScore(_scene, Vec2(cp.x, cp.y), points);
        return true;
    }

    return true;
}
