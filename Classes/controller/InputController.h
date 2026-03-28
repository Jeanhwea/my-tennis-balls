#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__

#include <functional>

#include "cocos2d.h"

/// Handles touch input, translates drag gestures into launch commands.
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

    /// Register touch listener on the given node.
    void init(cocos2d::Node *target);

private:
    bool _isDragging = false;
    cocos2d::Vec2 _dragStart;

    OnLaunchCallback _onLaunch;
    OnDragCallback _onDrag;
    OnDragEndCallback _onDragEnd;

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif  // __INPUT_CONTROLLER_H__
