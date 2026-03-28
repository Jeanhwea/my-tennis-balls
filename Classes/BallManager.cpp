#include "BallManager.h"

#include <cstdlib>

#include "GameConstants.h"

USING_NS_CC;

BallManager::BallManager(Node *parent) : _parent(parent) {}

Color3B BallManager::randomColor()
{
    static const Color3B palette[] = {
        Color3B(255, 80, 80),  Color3B(80, 200, 255),  Color3B(80, 255, 120),  Color3B(255, 220, 50),
        Color3B(255, 140, 50), Color3B(200, 100, 255), Color3B(255, 100, 200), Color3B(100, 255, 220),
    };
    return palette[std::rand() % 8];
}

int BallManager::countBalls() const
{
    int count = 0;
    for (auto child : _parent->getChildren()) {
        if (child->getTag() == TAG_BALL) {
            ++count;
        }
    }
    return count;
}

void BallManager::addBall(const Vec2 &position, const Vec2 &velocity)
{
    if (countBalls() >= MAX_BALLS) return;

    auto ball = Sprite::create("ball.png");
    ball->setScale(BALL_SCALE);
    ball->setPosition(position);
    ball->setTag(TAG_BALL);
    ball->setColor(randomColor());
    ball->setName(StringUtils::format("ball%02d", ++_ballCounter));

    const float radius = ball->getContentSize().width / 2 - BALL_SPRITE_PADDING;
    auto body =
        PhysicsBody::createCircle(radius, PhysicsMaterial(BALL_DENSITY, BALL_RESTITUTION, BALL_FRICTION));
    body->setVelocity(velocity);
    body->setCategoryBitmask(CATEGORY_BALL);
    body->setCollisionBitmask(CATEGORY_ALL);
    body->setContactTestBitmask(CATEGORY_ALL);
    ball->setPhysicsBody(body);
    _parent->addChild(ball, 5);

    // Spawn animation
    ball->setScale(0);
    ball->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE)));

    notifyCountChange();
}

void BallManager::removeBall(Node *ball)
{
    if (!ball) return;
    ball->getPhysicsBody()->setEnabled(false);
    ball->runAction(Sequence::create(
        Spawn::create(ScaleTo::create(0.2f, 0), FadeOut::create(0.2f), nullptr), RemoveSelf::create(),
        CallFunc::create([this]() { notifyCountChange(); }), nullptr));
}

void BallManager::notifyCountChange()
{
    if (_onCountChange) {
        _onCountChange(countBalls());
    }
}
