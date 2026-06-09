/// @file CollisionSystem.h
/// 处理物理碰撞检测和分发的模块，负责碰撞回调分发到各个处理逻辑。

#ifndef __COLLISION_SYSTEM_H__
#define __COLLISION_SYSTEM_H__

#include <functional>

#include "cocos2d.h"
#include "model/GameModel.h"

/// @class CollisionSystem
/// 碰撞检测分发系统，接收物理引擎的碰撞回调并将其分发到对应的处理函数。
class CollisionSystem
{
public:
    void setModel(GameModel *model) { _model = model; }
    void setSceneNode(cocos2d::Node *scene) { _scene = scene; }

    /// 设置延迟移除回调，当碰撞需要移除节点时调用
    void setScheduledRemovalCallback(std::function<void(cocos2d::Node *)> cb)
    {
        _scheduledRemovalCb = std::move(cb);
    }

    /// 物理碰撞回调入口
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
