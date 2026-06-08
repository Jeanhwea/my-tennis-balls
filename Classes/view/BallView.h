/// @file BallView.h
/// Ball sprite factory. Responsible for ball creation, removal animation, and visual effect updates
/// for highlights, shadows, glow, and motion blur.

#ifndef __BALL_VIEW_H__
#define __BALL_VIEW_H__

#include "cocos2d.h"

/// @namespace BallView
/// Ball sprite creation and removal animations.
namespace BallView
{

/// Create a ball sprite with physics body, add to parent, play spawn animation.
cocos2d::Sprite *spawn(cocos2d::Node *parent, const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity,
                       int ballIndex);

/// Play removal animation then remove from parent.
void despawn(cocos2d::Node *ball, const std::function<void()> &onComplete = nullptr);

/// Update highlight position (compute highlight location on the ball surface based on rotation,
/// simulating a fixed light source).
void updateHighlights(cocos2d::Node *ball);

/// Update motion blur effect (show trail at high speed).
void updateMotionBlur(cocos2d::Node *ball, cocos2d::Node *blurNode);

}  // namespace BallView

#endif  // __BALL_VIEW_H__
