#ifndef __OBSTACLE_VIEW_H__
#define __OBSTACLE_VIEW_H__

#include "cocos2d.h"

/// Obstacle rendering and physics body creation.
namespace ObstacleView
{

void spawnObstacles(cocos2d::Node *parent, const cocos2d::Size &visibleSize, int count);

}  // namespace ObstacleView

#endif  // __OBSTACLE_VIEW_H__
