#include "GameController.h"

#include <algorithm>

#include "Classes/GameConstants.h"
#include "Classes/View/ArenaView.h"
#include "Classes/View/BallView.h"
#include "Classes/View/ObstacleView.h"
#include "Classes/View/VFXHelper.h"

USING_NS_CC;

void GameController::init(Scene *scene, const Size &visibleSize)
{
    _scene = scene;
    _visibleSize = visibleSize;

    // Views
    ArenaView::addEdgeWalls(_scene, _visibleSize);
    ArenaView::addFloorSensor(_scene, _visibleSize);
    ObstacleView::spawnObstacles(_scene, _visibleSize, OBSTACLE_COUNT);

    _aimLine.init(_scene);

    _hud = HUD::create(_visibleSize);
    _scene->addChild(_hud, 20);

    // Model → View binding
    _model.scoreManager().setOnChange([this]() { refreshHUD(); });

    // Input
    setupInput();
    setupPhysics();
}

void GameController::update(float dt)
{
    _model.tick(dt);
}

// ── Wiring ──────────────────────────────────────────────────────

void GameController::setupInput()
{
    _input.setOnDrag([this](const Vec2 &start, const Vec2 &delta) { _aimLine.draw(start, delta); });

    _input.setOnDragEnd([this]() { _aimLine.clear(); });

    _input.setOnLaunch([this](const InputController::LaunchCommand &cmd) {
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
    _hud->updateScore(_model.scoreManager().score());
    _hud->updateCombo(_model.scoreManager().combo());
    _hud->updateBallCount(countBalls(), MAX_BALLS);
}

// ── Ball management ─────────────────────────────────────────────

int GameController::countBalls() const
{
    int count = 0;
    for (auto child : _scene->getChildren()) {
        if (child->getTag() == TAG_BALL) {
            ++count;
        }
    }
    return count;
}

void GameController::spawnBall(const Vec2 &position, const Vec2 &velocity)
{
    if (countBalls() >= MAX_BALLS) return;

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

// ── Physics callbacks ───────────────────────────────────────────

bool GameController::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return true;

    auto &score = _model.scoreManager();

    // Ball fell through floor
    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_FLOOR) ||
        (nodeA->getTag() == TAG_FLOOR && nodeB->getTag() == TAG_BALL)) {
        auto ball = (nodeA->getTag() == TAG_BALL) ? nodeA : nodeB;
        removeBall(ball);
        score.resetCombo();
        return false;
    }

    // Ball hits obstacle
    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_OBSTACLE) ||
        (nodeA->getTag() == TAG_OBSTACLE && nodeB->getTag() == TAG_BALL)) {
        auto obstacle = (nodeA->getTag() == TAG_OBSTACLE) ? nodeA : nodeB;
        auto cp = contact.getContactData()->points[0];
        int points = SCORE_PER_HIT * std::max(1, score.combo());

        score.addScore(SCORE_PER_HIT);
        VFXHelper::showFloatingScore(_scene, Vec2(cp.x, cp.y), points);
        VFXHelper::spawnHitParticle(_scene, Vec2(cp.x, cp.y));
        VFXHelper::flashNode(obstacle);
        return true;
    }

    // Ball hits ball
    if (nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_BALL) {
        auto cp = contact.getContactData()->points[0];
        int points = SCORE_PER_HIT / 2 * std::max(1, score.combo());

        score.addScore(SCORE_PER_HIT / 2);
        VFXHelper::showFloatingScore(_scene, Vec2(cp.x, cp.y), points);
        VFXHelper::spawnHitParticle(_scene, Vec2(cp.x, cp.y));
        return true;
    }

    return true;
}
