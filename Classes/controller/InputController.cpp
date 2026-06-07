#include "InputController.h"

#include <algorithm>

#include "common/GameConstants.h"

USING_NS_CC;

// ── 初始化触摸监听 ──

void InputController::init(Node *target)
{
    // 注册单点触摸事件监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch *t, Event *e) { return onTouchBegan(t, e); };
    listener->onTouchMoved = [this](Touch *t, Event *e) { onTouchMoved(t, e); };
    listener->onTouchEnded = [this](Touch *t, Event *e) { onTouchEnded(t, e); };
    target->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
}

// ── 触摸开始 ──

bool InputController::onTouchBegan(Touch *touch, Event * /*event*/)
{
    // 仅在发射区域（右侧 LAUNCH 区域）内触摸才生效
    auto pos = touch->getLocation();
    if (pos.x < _launchZoneMinX) {
        _isDragging = false;
        return false;
    }
    _isDragging = true;
    _dragStart = pos;
    return true;
}

// ── 触摸移动（拖拽瞄准） ──

void InputController::onTouchMoved(Touch *touch, Event * /*event*/)
{
    if (!_isDragging) return;
    // 拖拽偏移 = 起始位置 - 当前位置（反向即发射方向）
    Vec2 delta = _dragStart - touch->getLocation();
    if (_onDrag) {
        _onDrag(_dragStart, delta);
    }
}

// ── 触摸结束（发射弹球） ──

void InputController::onTouchEnded(Touch *touch, Event * /*event*/)
{
    if (!_isDragging) return;
    _isDragging = false;

    // 根据拖拽距离计算发射速度和方向
    Vec2 delta = _dragStart - touch->getLocation();
    float distance = delta.length();

    LaunchCommand cmd;
    cmd.position = _dragStart;

    if (distance < MIN_DRAG_DISTANCE) {
        // 拖拽距离过小，视为无效发射
        cmd.velocity = Vec2::ZERO;
    } else {
        Vec2 dir = delta.getNormalized();
        // 速度与拖拽距离成正比，但不超过上限
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
