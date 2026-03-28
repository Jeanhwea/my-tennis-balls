#ifndef __TRAY_VIEW_H__
#define __TRAY_VIEW_H__

#include "cocos2d.h"

/// Draws the tray shelf and spawns target balls on it.
namespace TrayView {

/// Create the tray shelf (static physics body) and target balls on top.
void create(cocos2d::Node *parent, const cocos2d::Size &visibleSize, int targetCount);

}  // namespace TrayView

#endif  // __TRAY_VIEW_H__
