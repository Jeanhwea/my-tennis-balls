#ifndef GAME_SCENE_BUILDER_H
#define GAME_SCENE_BUILDER_H

#include "cocos2d.h"
#include "model/GameModel.h"
#include "model/LevelData.h"

class InputController;

namespace GameSceneBuilder
{

struct SceneContext {
    cocos2d::Scene *scene = nullptr;
    cocos2d::Size visibleSize;
};

SceneContext createSceneContext(const std::string &levelFile);
void setupArena(cocos2d::Scene *scene, const cocos2d::Size &visibleSize);
cocos2d::Node *createHUD(cocos2d::Scene *scene, const cocos2d::Size &visibleSize,
                         const std::function<void()> &onBack);
InputController *createInputController(cocos2d::Scene *scene, const cocos2d::Size &visibleSize);
void setupPhysics(cocos2d::Scene *scene,
                  const std::function<bool(cocos2d::PhysicsContact &)> &onContactBegin);

}  // namespace GameSceneBuilder

#endif  // GAME_SCENE_BUILDER_H
