
#ifndef ARENA_VIEW_H
#define ARENA_VIEW_H

#include "cocos2d.h"

namespace ArenaView
{

void addEdgeWalls(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void addFloorSensor(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void drawZones(cocos2d::Node *parent, const cocos2d::Size &visibleSize);

}  // namespace ArenaView

#endif
