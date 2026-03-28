#ifndef __BALL_VIEW_H__
#define __BALL_VIEW_H__

#include "cocos2d.h"

/// 弹球精灵的创建和移除动画。
namespace BallView
{

/// 创建带物理体的弹球精灵，添加到父节点，播放生成动画。
cocos2d::Sprite *spawn(cocos2d::Node *parent, const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity,
                       int ballIndex);

/// 播放移除动画后从父节点移除。
void despawn(cocos2d::Node *ball, const std::function<void()> &onComplete = nullptr);

}  // namespace BallView

#endif  // __BALL_VIEW_H__
