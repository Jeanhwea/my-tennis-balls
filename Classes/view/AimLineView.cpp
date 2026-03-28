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
    float powerRatio = speed / MAX_LAUNCH_SPEED;

    // Gradient dotted trail
    int dots = static_cast<int>(distance / 10);
    dots = std::min(dots, 40);
    for (int i = 0; i < dots; ++i) {
        float t = static_cast<float>(i) / dots;
        Vec2 p = dragStart + launchDir * (distance * t * 0.5f);
        float alpha = (1.0f - t * 0.7f) * 0.8f;
        float radius = 3.0f - t * 1.5f;
        // Color shifts from white to yellow/orange based on power
        Color4F dotColor(1.0f, 1.0f - powerRatio * t * 0.5f, 1.0f - powerRatio * t, alpha);
        _drawNode->drawSolidCircle(p, std::max(radius, 1.0f), 0, 8, dotColor);
    }

    // Arrowhead (brighter, larger)
    Vec2 tip = dragStart + launchDir * (distance * 0.5f);
    Color4F tipColor(1.0f, 0.9f - powerRatio * 0.4f, 0.3f, 0.95f);
    _drawNode->drawSolidCircle(tip, 6.0f, 0, 12, tipColor);
    _drawNode->drawSolidCircle(tip, 3.0f, 0, 8, Color4F(1, 1, 1, 0.9f));

    // Power ring at drag origin
    Color4F ringColor(1.0f, 1.0f - powerRatio, 0.2f, 0.6f);
    _drawNode->drawCircle(dragStart, 12.0f, 0, 24, false, ringColor);
    _drawNode->drawSolidCircle(dragStart, 4.0f, 0, 8, Color4F(1, 1, 1, 0.7f));
}
