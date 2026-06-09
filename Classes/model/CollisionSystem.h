/// @file CollisionSystem.h
/// Physics collision event dispatcher.

#ifndef __COLLISION_SYSTEM_H__
#define __COLLISION_SYSTEM_H__

#include <functional>

#include "cocos2d.h"
#include "model/GameModel.h"

/// @class CollisionSystem
class CollisionSystem
{
public:
    void setModel(GameModel *model) { _model = model; }
    void setSceneNode(cocos2d::Node *scene) { _scene = scene; }

    void setScheduledRemovalCallback(std::function<void(cocos2d::Node *)> cb)
    {
        _scheduledRemovalCb = std::move(cb);
    }

    bool onContactBegin(cocos2d::PhysicsContact &contact);

private:
    GameModel *_model = nullptr;
    cocos2d::Node *_scene = nullptr;
    std::function<void(cocos2d::Node *)> _scheduledRemovalCb;

    bool handleFloorContact(cocos2d::Node *floor, cocos2d::Node *other, cocos2d::PhysicsContact &contact);
    bool handleBallTargetContact(cocos2d::Node *ball, cocos2d::Node *target, cocos2d::PhysicsContact &contact);
    bool handleBallBallContact(cocos2d::Node *a, cocos2d::Node *b, cocos2d::PhysicsContact &contact);
};

#endif  // __COLLISION_SYSTEM_H__
