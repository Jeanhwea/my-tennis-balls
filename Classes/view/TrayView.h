#ifndef __TRAY_VIEW_H__
#define __TRAY_VIEW_H__

#include "cocos2d.h"
#include "model/LevelData.h"

/// 绘制托盘架并根据关卡数据生成目标球。
namespace TrayView
{

/// 创建关卡中的所有托盘和目标球。
/// @param outTargets 输出参数，用于存储创建的目标球节点指针
/// @return 目标球总数
int createFromLevel(cocos2d::Node *parent, const cocos2d::Size &visibleSize, const LevelData &level,
                    cocos2d::Vector<cocos2d::Node *> &outTargets);

}  // namespace TrayView

#endif  // __TRAY_VIEW_H__
