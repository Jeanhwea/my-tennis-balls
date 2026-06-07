/// @file AimLineView.h
/// 拖拽瞄准线视图。渲染虚线弹道轨迹、力度指示圆环和末端落点标记。

#ifndef __AIM_LINE_VIEW_H__
#define __AIM_LINE_VIEW_H__

#include "cocos2d.h"

/// @class AimLineView
/// 渲染拖拽发射的瞄准线和力度指示器。
class AimLineView
{
public:
    /// 创建内部 DrawNode 并添加到父节点。
    void init(cocos2d::Node *parent);

    /// 从拖拽起点根据当前触摸偏移绘制瞄准线。
    void draw(const cocos2d::Vec2 &dragStart, const cocos2d::Vec2 &delta);

    /// 清除瞄准线。
    void clear();

private:
    cocos2d::DrawNode *_drawNode = nullptr;
};

#endif  // __AIM_LINE_VIEW_H__
