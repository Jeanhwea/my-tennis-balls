#ifndef TRAY_VIEW_H
#define TRAY_VIEW_H

#include "cocos2d.h"
#include "model/LevelData.h"

namespace TrayView
{

int createFromLevel(cocos2d::Node *parent, const cocos2d::Size &visibleSize, const LevelData &level,
                    cocos2d::Vector<cocos2d::Node *> &outTargets);

}  // namespace TrayView

#endif
