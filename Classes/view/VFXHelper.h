/// @file VFXHelper.h
/// Stateless visual effects utility set. Provides common effects such as particle pool, floating score, hit effects, and node flash.

#ifndef __VFX_HELPER_H__
#define __VFX_HELPER_H__

#include "cocos2d.h"

/// @namespace VFXHelper
/// Stateless visual effects utility set.
namespace VFXHelper
{

/// Initialize the particle pool, pre-allocate DrawNode objects to avoid runtime allocation overhead.
/// @param parent Parent node for the particle pool node
/// @param poolSize Pool size, defaults to 50
void initParticlePool(cocos2d::Node *parent, int poolSize = 50);

void spawnHitParticle(cocos2d::Node *parent, const cocos2d::Vec2 &position);
void showFloatingScore(cocos2d::Node *parent, const cocos2d::Vec2 &position, int points);
void flashNode(cocos2d::Node *node);

}  // namespace VFXHelper

#endif  // __VFX_HELPER_H__
