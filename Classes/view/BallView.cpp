#include "BallView.h"

#include "common/GameConstants.h"

USING_NS_CC;

namespace
{
constexpr float SHADOW_OFFSET_Y = -8.0f;
constexpr float SHADOW_SCALE = 1.1f;
constexpr float SHADOW_OPACITY = 0.25f;
constexpr float MOTION_BLUR_OPACITY = 0.3f;
constexpr float MOTION_BLUR_THRESHOLD = 200.0f;
}

Sprite *BallView::spawn(Node *parent, const Vec2 &position, const Vec2 &velocity, int ballIndex)
{
    auto shadow = Sprite::create("ball.png");
    shadow->setColor(Color3B::BLACK);
    shadow->setOpacity(static_cast<uint8_t>(255 * SHADOW_OPACITY));
    shadow->setScale(BALL_SCALE * SHADOW_SCALE);
    shadow->setPosition(position.x, position.y + SHADOW_OFFSET_Y);
    shadow->setName(StringUtils::format("ball%02d_shadow", ballIndex));
    parent->addChild(shadow, 3);

    auto motionBlur = Sprite::create("ball.png");
    motionBlur->setColor(Color3B(200, 220, 255));
    motionBlur->setOpacity(0);
    motionBlur->setScale(BALL_SCALE);
    motionBlur->setName(StringUtils::format("ball%02d_blur", ballIndex));
    parent->addChild(motionBlur, 4);

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
    body->setCollisionBitmask(CATEGORY_EDGE | CATEGORY_TARGET | CATEGORY_TRAY | CATEGORY_OBSTACLE);
    body->setContactTestBitmask(CATEGORY_TARGET | CATEGORY_FLOOR);
    ball->setPhysicsBody(body);
    parent->addChild(ball, 5);

    ball->setScale(0);
    ball->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE)));

    shadow->setScale(0);
    shadow->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE * SHADOW_SCALE)));

    auto ring = DrawNode::create();
    ring->drawCircle(Vec2::ZERO, radius * 2.5f, 0, 24, false, Color4F(0.5f, 0.8f, 1.0f, 0.6f));
    ring->drawCircle(Vec2::ZERO, radius * 1.8f, 0, 16, false, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
    ring->setPosition(position);
    parent->addChild(ring, 4);
    ring->runAction(Sequence::create(ScaleTo::create(0.35f, 2.5f), RemoveSelf::create(), nullptr));

    return ball;
}

void BallView::updateMotionBlur(Node *ball, Node *blurNode)
{
    if (!ball || !blurNode) return;

    auto body = ball->getPhysicsBody();
    if (!body) return;

    Vec2 velocity = body->getVelocity();
    float speed = velocity.length();

    if (speed > MOTION_BLUR_THRESHOLD) {
        float normalizedSpeed = std::min(speed / 1000.0f, 1.0f);
        uint8_t opacity = static_cast<uint8_t>(255 * MOTION_BLUR_OPACITY * normalizedSpeed);
        blurNode->setOpacity(opacity);
        blurNode->setPosition(ball->getPosition() - velocity.getNormalized() * 5);
        blurNode->setScale(BALL_SCALE);
    } else {
        blurNode->setOpacity(0);
    }
}

void BallView::updateEffects(Node *ball)
{
    auto name = ball->getName();
    if (name.empty()) return;

    auto parent = ball->getParent();
    if (!parent) return;

    auto shadow = parent->getChildByName(name + "_shadow");
    auto blur = parent->getChildByName(name + "_blur");

    Vec2 ballPos = ball->getPosition();
    if (shadow) shadow->setPosition(ballPos.x, ballPos.y + SHADOW_OFFSET_Y);

    updateMotionBlur(ball, blur);
}

void BallView::despawn(Node *ball, const std::function<void()> &onComplete)
{
    if (!ball) return;
    ball->getPhysicsBody()->setEnabled(false);

    auto name = ball->getName();
    if (!name.empty()) {
        auto parent = ball->getParent();
        if (parent) {
            auto shadow = parent->getChildByName(name + "_shadow");
            auto blur = parent->getChildByName(name + "_blur");
            if (shadow) {
                shadow->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
            }
            if (blur) {
                blur->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
            }
        }
    }

    Vector<FiniteTimeAction *> seq;
    seq.pushBack(ScaleTo::create(0.2f, 0));
    seq.pushBack(RemoveSelf::create());
    if (onComplete) {
        seq.pushBack(CallFunc::create(onComplete));
    }
    ball->runAction(Sequence::create(seq));
}
