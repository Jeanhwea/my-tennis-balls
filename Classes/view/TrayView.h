#ifndef __TRAY_VIEW_H__
#define __TRAY_VIEW_H__

#include "cocos2d.h"
#include "model/LevelData.h"

/// Draws tray shelves and spawns target balls based on level data.
namespace TrayView
{

/// Create all trays and targets for a level. Returns total target count.
int createFromLevel(cocos2d::Node *parent, const cocos2d::Size &visibleSize, const LevelData &level);

}  // namespace TrayView

#endif  // __TRAY_VIEW_H__
