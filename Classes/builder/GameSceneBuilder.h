#ifndef GAME_SCENE_BUILDER_H
#define GAME_SCENE_BUILDER_H

#include "cocos2d.h"

namespace GameSceneBuilder
{

void setupArena(cocos2d::Scene *scene, const cocos2d::Size &visibleSize);
void setupPhysics(cocos2d::Scene *scene,
                  const std::function<bool(cocos2d::PhysicsContact &)> &onContactBegin);

}

#endif  // GAME_SCENE_BUILDER_H
