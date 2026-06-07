#include "BallView.h"

#include "common/GameConstants.h"

USING_NS_CC;

namespace
{
// 光照效果参数
constexpr float HIGHLIGHT_SCALE = 0.35f;         // 高光缩放
constexpr float SHADOW_OFFSET_Y = -8.0f;         // 阴影Y偏移
constexpr float SHADOW_SCALE = 1.1f;             // 阴影缩放
constexpr float SHADOW_OPACITY = 0.25f;          // 阴影透明度
constexpr float GLOW_OPACITY = 0.15f;            // 光晕透明度
constexpr float MOTION_BLUR_OPACITY = 0.3f;      // 运动模糊透明度
constexpr float MOTION_BLUR_THRESHOLD = 200.0f;  // 运动模糊触发速度

// 光源方向（从左上方照射，归一化向量）
constexpr float LIGHT_DIR_X = -0.5f;
constexpr float LIGHT_DIR_Y = 0.5f;
}  // namespace

// ── 创建弹球（含阴影、光晕、高光、运动模糊等层） ──

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

    // ── 光照系统 ──
    // 高光和次高光是独立节点，不是球的子节点，从而模拟固定光源空间效果
    // 随着球旋转，高光会在球表面滑动（updateHighlights 中计算）

    // 主高光：球左上方白色高光（模拟主光源照射）
    auto highlight = Sprite::create("ball.png");
    highlight->setColor(Color3B(255, 255, 255));
    highlight->setOpacity(180);
    highlight->setScale(HIGHLIGHT_SCALE);
    highlight->setBlendFunc(BlendFunc::ADDITIVE);
    highlight->setPosition(position.x + radius * LIGHT_DIR_X, position.y + radius * LIGHT_DIR_Y);
    highlight->setName(StringUtils::format("ball%02d_highlight", ballIndex));
    parent->addChild(highlight, 6);

    // 次高光：球右下方淡蓝色（模拟环境反射补光）
    auto subHighlight = Sprite::create("ball.png");
    subHighlight->setColor(Color3B(200, 230, 255));
    subHighlight->setOpacity(60);
    subHighlight->setScale(0.2f);
    subHighlight->setBlendFunc(BlendFunc::ADDITIVE);
    subHighlight->setPosition(position.x - radius * LIGHT_DIR_X, position.y - radius * LIGHT_DIR_Y);
    subHighlight->setName(StringUtils::format("ball%02d_subhl", ballIndex));
    parent->addChild(subHighlight, 6);

    // 生成动画
    ball->setScale(0);
    ball->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE)));

    shadow->setScale(0);
    shadow->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE * SHADOW_SCALE)));

    glow->setScale(0);
    glow->runAction(EaseBackOut::create(ScaleTo::create(0.25f, BALL_SCALE * 1.3f)));

    highlight->setScale(0);
    highlight->runAction(EaseBackOut::create(ScaleTo::create(0.25f, HIGHLIGHT_SCALE)));

    subHighlight->setScale(0);
    subHighlight->runAction(EaseBackOut::create(ScaleTo::create(0.25f, 0.2f)));

    // 生成光环效果
    auto ring = DrawNode::create();
    ring->drawCircle(Vec2::ZERO, radius * 2.5f, 0, 24, false, Color4F(0.5f, 0.8f, 1.0f, 0.6f));
    ring->drawCircle(Vec2::ZERO, radius * 1.8f, 0, 16, false, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
    ring->setPosition(position);
    parent->addChild(ring, 4);
    ring->runAction(Sequence::create(ScaleTo::create(0.35f, 2.5f), RemoveSelf::create(), nullptr));

    return ball;
}

void BallView::updateHighlights(Node *ball)
{
    /// 随球旋转同步更新高光位置，模拟固定光源下球表面反射点的滑动。
    /// 核心思路：高光是独立节点，通过旋转光源方向向量来计算球表面上的反射位置。
    if (!ball) return;

    auto parent = ball->getParent();
    if (!parent) return;

    std::string name = ball->getName();
    if (name.empty()) return;

    auto highlight = parent->getChildByName(name + "_highlight");
    auto subHighlight = parent->getChildByName(name + "_subhl");
    if (!highlight || !subHighlight) return;

    // 读取物理体的旋转角（碰撞反弹产生的自然旋转）
    auto body = ball->getPhysicsBody();
    float rotation = body ? -body->getRotation() : 0.0f;

    float radius = ball->getContentSize().width * BALL_SCALE / 2 - BALL_SPRITE_PADDING;
    float rad = CC_DEGREES_TO_RADIANS(rotation);

    Vec2 lightDir(LIGHT_DIR_X, LIGHT_DIR_Y);

    // 固定光源向量绕球心旋转，得到球表面反射点的新位置
    float cosR = cosf(rad);
    float sinR = sinf(rad);
    Vec2 rotatedLightDir(lightDir.x * cosR - lightDir.y * sinR, lightDir.x * sinR + lightDir.y * cosR);

    Vec2 ballPos = ball->getPosition();
    highlight->setPosition(ballPos.x + rotatedLightDir.x * radius, ballPos.y + rotatedLightDir.y * radius);
    subHighlight->setPosition(ballPos.x - rotatedLightDir.x * radius,
                              ballPos.y - rotatedLightDir.y * radius);
}

void BallView::updateMotionBlur(Node *ball, Node *blurNode)
{
    if (!ball || !blurNode) return;

    auto body = ball->getPhysicsBody();
    if (!body) return;

    Vec2 velocity = body->getVelocity();
    float speed = velocity.length();

    // 高速时显示运动模糊
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

// ── 移除弹球（含所有关联渲染层） ──

void BallView::despawn(Node *ball, const std::function<void()> &onComplete)
{
    if (!ball) return;
    ball->getPhysicsBody()->setEnabled(false);

    // 同时移除关联的阴影、光晕和高光
    auto name = ball->getName();
    if (!name.empty()) {
        auto parent = ball->getParent();
        if (parent) {
            auto shadow = parent->getChildByName(name + "_shadow");
            auto glow = parent->getChildByName(name + "_glow");
            auto blur = parent->getChildByName(name + "_blur");
            auto highlight = parent->getChildByName(name + "_highlight");
            auto subHighlight = parent->getChildByName(name + "_subhl");
            if (shadow) {
                shadow->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
            }
            if (glow) {
                glow->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
            }
            if (blur) {
                blur->runAction(Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
            }
            if (highlight) {
                highlight->runAction(
                    Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
            }
            if (subHighlight) {
                subHighlight->runAction(
                    Sequence::create(FadeOut::create(0.15f), RemoveSelf::create(), nullptr));
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
