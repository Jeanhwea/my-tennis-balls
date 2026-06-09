#include "VFXHelper.h"

#include "common/GameConstants.h"
#include "common/Random.h"

USING_NS_CC;
using common::randomFloat;

namespace
{

static constexpr int PARTICLE_COUNT = 12;
static constexpr float PARTICLE_SIZE = 4.5f;
static constexpr float PARTICLE_SPEED = 150.0f;
static constexpr float PARTICLE_LIFE = 0.45f;

static constexpr float FLOAT_SCORE_FONT = 28.0f;
static constexpr float FLOAT_SCORE_RISE = 60.0f;
static constexpr float FLOAT_SCORE_DUR = 0.8f;

// Particle pool configuration
static constexpr int DEFAULT_POOL_SIZE = 50;
static constexpr int LABEL_POOL_SIZE = 20;

// Maximum active particles cap (Requirement 2.3)
// spawnHitParticle uses 15 nodes: 12 particles + 2 rings + 1 flash
static constexpr int PARTICLES_PER_HIT = 15;
static constexpr int MAX_ACTIVE_PARTICLES = 50;

// Pool state
struct ParticlePool {
    cocos2d::Vector<cocos2d::DrawNode *> available;
    cocos2d::Vector<cocos2d::DrawNode *> inUse;
    int maxSize = 0;
    bool initialized = false;
};

struct LabelPool {
    cocos2d::Vector<cocos2d::Label *> available;
    cocos2d::Vector<cocos2d::Label *> inUse;
    int maxSize = 0;
    bool initialized = false;
};

ParticlePool g_particlePool;
LabelPool g_labelPool;

}  // namespace

void VFXHelper::initParticlePool(Node *parent, int poolSize)
{
    if (g_particlePool.initialized) {
        return;  // Already initialized
    }

    g_particlePool.maxSize = poolSize;
    g_particlePool.available.clear();
    g_particlePool.inUse.clear();

    // Pre-allocate DrawNode objects
    for (int i = 0; i < poolSize; ++i) {
        auto node = DrawNode::create();
        node->setVisible(false);
        node->setCascadeOpacityEnabled(true);
        parent->addChild(node, 15);
        g_particlePool.available.pushBack(node);
    }

    g_particlePool.initialized = true;

    // Initialize label pool
    if (!g_labelPool.initialized) {
        g_labelPool.maxSize = LABEL_POOL_SIZE;
        g_labelPool.available.clear();
        g_labelPool.inUse.clear();

        for (int i = 0; i < LABEL_POOL_SIZE; ++i) {
            auto label = Label::createWithTTF("", FONT_TITLE, FLOAT_SCORE_FONT);
            label->setVisible(false);
            label->setCascadeOpacityEnabled(true);
            parent->addChild(label, 20);
            g_labelPool.available.pushBack(label);
        }

        g_labelPool.initialized = true;
    }
}

void VFXHelper::resetParticlePool()
{
    if (!g_particlePool.initialized) return;
    for (auto node : g_particlePool.inUse) {
        node->removeFromParent();
    }
    for (auto node : g_particlePool.available) {
        node->removeFromParent();
    }
    g_particlePool.available.clear();
    g_particlePool.inUse.clear();
    g_particlePool.initialized = false;
}

void VFXHelper::resetLabelPool()
{
    if (!g_labelPool.initialized) return;
    for (auto label : g_labelPool.inUse) {
        label->removeFromParent();
    }
    for (auto label : g_labelPool.available) {
        label->removeFromParent();
    }
    g_labelPool.available.clear();
    g_labelPool.inUse.clear();
    g_labelPool.initialized = false;
}

namespace
{

/// Get a DrawNode from the pool, or create a new one if pool is exhausted.
DrawNode *acquireParticleNode(Node *parent)
{
    if (!g_particlePool.initialized) {
        // Fallback: create new node if pool not initialized
        auto node = DrawNode::create();
        parent->addChild(node, 15);
        return node;
    }

    if (!g_particlePool.available.empty()) {
        auto node = g_particlePool.available.back();
        g_particlePool.available.popBack();
        g_particlePool.inUse.pushBack(node);
        node->setVisible(true);
        node->setOpacity(255);
        node->setScale(1.0f);
        node->clear();
        return node;
    }

    // Pool exhausted, create temporary node (will be destroyed after use)
    auto node = DrawNode::create();
    parent->addChild(node, 15);
    return node;
}

/// Return a DrawNode to the pool after its animation completes.
void releaseParticleNode(DrawNode *node)
{
    if (!g_particlePool.initialized) {
        // Not pooled, remove from parent
        node->removeFromParent();
        return;
    }

    // Check if this node is from the pool
    if (g_particlePool.inUse.contains(node)) {
        g_particlePool.inUse.eraseObject(node);
        if (static_cast<int>(g_particlePool.available.size()) < g_particlePool.maxSize) {
            node->setVisible(false);
            node->stopAllActions();
            node->clear();
            g_particlePool.available.pushBack(node);
        } else {
            // Pool is full, remove the node
            node->removeFromParent();
        }
    } else {
        // Not from pool, remove normally
        node->removeFromParent();
    }
}

/// Get a Label from the pool, or create a new one if pool is exhausted.
Label *acquireLabelNode(Node *parent)
{
    if (!g_labelPool.initialized) {
        // Fallback: create new label if pool not initialized
        auto label = Label::createWithTTF("", FONT_TITLE, FLOAT_SCORE_FONT);
        parent->addChild(label, 20);
        return label;
    }

    if (!g_labelPool.available.empty()) {
        auto label = g_labelPool.available.back();
        g_labelPool.available.popBack();
        g_labelPool.inUse.pushBack(label);
        label->setVisible(true);
        label->setOpacity(255);
        label->setScale(1.0f);
        return label;
    }

    // Pool exhausted, create temporary label (will be destroyed after use)
    auto label = Label::createWithTTF("", FONT_TITLE, FLOAT_SCORE_FONT);
    parent->addChild(label, 20);
    return label;
}

/// Return a Label to the pool after its animation completes.
void releaseLabelNode(Label *label)
{
    if (!g_labelPool.initialized) {
        // Not pooled, remove from parent
        label->removeFromParent();
        return;
    }

    // Check if this label is from the pool
    if (g_labelPool.inUse.contains(label)) {
        g_labelPool.inUse.eraseObject(label);
        if (static_cast<int>(g_labelPool.available.size()) < g_labelPool.maxSize) {
            label->setVisible(false);
            label->stopAllActions();
            label->setString("");
            g_labelPool.available.pushBack(label);
        } else {
            // Pool is full, remove the label
            label->removeFromParent();
        }
    } else {
        // Not from pool, remove normally
        label->removeFromParent();
    }
}

}  // namespace

