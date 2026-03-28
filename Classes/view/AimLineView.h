#ifndef __AIM_LINE_VIEW_H__
#define __AIM_LINE_VIEW_H__

#include "cocos2d.h"

/// Renders the drag-to-launch aim line and power indicator.
class AimLineView
{
public:
    /// Create the internal DrawNode and add it to parent.
    void init(cocos2d::Node *parent);

    /// Draw aim line from dragStart given current touch delta.
    void draw(const cocos2d::Vec2 &dragStart, const cocos2d::Vec2 &delta);

    /// Clear the aim line.
    void clear();

private:
    cocos2d::DrawNode *_drawNode = nullptr;
};

#endif  // __AIM_LINE_VIEW_H__
