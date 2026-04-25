#ifndef __VFX_HELPER_H__
#define __VFX_HELPER_H__

#include "cocos2d.h"

/// 无状态视觉特效工具集。
namespace VFXHelper
{

/// 初始化粒子池，预分配 DrawNode 对象以避免运行时分配开销。
/// @param parent 粒子池节点的父节点
/// @param poolSize 池大小，默认 50
void initParticlePool(cocos2d::Node *parent, int poolSize = 50);

void spawnHitParticle(cocos2d::Node *parent, const cocos2d::Vec2 &position);
void showFloatingScore(cocos2d::Node *parent, const cocos2d::Vec2 &position, int points);
void flashNode(cocos2d::Node *node);

}  // namespace VFXHelper

#endif  // __VFX_HELPER_H__
