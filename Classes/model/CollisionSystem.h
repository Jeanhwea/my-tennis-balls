#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <functional>
#include "cocos2d.h"

class CollisionSystem
{
public:
    using NodeCallback = std::function<void(cocos2d::Node *)>;
    using ScoreCallback = std::function<void(cocos2d::Node *scene, const cocos2d::Vec2 &pos, int basePoints)>;
    using HitCallback = std::function<void(cocos2d::Node *scene, const cocos2d::Vec2 &pos)>;
    using VoidCallback = std::function<void()>;

    void setOnScheduledRemoval(NodeCallback cb) { _scheduledRemovalCb = std::move(cb); }
    void setOnScore(ScoreCallback cb) { _scoreCb = std::move(cb); }
    void setOnHitParticle(HitCallback cb) { _hitParticleCb = std::move(cb); }
    void setOnComboReset(VoidCallback cb) { _comboResetCb = std::move(cb); }
    void setOnTargetRemoved(VoidCallback cb) { _targetRemovedCb = std::move(cb); }

    bool onContactBegin(cocos2d::PhysicsContact &contact);

private:
    std::function<void(cocos2d::Node *)> _scheduledRemovalCb;
    ScoreCallback _scoreCb;
    HitCallback _hitParticleCb;
    VoidCallback _comboResetCb;
    VoidCallback _targetRemovedCb;

    bool handleFloorContact(cocos2d::Node *floor, cocos2d::Node *other, cocos2d::PhysicsContact &contact);
    bool handleBallTargetContact(cocos2d::Node *ball, cocos2d::Node *target, cocos2d::PhysicsContact &contact);
    bool handleBallBallContact(cocos2d::Node *a, cocos2d::Node *b, cocos2d::PhysicsContact &contact);
};

#endif  // COLLISION_SYSTEM_H