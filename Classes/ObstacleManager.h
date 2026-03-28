#ifndef __OBSTACLE_MANAGER_H__
#define __OBSTACLE_MANAGER_H__

#include "cocos2d.h"

namespace ObstacleManager
{

void spawnObstacles(cocos2d::Node *parent, const cocos2d::Size &visibleSize, int count);

}  // namespace ObstacleManager

#endif  // __OBSTACLE_MANAGER_H__
