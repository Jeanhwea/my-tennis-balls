#ifndef __BALL_VIEW_H__
#define __BALL_VIEW_H__

#include "cocos2d.h"

/// Ball sprite creation and removal animations.
namespace BallView
{

/// Create a ball sprite with physics body, add to parent, play spawn animation.
cocos2d::Sprite *spawn(cocos2d::Node *parent, const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity,
                       int ballIndex);

/// Play removal animation then remove from parent.
void despawn(cocos2d::Node *ball, const std::function<void()> &onComplete = nullptr);

}  // namespace BallView

#endif  // __BALL_VIEW_H__
