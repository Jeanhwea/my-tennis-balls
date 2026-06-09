/// @file BallManager.h
/// Ball and target lifecycle manager: creation, tracking, removal, visual effects, out-of-bounds collection.

#ifndef __BALL_MANAGER_H__
#define __BALL_MANAGER_H__

#include <functional>

#include "cocos2d.h"

/// @class BallManager
/// Manages ball and target lifecycle: creation, tracking, visual effects, out-of-bounds collection, and removal.
/// Callbacks: _despawnCb fires after each despawn to let GameController refresh HUD/check state.
class BallManager
{
public:
    void setDespawnCallback(std::function<void()> cb) { _despawnCb = std::move(cb); }

    void initFromLevel();

    void spawnBall(cocos2d::Node *parent, const cocos2d::Vec2 &position,
                   const cocos2d::Vec2 &velocity, int ballIndex);

    void updateEffects();

    void scheduleRemoval(cocos2d::Node *node) { _pendingRemoval.push_back(node); }

    void collectOutOfBounds(cocos2d::Node *scene, const cocos2d::Size &visibleSize);

    void processPendingRemovals();

    void clear();

    int ballCount() const { return static_cast<int>(_activeBalls.size()); }

    const cocos2d::Vector<cocos2d::Node *> &activeBalls() const { return _activeBalls; }

private:
    cocos2d::Vector<cocos2d::Node *> _activeBalls;
    std::vector<cocos2d::Node *> _pendingRemoval;

    std::function<void()> _despawnCb;
};

#endif  // __BALL_MANAGER_H__
