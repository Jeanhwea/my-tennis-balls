#ifndef __VFX_HELPER_H__
#define __VFX_HELPER_H__

#include "cocos2d.h"

/// 无状态视觉特效工具集。
namespace VFXHelper
{

void spawnHitParticle(cocos2d::Node *parent, const cocos2d::Vec2 &position);
void showFloatingScore(cocos2d::Node *parent, const cocos2d::Vec2 &position, int points);
void flashNode(cocos2d::Node *node);

}  // namespace VFXHelper

#endif  // __VFX_HELPER_H__
