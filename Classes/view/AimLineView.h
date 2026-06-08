/// @file AimLineView.h
/// Drag aim line view. Renders dashed trajectory, power indicator ring, and end-point marker.

#ifndef __AIM_LINE_VIEW_H__
#define __AIM_LINE_VIEW_H__

#include "cocos2d.h"

/// @class AimLineView
/// Renders the drag-and-launch aim line and power indicator.
class AimLineView
{
public:
    /// Create internal DrawNode and add to parent.
    void init(cocos2d::Node *parent);

    /// Draw aim line from drag start based on current touch offset.
    void draw(const cocos2d::Vec2 &dragStart, const cocos2d::Vec2 &delta);

    /// Clear the aim line.
    void clear();

private:
    cocos2d::DrawNode *_drawNode = nullptr;
};

#endif  // __AIM_LINE_VIEW_H__
