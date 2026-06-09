#include "GameSceneBuilder.h"
#include "view/ArenaView.h"

USING_NS_CC;

namespace GameSceneBuilder
{

void setupArena(Scene *scene, const Size &visibleSize)
{
    ArenaView::addEdgeWalls(scene, visibleSize);
    ArenaView::addFloorSensor(scene, visibleSize);
    ArenaView::drawZones(scene, visibleSize);
}

void setupPhysics(Scene *scene,
                  const std::function<bool(PhysicsContact &)> &onContactBegin)
{
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = onContactBegin;
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, scene);
}

}