void VFXHelper::spawnHitParticle(Node *parent, const Vec2 &position)
{
    // Check particle count cap (Requirement 2.3)
    // Reject spawn if adding 15 particles would exceed maximum
    if (g_particlePool.initialized &&
        static_cast<int>(g_particlePool.inUse.size()) + PARTICLES_PER_HIT > MAX_ACTIVE_PARTICLES) {
        return;  // At capacity, reject spawn request
    }

    // Particles
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        auto dot = acquireParticleNode(parent);
        float size = randomFloat(PARTICLE_SIZE * 0.5f, PARTICLE_SIZE * 1.2f);
        Color4F c(randomFloat(0.8f, 1.0f), randomFloat(0.7f, 1.0f), randomFloat(0.2f, 0.5f), 1.0f);
        dot->drawSolidCircle(Vec2::ZERO, size, 0, 8, c);
        dot->drawSolidCircle(Vec2::ZERO, size * 0.4f, 0, 6, Color4F(1, 1, 1, 0.6f));
        dot->setPosition(position);

        float angle = randomFloat(0, static_cast<float>(M_PI) * 2);
        float speed = randomFloat(PARTICLE_SPEED * 0.5f, PARTICLE_SPEED);
        Vec2 dir(cosf(angle) * speed, sinf(angle) * speed);

        dot->runAction(Sequence::create(
            Spawn::create(MoveBy::create(PARTICLE_LIFE, dir * PARTICLE_LIFE),
                          FadeOut::create(PARTICLE_LIFE), ScaleTo::create(PARTICLE_LIFE, 0.1f), nullptr),
            CallFunc::create([dot]() { releaseParticleNode(dot); }), nullptr));
    }

    // Shock rings
    auto ring1 = acquireParticleNode(parent);
    ring1->drawCircle(Vec2::ZERO, 8.0f, 0, 24, false, Color4F(1.0f, 0.9f, 0.4f, 0.7f));
    ring1->setPosition(position);
    ring1->runAction(
        Sequence::create(Spawn::create(ScaleTo::create(0.25f, 3.0f), FadeOut::create(0.25f), nullptr),
                         CallFunc::create([ring1]() { releaseParticleNode(ring1); }), nullptr));

    auto ring2 = acquireParticleNode(parent);
    ring2->drawCircle(Vec2::ZERO, 5.0f, 0, 16, false, Color4F(1.0f, 1.0f, 0.8f, 0.4f));
    ring2->setPosition(position);
    ring2->runAction(
        Sequence::create(DelayTime::create(0.05f),
                         Spawn::create(ScaleTo::create(0.2f, 2.5f), FadeOut::create(0.2f), nullptr),
                         CallFunc::create([ring2]() { releaseParticleNode(ring2); }), nullptr));

    // Flash
    auto flash = acquireParticleNode(parent);
    flash->drawSolidCircle(Vec2::ZERO, 12.0f, 0, 12, Color4F(1.0f, 0.95f, 0.7f, 0.5f));
    flash->setPosition(position);
    flash->runAction(
        Sequence::create(Spawn::create(ScaleTo::create(0.12f, 0.1f), FadeOut::create(0.12f), nullptr),
                         CallFunc::create([flash]() { releaseParticleNode(flash); }), nullptr));

    // Micro-shake
    if (parent->getNumberOfRunningActions() < 3) {
        parent->runAction(
            Sequence::create(MoveBy::create(0.02f, Vec2(randomFloat(-2, 2), randomFloat(-2, 2))),
                             MoveBy::create(0.02f, Vec2(randomFloat(-1, 1), randomFloat(-1, 1))),
                             MoveTo::create(0.03f, Vec2::ZERO), nullptr));
    }
}

void VFXHelper::showFloatingScore(Node *parent, const Vec2 &position, int points)
{
    auto label = acquireLabelNode(parent);
    label->setString(StringUtils::format("+%d", points));
    label->setPosition(position);
    label->setTextColor(Color4B(255, 240, 100, 255));
    label->enableShadow(Color4B(200, 150, 0, 80), Size(1, -1));

    label->runAction(Sequence::create(
        Spawn::create(MoveBy::create(FLOAT_SCORE_DUR, Vec2(0, FLOAT_SCORE_RISE)),
                      FadeOut::create(FLOAT_SCORE_DUR), ScaleTo::create(FLOAT_SCORE_DUR, 0.6f), nullptr),
        CallFunc::create([label]() { releaseLabelNode(label); }), nullptr));
}

void VFXHelper::flashNode(Node *node)
{
    if (!node) return;
    node->runAction(Sequence::create(TintTo::create(0.05f, 255, 255, 255),
                                     TintTo::create(0.15f, 200, 200, 200), nullptr));
}
