#include "InputController.h"

#include <algorithm>

#include "common/GameConstants.h"

USING_NS_CC;

// -- Initialize touch listener --

void InputController::init(Node *target)
{
    // Register single-point touch event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch *t, Event *e) { return onTouchBegan(t, e); };
    listener->onTouchMoved = [this](Touch *t, Event *e) { onTouchMoved(t, e); };
    listener->onTouchEnded = [this](Touch *t, Event *e) { onTouchEnded(t, e); };
    target->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
}

// -- Touch began --

bool InputController::onTouchBegan(Touch *touch, Event * /*event*/)
{
    // Only register touches within the launch zone (right-side LAUNCH area)
    auto pos = touch->getLocation();
    if (pos.x < _launchZoneMinX) {
        _isDragging = false;
        return false;
    }
    _isDragging = true;
    _dragStart = pos;
    return true;
}

// -- Touch moved (drag to aim) --

void InputController::onTouchMoved(Touch *touch, Event * /*event*/)
{
    if (!_isDragging) return;
    // Drag offset = start position - current position (reversed gives launch direction)
    Vec2 delta = _dragStart - touch->getLocation();
    if (_onDrag) {
        _onDrag(_dragStart, delta);
    }
}

// -- Touch ended (launch ball) --

void InputController::onTouchEnded(Touch *touch, Event * /*event*/)
{
    if (!_isDragging) return;
    _isDragging = false;

    // Calculate launch velocity and direction from drag distance
    Vec2 delta = _dragStart - touch->getLocation();
    float distance = delta.length();

    LaunchCommand cmd;
    cmd.position = _dragStart;

    if (distance < MIN_DRAG_DISTANCE) {
        // Drag distance too small, treat as invalid launch
        cmd.velocity = Vec2::ZERO;
    } else {
        Vec2 dir = delta.getNormalized();
        // Speed is proportional to drag distance, capped at maximum
        float speed = std::min(distance * LAUNCH_FORCE_SCALE, MAX_LAUNCH_SPEED);
        cmd.velocity = dir * speed;
    }

    if (_onLaunch) {
        _onLaunch(cmd);
    }
    if (_onDragEnd) {
        _onDragEnd();
    }
}
