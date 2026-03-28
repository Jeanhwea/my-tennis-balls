#include "BallView.h"

#include "common/GameConstants.h"

USING_NS_CC;

Sprite *BallView::spawn(Node *parent, const Vec2 &position, const Vec2 &velocity, int ballIndex)
{
    auto ball = Sprite::create("ball.png");
    ball->setScale(BALL_SCALE);
    ball->setPosition(position);
    ball->setTag(TAG_BALL);
    ball->setName(StringUtils::format("ball%02d", ballIndex));

    const float radius = ball->getContentSize().width / 2 - BALL_SPRITE_PADDING;
    auto body =
        PhysicsBody::createCircle(radius, PhysicsMaterial(BALL_DENSITY, BALL_RESTITUTION, BALL_FRICTION));
    body->setVelocity(velocity);
    body->setCategoryBitmask(CATEGORY_BALL);
    body->setCollisionBitmask(CATEGORY_ALL);
    body->setContactTestBitmask(CATEGORY_ALL);
    ball->setPhysicsBody(body);
    parent->addChild(ball, 5);

    ball->setScale(0);
    ball->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE)));

    auto ring = DrawNode::create();
    ring->drawCircle(Vec2::ZERO, radius * 2.5f, 0, 24, false, Color4F(0.5f, 0.8f, 1.0f, 0.6f));
    ring->drawCircle(Vec2::ZERO, radius * 1.8f, 0, 16, false, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
    ring->setPosition(position);
    parent->addChild(ring, 4);
    ring->runAction(Sequence::create(ScaleTo::create(0.35f, 2.5f), RemoveSelf::create(), nullptr));

    return ball;
}

void BallView::despawn(Node *ball, const std::function<void()> &onComplete)
{
    if (!ball) return;
    ball->getPhysicsBody()->setEnabled(false);

    Vector<FiniteTimeAction *> seq;
    seq.pushBack(ScaleTo::create(0.2f, 0));
    seq.pushBack(RemoveSelf::create());
    if (onComplete) {
        seq.pushBack(CallFunc::create(onComplete));
    }
    ball->runAction(Sequence::create(seq));
}
