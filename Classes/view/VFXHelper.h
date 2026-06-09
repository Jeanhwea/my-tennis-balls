#ifndef VFX_HELPER_H
#define VFX_HELPER_H

#include "cocos2d.h"

namespace VFXHelper
{

void initParticlePool(cocos2d::Node *parent, int poolSize = 50);

void resetParticlePool();
void resetLabelPool();

void spawnHitParticle(cocos2d::Node *parent, const cocos2d::Vec2 &position);
void showFloatingScore(cocos2d::Node *parent, const cocos2d::Vec2 &position, int points);
void flashNode(cocos2d::Node *node);

}  // namespace VFXHelper

#endif
