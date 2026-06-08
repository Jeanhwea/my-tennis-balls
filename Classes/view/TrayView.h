/// @file TrayView.h
/// Tray rack and target ball rendering. Create tray physics bodies, visual effects, and collidable target
/// balls at the corresponding positions based on LevelData.

#ifndef __TRAY_VIEW_H__
#define __TRAY_VIEW_H__

#include "cocos2d.h"
#include "model/LevelData.h"

/// @namespace TrayView
/// Draw tray racks and generate target balls based on level data.
namespace TrayView
{

/// Create all trays and target balls in the level.
/// @param outTargets Output parameter for storing created target ball node pointers
/// @return Total number of target balls
int createFromLevel(cocos2d::Node *parent, const cocos2d::Size &visibleSize, const LevelData &level,
                    cocos2d::Vector<cocos2d::Node *> &outTargets);

}  // namespace TrayView

#endif  // __TRAY_VIEW_H__
