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

void BallManager::collectOutOfBounds(Node *scene, const Size &visibleSize)
{
    for (auto child : scene->getChildren()) {
        if (child->getTag() == TAG_TARGET && child->getPositionY() < OOB_BOTTOM) {
            _pendingRemoval.push_back(child);
        }
    }
    for (auto ball : _activeBalls) {
        float x = ball->getPositionX();
        float y = ball->getPositionY();
        if (y < OOB_BOTTOM || y > visibleSize.height + OOB_TOP_MARGIN || x < -OOB_SIDE_MARGIN ||
            x > visibleSize.width + OOB_SIDE_MARGIN) {
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
