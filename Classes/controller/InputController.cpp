#include "InputController.h"

#include <algorithm>

#include "common/GameConstants.h"

USING_NS_CC;

void InputController::init(Node *target)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch *t, Event *e) { return onTouchBegan(t, e); };
    listener->onTouchMoved = [this](Touch *t, Event *e) { onTouchMoved(t, e); };
    listener->onTouchEnded = [this](Touch *t, Event *e) { onTouchEnded(t, e); };
    target->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
}

bool InputController::onTouchBegan(Touch *touch, Event * /*event*/)
{
    auto pos = touch->getLocation();
    // 忽略发射区域外的触摸
    if (pos.x < _launchZoneMinX) {
        _isDragging = false;
        return false;
    }
    _isDragging = true;
    _dragStart = pos;
    return true;
}

void InputController::onTouchMoved(Touch *touch, Event * /*event*/)
{
    if (!_isDragging) return;
    Vec2 delta = _dragStart - touch->getLocation();
    if (_onDrag) {
        _onDrag(_dragStart, delta);
    }
}

void InputController::onTouchEnded(Touch *touch, Event * /*event*/)
{
    if (!_isDragging) return;
    _isDragging = false;

    Vec2 delta = _dragStart - touch->getLocation();
    float distance = delta.length();

    LaunchCommand cmd;
    cmd.position = _dragStart;

    if (distance < MIN_DRAG_DISTANCE) {
        cmd.velocity = Vec2::ZERO;
    } else {
        Vec2 dir = delta.getNormalized();
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
