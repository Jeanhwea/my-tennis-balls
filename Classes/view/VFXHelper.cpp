#include "VFXHelper.h"

#include "common/GameConstants.h"
#include "common/Random.h"

USING_NS_CC;
using common::randomFloat;

namespace
{

constexpr int PARTICLE_COUNT = 12;
constexpr float PARTICLE_SIZE = 4.5f;
constexpr float PARTICLE_SPEED = 150.0f;
constexpr float PARTICLE_LIFE = 0.45f;

constexpr float FLOAT_SCORE_FONT = 28.0f;
constexpr float FLOAT_SCORE_RISE = 60.0f;
constexpr float FLOAT_SCORE_DUR = 0.8f;

}  // namespace

void VFXHelper::spawnHitParticle(Node *parent, const Vec2 &position)
{
    // 粒子爆发（带拖尾感的不同大小）
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        auto dot = DrawNode::create();
        float size = randomFloat(PARTICLE_SIZE * 0.5f, PARTICLE_SIZE * 1.2f);
        Color4F c(randomFloat(0.8f, 1.0f), randomFloat(0.7f, 1.0f), randomFloat(0.2f, 0.5f), 1.0f);
        dot->drawSolidCircle(Vec2::ZERO, size, 0, 8, c);
        // 粒子内核高光
        dot->drawSolidCircle(Vec2::ZERO, size * 0.4f, 0, 6, Color4F(1, 1, 1, 0.6f));
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

    // 双层冲击环
    auto ring1 = DrawNode::create();
    ring1->drawCircle(Vec2::ZERO, 8.0f, 0, 24, false, Color4F(1.0f, 0.9f, 0.4f, 0.7f));
    ring1->setPosition(position);
    parent->addChild(ring1, 14);
    ring1->runAction(
        Sequence::create(Spawn::create(ScaleTo::create(0.25f, 3.0f), FadeOut::create(0.25f), nullptr),
                         RemoveSelf::create(), nullptr));

    auto ring2 = DrawNode::create();
    ring2->drawCircle(Vec2::ZERO, 5.0f, 0, 16, false, Color4F(1.0f, 1.0f, 0.8f, 0.4f));
    ring2->setPosition(position);
    parent->addChild(ring2, 14);
    ring2->runAction(
        Sequence::create(DelayTime::create(0.05f),
                         Spawn::create(ScaleTo::create(0.2f, 2.5f), FadeOut::create(0.2f), nullptr),
                         RemoveSelf::create(), nullptr));

    // 中心闪光
    auto flash = DrawNode::create();
    flash->drawSolidCircle(Vec2::ZERO, 12.0f, 0, 12, Color4F(1.0f, 0.95f, 0.7f, 0.5f));
    flash->setPosition(position);
    parent->addChild(flash, 16);
    flash->runAction(
        Sequence::create(Spawn::create(ScaleTo::create(0.12f, 0.1f), FadeOut::create(0.12f), nullptr),
                         RemoveSelf::create(), nullptr));

    // 微震动（screen shake）
    if (parent->getNumberOfRunningActions() < 3) {
        parent->runAction(
            Sequence::create(MoveBy::create(0.02f, Vec2(randomFloat(-2, 2), randomFloat(-2, 2))),
                             MoveBy::create(0.02f, Vec2(randomFloat(-1, 1), randomFloat(-1, 1))),
                             MoveTo::create(0.03f, Vec2::ZERO), nullptr));
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
