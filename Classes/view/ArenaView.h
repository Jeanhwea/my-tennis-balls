#ifndef __ARENA_VIEW_H__
#define __ARENA_VIEW_H__

#include "cocos2d.h"

/// 创建物理边界墙、地板传感器和区域视觉效果。
namespace ArenaView
{

void addEdgeWalls(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void addFloorSensor(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void drawZones(cocos2d::Node *parent, const cocos2d::Size &visibleSize);

}  // namespace ArenaView

#endif  // __ARENA_VIEW_H__
