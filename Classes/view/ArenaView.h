/// @file ArenaView.h
/// 竞技场视图。创建物理边界墙、地板传感器、背景渐变、网格线、发射区域和装饰效果。

#ifndef __ARENA_VIEW_H__
#define __ARENA_VIEW_H__

#include "cocos2d.h"

/// @namespace ArenaView
/// 创建物理边界墙、地板传感器和区域视觉效果。
namespace ArenaView
{

void addEdgeWalls(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void addFloorSensor(cocos2d::Node *parent, const cocos2d::Size &visibleSize);
void drawZones(cocos2d::Node *parent, const cocos2d::Size &visibleSize);

}  // namespace ArenaView

#endif  // __ARENA_VIEW_H__
