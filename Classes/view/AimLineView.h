
#ifndef AIM_LINE_VIEW_H
#define AIM_LINE_VIEW_H

#include "cocos2d.h"

class AimLineView
{
public:
    void init(cocos2d::Node *parent);

    void draw(const cocos2d::Vec2 &dragStart, const cocos2d::Vec2 &delta);

    void clear();

private:
    cocos2d::DrawNode *_drawNode = nullptr;
};

#endif
