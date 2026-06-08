/// @file InputController.h
/// Touch input controller that converts drag gestures into ball launch commands. Supports drag callbacks, launch callbacks, and launch zone constraints.

#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__

#include <functional>

#include "cocos2d.h"

/// @class InputController
/// Handles touch input, converting drag gestures into launch commands.
class InputController
{
public:
    struct LaunchCommand {
        cocos2d::Vec2 position;
        cocos2d::Vec2 velocity;
    };

    using OnLaunchCallback = std::function<void(const LaunchCommand &)>;
    using OnDragCallback = std::function<void(const cocos2d::Vec2 &start, const cocos2d::Vec2 &delta)>;
    using OnDragEndCallback = std::function<void()>;

    void setOnLaunch(OnLaunchCallback cb) { _onLaunch = std::move(cb); }

    void setOnDrag(OnDragCallback cb) { _onDrag = std::move(cb); }

    void setOnDragEnd(OnDragEndCallback cb) { _onDragEnd = std::move(cb); }

    /// Set the minimum X coordinate for valid launch start positions (right-side zone).
    void setLaunchZoneMinX(float minX) { _launchZoneMinX = minX; }

    /// Register touch listeners on the specified node.
    void init(cocos2d::Node *target);

private:
    bool _isDragging = false;
    cocos2d::Vec2 _dragStart;
    float _launchZoneMinX = 0.0f;

    OnLaunchCallback _onLaunch;
    OnDragCallback _onDrag;
    OnDragEndCallback _onDragEnd;

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif  // __INPUT_CONTROLLER_H__
