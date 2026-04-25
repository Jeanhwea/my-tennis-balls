#include "AmbientParticles.h"

#include "common/Random.h"

USING_NS_CC;
using common::randomFloat;

namespace
{

static constexpr int MAX_DOTS = 25;
static constexpr float SPAWN_INTERVAL = 0.6f;
static constexpr float DOT_MIN_LIFE = 4.0f;
static constexpr float DOT_MAX_LIFE = 8.0f;
static constexpr float DOT_MIN_SIZE = 1.0f;
static constexpr float DOT_MAX_SIZE = 3.0f;
static constexpr float DRIFT_SPEED = 15.0f;

}  // namespace

AmbientParticles *AmbientParticles::create(const Size &area)
{
    auto ret = new (std::nothrow) AmbientParticles();
    if (ret && ret->initWithArea(area)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AmbientParticles::initWithArea(const Size &area)
{
    if (!Node::init()) return false;
    _area = area;
    return true;
}

void AmbientParticles::start()
{
    if (_running) return;
    _running = true;
    schedule([this](float) { spawnDot(); }, SPAWN_INTERVAL, "ambient_spawn");
}

void AmbientParticles::stop()
{
    _running = false;
    unschedule("ambient_spawn");
}

void AmbientParticles::spawnDot()
{
    if (getChildrenCount() >= MAX_DOTS) return;

    auto dot = DrawNode::create();
    float size = randomFloat(DOT_MIN_SIZE, DOT_MAX_SIZE);
    float brightness = randomFloat(0.3f, 0.7f);
    float alpha = randomFloat(0.08f, 0.25f);
    Color4F color(brightness * 0.6f, brightness * 0.8f, brightness, alpha);
    dot->drawSolidCircle(Vec2::ZERO, size, 0, 6, color);

    float x = randomFloat(0, _area.width);
    float y = randomFloat(0, _area.height);
    dot->setPosition(Vec2(x, y));
    addChild(dot);

    float life = randomFloat(DOT_MIN_LIFE, DOT_MAX_LIFE);
    float dx = randomFloat(-DRIFT_SPEED, DRIFT_SPEED) * life;
    float dy = randomFloat(DRIFT_SPEED * 0.5f, DRIFT_SPEED * 1.5f) * life;

    dot->setOpacity(0);
    dot->runAction(Sequence::create(FadeIn::create(life * 0.3f),
                                    Spawn::create(MoveBy::create(life, Vec2(dx, dy)),
                                                  Sequence::create(DelayTime::create(life * 0.6f),
                                                                   FadeOut::create(life * 0.4f), nullptr),
                                                  nullptr),
                                    RemoveSelf::create(), nullptr));
}
