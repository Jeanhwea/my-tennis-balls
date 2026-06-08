/// @file ArenaView.h
/// Arena view. Creates physical boundary walls, floor sensor, background gradient, grid lines, launch zone,
/// and decorative effects.

#ifndef __ARENA_VIEW_H__
#define __ARENA_VIEW_H__

#include "cocos2d.h"

/// @namespace ArenaView
/// Create physical boundary walls, floor sensor, and zone visual effects.
namespace ArenaView
{

void addEdgeWalls(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void addFloorSensor(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void drawZones(cocos2d::Node *parent, const cocos2d::Size &visibleSize);

}  // namespace ArenaView

#endif  // __ARENA_VIEW_H__
