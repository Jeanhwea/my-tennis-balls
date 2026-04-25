#include "BallView.h"

#include "common/GameConstants.h"

USING_NS_CC;

namespace
{
// 光照效果参数
constexpr float HIGHLIGHT_OFFSET_RATIO = 0.25f;  // 高光偏移比例
constexpr float HIGHLIGHT_SCALE = 0.35f;         // 高光缩放
constexpr float SHADOW_OFFSET_Y = -8.0f;         // 阴影Y偏移
constexpr float SHADOW_SCALE = 1.1f;             // 阴影缩放
constexpr float SHADOW_OPACITY = 0.25f;          // 阴影透明度
constexpr float GLOW_OPACITY = 0.15f;            // 光晕透明度

// 滚动光照参数
constexpr float ROLLING_HIGHLIGHT_MAX_OFFSET = 0.35f;  // 滚动时高光最大偏移比例
constexpr float ROLLING_SPEED_THRESHOLD = 100.0f;      // 触发滚动效果的最低速度
constexpr float ROLLING_SPEED_MAX = 800.0f;            // 最大速度（用于归一化）
constexpr float MOTION_BLUR_OPACITY = 0.3f;            // 运动模糊透明度
}  // namespace

Sprite *BallView::spawn(Node *parent, const Vec2 &position, const Vec2 &velocity, int ballIndex)
{
    // 创建阴影层（最底层）
    auto shadow = Sprite::create("ball.png");
    shadow->setColor(Color3B::BLACK);
    shadow->setOpacity(static_cast<uint8_t>(255 * SHADOW_OPACITY));
    shadow->setScale(BALL_SCALE * SHADOW_SCALE);
    shadow->setPosition(position.x, position.y + SHADOW_OFFSET_Y);
    shadow->setName(StringUtils::format("ball%02d_shadow", ballIndex));
    parent->addChild(shadow, 3);

    // 创建光晕层（底层）
    auto glow = Sprite::create("ball.png");
    glow->setColor(Color3B(150, 200, 255));
    glow->setOpacity(static_cast<uint8_t>(255 * GLOW_OPACITY));
    glow->setScale(BALL_SCALE * 1.3f);
    glow->setPosition(position);
    glow->setName(StringUtils::format("ball%02d_glow", ballIndex));
    parent->addChild(glow, 4);

    // 创建运动模糊层（用于高速运动）
    auto motionBlur = Sprite::create("ball.png");
    motionBlur->setColor(Color3B(200, 220, 255));
    motionBlur->setOpacity(0);
    motionBlur->setScale(BALL_SCALE);
    motionBlur->setName(StringUtils::format("ball%02d_blur", ballIndex));
    parent->addChild(motionBlur, 4);

    // 创建主球体
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

    // 创建高光层（顶层）- 初始位置在左上方
    auto highlight = Sprite::create("ball.png");
    highlight->setColor(Color3B(255, 255, 255));
    highlight->setOpacity(180);
    highlight->setScale(BALL_SCALE * HIGHLIGHT_SCALE);
    highlight->setBlendFunc(BlendFunc::ADDITIVE);
    highlight->setPosition(Vec2(radius * HIGHLIGHT_OFFSET_RATIO, radius * HIGHLIGHT_OFFSET_RATIO));
    highlight->setName("highlight");
    ball->addChild(highlight, 1);

    // 创建次高光层（模拟环境反射）
    auto subHighlight = Sprite::create("ball.png");
    subHighlight->setColor(Color3B(200, 230, 255));
    subHighlight->setOpacity(60);
    subHighlight->setScale(BALL_SCALE * 0.2f);
    subHighlight->setBlendFunc(BlendFunc::ADDITIVE);
    subHighlight->setPosition(Vec2(-radius * 0.3f, -radius * 0.3f));
    subHighlight->setName("subHighlight");
    ball->addChild(subHighlight, 1);

    // 生成动画
    ball->setScale(0);
    ball->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE)));

    shadow->setScale(0);
    shadow->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE * SHADOW_SCALE)));

    glow->setScale(0);
    glow->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE * 1.3f)));

    // 生成光环效果
    auto ring = DrawNode::create();
    ring->drawCircle(Vec2::ZERO, radius * 2.5f, 0, 24, false, Color4F(0.5f, 0.8f, 1.0f, 0.6f));
    ring->drawCircle(Vec2::ZERO, radius * 1.8f, 0, 16, false, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
    ring->setPosition(position);
    parent->addChild(ring, 4);
    ring->runAction(Sequence::create(ScaleTo::create(0.35f, 2.5f), RemoveSelf::create(), nullptr));

    return ball;
}

