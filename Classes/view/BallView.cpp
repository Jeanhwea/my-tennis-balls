#include "BallView.h"

#include "GameConstants.h"
#include "common/Random.h"

USING_NS_CC;

namespace
{

Color3B randomColor()
{
    static const Color3B palette[] = {
        Color3B(255, 80, 80),  Color3B(80, 200, 255),  Color3B(80, 255, 120),  Color3B(255, 220, 50),
        Color3B(255, 140, 50), Color3B(200, 100, 255), Color3B(255, 100, 200), Color3B(100, 255, 220),
    };
    return palette[std::rand() % 8];
}

}  // namespace

Sprite *BallView::spawn(Node *parent, const Vec2 &position, const Vec2 &velocity, int ballIndex)
{
    auto ball = Sprite::create("ball.png");
    ball->setScale(BALL_SCALE);
    ball->setPosition(position);
    ball->setTag(TAG_BALL);
    ball->setColor(randomColor());
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

    // Spawn animation
    ball->setScale(0);
    ball->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE)));

    return ball;
}

void BallView::despawn(Node *ball, const std::function<void()> &onComplete)
{
    if (!ball) return;
    ball->getPhysicsBody()->setEnabled(false);

    Vector<FiniteTimeAction *> seq;
    seq.pushBack(Spawn::create(ScaleTo::create(0.2f, 0), FadeOut::create(0.2f), nullptr));
    seq.pushBack(RemoveSelf::create());
    if (onComplete) {
        seq.pushBack(CallFunc::create(onComplete));
    }
    ball->runAction(Sequence::create(seq));
}
