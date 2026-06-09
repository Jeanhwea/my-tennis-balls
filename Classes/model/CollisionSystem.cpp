#include "CollisionSystem.h"

#include "common/GameConstants.h"

USING_NS_CC;

bool CollisionSystem::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (!nodeA || !nodeB) return true;

    if (nodeA->getTag() == TAG_FLOOR || nodeB->getTag() == TAG_FLOOR) {
        auto floor = (nodeA->getTag() == TAG_FLOOR) ? nodeA : nodeB;
        auto other = (nodeA->getTag() == TAG_FLOOR) ? nodeB : nodeA;
        return handleFloorContact(floor, other, contact);
    }

    if ((nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_TARGET) ||
        (nodeA->getTag() == TAG_TARGET && nodeB->getTag() == TAG_BALL)) {
        return handleBallTargetContact(nodeA, nodeB, contact);
    }

    if (nodeA->getTag() == TAG_BALL && nodeB->getTag() == TAG_BALL) {
        return handleBallBallContact(nodeA, nodeB, contact);
    }

    return true;
}

bool CollisionSystem::handleFloorContact(Node * /*floor*/, Node *other, PhysicsContact &contact)
{
    auto cp = Vec2(contact.getContactData()->points[0].x, contact.getContactData()->points[0].y);

    if (other->getTag() == TAG_TARGET) {
        if (_scoreCb) _scoreCb(nullptr, cp, SCORE_TARGET_FALL);
        if (_hitParticleCb) _hitParticleCb(nullptr, cp);
        if (_targetRemovedCb) _targetRemovedCb();
        if (_scheduledRemovalCb) _scheduledRemovalCb(other);
    } else if (other->getTag() == TAG_BALL) {
        if (_comboResetCb) _comboResetCb();
        if (_scheduledRemovalCb) _scheduledRemovalCb(other);
    }
    return false;
}

bool CollisionSystem::handleBallTargetContact(Node * /*ball*/, Node * /*target*/, PhysicsContact &contact)
{
    auto cp = Vec2(contact.getContactData()->points[0].x, contact.getContactData()->points[0].y);
    if (_scoreCb) _scoreCb(nullptr, cp, SCORE_PER_HIT);
    if (_hitParticleCb) _hitParticleCb(nullptr, cp);
    return true;
}

bool CollisionSystem::handleBallBallContact(Node * /*a*/, Node * /*b*/, PhysicsContact &contact)
{
    auto cp = Vec2(contact.getContactData()->points[0].x, contact.getContactData()->points[0].y);
    if (_scoreCb) _scoreCb(nullptr, cp, SCORE_PER_HIT / 2);
    return true;
}
