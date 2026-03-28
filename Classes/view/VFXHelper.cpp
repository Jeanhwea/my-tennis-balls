#include "VFXHelper.h"

#include "GameConstants.h"
#include "common/Random.h"

USING_NS_CC;
using common::randomFloat;

namespace
{

constexpr int PARTICLE_COUNT = 8;
constexpr float PARTICLE_SIZE = 4.0f;
constexpr float PARTICLE_SPEED = 120.0f;
constexpr float PARTICLE_LIFE = 0.4f;

constexpr float FLOAT_SCORE_FONT = 28.0f;
constexpr float FLOAT_SCORE_RISE = 60.0f;
constexpr float FLOAT_SCORE_DUR = 0.8f;

}  // namespace

void VFXHelper::spawnHitParticle(Node *parent, const Vec2 &position)
{
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        auto dot = DrawNode::create();
        Color4F c(randomFloat(0.7f, 1.0f), randomFloat(0.7f, 1.0f), randomFloat(0.2f, 0.6f), 1.0f);
        dot->drawSolidCircle(Vec2::ZERO, PARTICLE_SIZE, 0, 8, c);
        dot->setPosition(position);
        parent->addChild(dot, 15);

        float angle = randomFloat(0, static_cast<float>(M_PI) * 2);
        float speed = randomFloat(PARTICLE_SPEED * 0.5f, PARTICLE_SPEED);
        Vec2 dir(cosf(angle) * speed, sinf(angle) * speed);

        dot->runAction(Sequence::create(
            Spawn::create(MoveBy::create(PARTICLE_LIFE, dir * PARTICLE_LIFE),
                          FadeOut::create(PARTICLE_LIFE), ScaleTo::create(PARTICLE_LIFE, 0.1f), nullptr),
            RemoveSelf::create(), nullptr));
    }
}

void VFXHelper::showFloatingScore(Node *parent, const Vec2 &position, int points)
{
    auto label = Label::createWithTTF(StringUtils::format("+%d", points), FONT_TITLE, FLOAT_SCORE_FONT);
    label->setPosition(position);
    label->setTextColor(Color4B(255, 240, 100, 255));
    label->enableShadow(Color4B(200, 150, 0, 80), Size(1, -1));
    parent->addChild(label, 20);

    label->runAction(Sequence::create(
        Spawn::create(MoveBy::create(FLOAT_SCORE_DUR, Vec2(0, FLOAT_SCORE_RISE)),
                      FadeOut::create(FLOAT_SCORE_DUR), ScaleTo::create(FLOAT_SCORE_DUR, 0.6f), nullptr),
        RemoveSelf::create(), nullptr));
}

void VFXHelper::flashNode(Node *node)
{
    if (!node) return;
    node->runAction(Sequence::create(TintTo::create(0.05f, 255, 255, 255),
                                     TintTo::create(0.15f, 200, 200, 200), nullptr));
}
