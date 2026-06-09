#include "BallManager.h"

#include "common/GameConstants.h"
#include "view/BallView.h"

USING_NS_CC;

void BallManager::initFromLevel()
{
    _activeBalls.clear();
    _pendingRemoval.clear();
}

void BallManager::spawnBall(Node *parent, const Vec2 &position, const Vec2 &velocity, int ballIndex)
{
    auto ball = BallView::spawn(parent, position, velocity, ballIndex);
    _activeBalls.pushBack(ball);
}

void BallManager::updateEffects()
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
        if (shadow) shadow->setPosition(ballPos.x, ballPos.y + BALL_SHADOW_OFFSET_Y);
        if (glow) glow->setPosition(ballPos);

        BallView::updateMotionBlur(ball, blur);
        BallView::updateHighlights(ball);
    }
}

void BallManager::collectOutOfBounds(Node *scene, const Size &visibleSize)
{
    // Collect out-of-bounds targets (scan scene by tag)
    for (auto child : scene->getChildren()) {
        if (child->getTag() == TAG_TARGET && child->getPositionY() < OOB_BOTTOM) {
            _pendingRemoval.push_back(child);
        }
    }
    // Collect out-of-bounds balls
    for (auto ball : _activeBalls) {
        float x = ball->getPositionX();
        float y = ball->getPositionY();
        if (y < OOB_BOTTOM || y > visibleSize.height + OOB_TOP_MARGIN ||
            x < -OOB_SIDE_MARGIN || x > visibleSize.width + OOB_SIDE_MARGIN) {
            _pendingRemoval.push_back(ball);
        }
    }
}

void BallManager::processPendingRemovals()
{
    for (auto node : _pendingRemoval) {
        _activeBalls.eraseObject(node);
        BallView::despawn(node, [this]() {
            if (_despawnCb) _despawnCb();
        });
    }
    _pendingRemoval.clear();
}

void BallManager::clear()
{
    _activeBalls.clear();
    _pendingRemoval.clear();
}
