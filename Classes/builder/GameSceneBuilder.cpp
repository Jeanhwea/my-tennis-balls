#include "GameSceneBuilder.h"
#include "common/GameConstants.h"
#include "controller/InputController.h"
#include "view/ArenaView.h"
#include "view/HUD.h"

USING_NS_CC;

namespace GameSceneBuilder
{

SceneContext createSceneContext(const std::string &levelFile)
{
    SceneContext ctx;
    ctx.scene = Scene::createWithPhysics();
    ctx.visibleSize = Director::getInstance()->getVisibleSize();
    return ctx;
}

void setupArena(Scene *scene, const Size &visibleSize)
{
    ArenaView::addEdgeWalls(scene, visibleSize);
    ArenaView::addFloorSensor(scene, visibleSize);
    ArenaView::drawZones(scene, visibleSize);
}

Node *createHUD(Scene *scene, const Size &visibleSize,
                const std::function<void()> &onBack)
{
    auto hud = HUD::create(visibleSize);
    hud->setOverlayParent(scene);
    scene->addChild(hud, 20);
    hud->setOnBack(onBack);
    return hud;
}

InputController *createInputController(Scene *scene, const Size &visibleSize)
{
    auto input = new InputController();
    input->setLaunchZoneMinX(visibleSize.width * (1.0f - LAUNCH_ZONE_RATIO));
    input->init(scene);
    return input;
}

void setupPhysics(Scene *scene,
                  const std::function<bool(PhysicsContact &)> &onContactBegin)
{
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = onContactBegin;
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, scene);
}

}  // namespace GameSceneBuilder
