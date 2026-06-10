#ifndef BALL_VIEW_H
#define BALL_VIEW_H

#include "cocos2d.h"

namespace BallView
{

cocos2d::Sprite *spawn(cocos2d::Node *parent, const cocos2d::Vec2 &position, const cocos2d::Vec2 &velocity,
                       int ballIndex);

void despawn(cocos2d::Node *ball, const std::function<void()> &onComplete = nullptr);

void updateEffects(cocos2d::Node *ball);

void updateMotionBlur(cocos2d::Node *ball, cocos2d::Node *blurNode);

}  // namespace BallView

#endif  // BALL_VIEW_H