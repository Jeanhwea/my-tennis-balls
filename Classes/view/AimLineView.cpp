#include "AimLineView.h"

#include <algorithm>

#include "GameConstants.h"

USING_NS_CC;

void AimLineView::init(Node *parent)
{
    _drawNode = DrawNode::create();
    parent->addChild(_drawNode, 10);
}

void AimLineView::clear()
{
    if (_drawNode) {
        _drawNode->clear();
    }
}

void AimLineView::draw(const Vec2 &dragStart, const Vec2 &delta)
{
    clear();
    float distance = delta.length();
    if (distance <= MIN_DRAG_DISTANCE) return;

    Vec2 launchDir = delta.getNormalized();
    float speed = std::min(distance * LAUNCH_FORCE_SCALE, MAX_LAUNCH_SPEED);

    // Dotted aim line
    int dots = static_cast<int>(distance / 8);
    for (int i = 0; i < dots; ++i) {
        float t = static_cast<float>(i) / dots;
        Vec2 p = dragStart + launchDir * (distance * t * 0.5f);
        float alpha = 1.0f - t * 0.6f;
        _drawNode->drawSolidCircle(p, 2.5f, 0, 8, Color4F(1, 1, 1, alpha));
    }

    // Arrowhead
    Vec2 tip = dragStart + launchDir * (distance * 0.5f);
    _drawNode->drawSolidCircle(tip, 5.0f, 0, 8, Color4F(1, 1, 0, 0.9f));

    // Power indicator (yellow → red)
    float powerRatio = speed / MAX_LAUNCH_SPEED;
    Color4F powerColor(1.0f, 1.0f - powerRatio, 0, 0.8f);
    _drawNode->drawSolidCircle(dragStart, 8.0f, 0, 16, powerColor);
}