void BallView::updateRollingEffect(Node *ball)
{
    if (!ball) return;

    auto body = ball->getPhysicsBody();
    if (!body) return;

    // 获取球的速度
    Vec2 velocity = body->getVelocity();
    float speed = velocity.length();

    // 获取高光层
    auto highlight = ball->getChildByName("highlight");
    auto subHighlight = ball->getChildByName("subHighlight");
    if (!highlight) return;

    const float radius = ball->getContentSize().width * ball->getScale() / 2;

    // 计算高光偏移：根据速度方向反向偏移（模拟光源固定，球滚动时光照点相对移动）
    if (speed > ROLLING_SPEED_THRESHOLD) {
        // 归一化速度
        float normalizedSpeed = std::min(speed / ROLLING_SPEED_MAX, 1.0f);

        // 速度方向的反方向（光从固定方向照射，球滚动时光点相对移动）
        Vec2 velocityDir = velocity.getNormalized();
        Vec2 lightOffset = -velocityDir * radius * ROLLING_HIGHLIGHT_MAX_OFFSET * normalizedSpeed;

        // 平滑移动高光位置
        Vec2 basePos(radius * HIGHLIGHT_OFFSET_RATIO, radius * HIGHLIGHT_OFFSET_RATIO);
        highlight->setPosition(basePos + lightOffset);

        // 根据速度调整高光透明度（高速时更亮）
        uint8_t opacity = static_cast<uint8_t>(180 + 75 * normalizedSpeed);
        highlight->setOpacity(opacity);

        // 次高光也跟随移动（相反方向）
        if (subHighlight) {
            Vec2 subBasePos(-radius * 0.3f, -radius * 0.3f);
            subHighlight->setPosition(subBasePos - lightOffset * 0.3f);
        }
    } else {
        // 静止时恢复默认位置
        highlight->setPosition(Vec2(radius * HIGHLIGHT_OFFSET_RATIO, radius * HIGHLIGHT_OFFSET_RATIO));
        highlight->setOpacity(180);
        if (subHighlight) {
            subHighlight->setPosition(Vec2(-radius * 0.3f, -radius * 0.3f));
        }
    }
}

void BallView::updateMotionBlur(Node *ball, Node *blurNode)
{
    if (!ball || !blurNode) return;

    auto body = ball->getPhysicsBody();
    if (!body) return;

    Vec2 velocity = body->getVelocity();
    float speed = velocity.length();

    // 高速时显示运动模糊
    if (speed > ROLLING_SPEED_THRESHOLD * 2) {
        float normalizedSpeed = std::min(speed / ROLLING_SPEED_MAX, 1.0f);
        uint8_t opacity = static_cast<uint8_t>(255 * MOTION_BLUR_OPACITY * normalizedSpeed);
        blurNode->setOpacity(opacity);
        blurNode->setPosition(ball->getPosition() - velocity.getNormalized() * 5);
        blurNode->setScale(BALL_SCALE);
    } else {
        blurNode->setOpacity(0);
    }
}

void BallView::despawn(Node *ball, const std::function<void()> &onComplete)
{
    if (!ball) return;
    ball->getPhysicsBody()->setEnabled(false);

    // 同时移除关联的阴影和光晕
    auto name = ball->getName();
    if (!name.empty()) {
        auto parent = ball->getParent();
        if (parent) {
            auto shadow = parent->getChildByName(name + "_shadow");
            auto glow = parent->getChildByName(name + "_glow");
            if (shadow) {
                shadow->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
            }
            if (glow) {
                glow->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
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
