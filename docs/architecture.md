# my-tennis-balls — 架构概览

## 技术栈

- **引擎**: Cocos2d-x v3 (C++, 32 个文件, ~3300 行)
- **构建**: CMake (macOS/Linux/Windows) + Android.mk (Android)
- **音频**: Cocos2d-x AudioEngine / SimpleAudioEngine
- **物理**: Cocos2d-x 内置物理引擎 (PhysicsBody, PhysicsContact)
- **JSON**: RapidJSON (随 Cocos2d-x 捆绑)

## 目录结构

```
Classes/
  AppDelegate.{h,cpp}     — 应用生命周期、视口设置、场景启动
  Version.h                — 版本字符串，由 Version.h.in 生成

  builder/
    GameSceneBuilder.{h,cpp} — 场景构建工具（竞技场、HUD、输入、物理）。
                               从 GameController 中抽取。

  common/
    GameConstants.h           — 全局常量（尺寸、物理参数、颜色、标签）
    Random.h                  — 内联 RNG 辅助函数 (randomFloat, randomInt)

  controller/
    GameController.{h,cpp}    — 顶层中介者：连接 model、view、input、physics
    InputController.{h,cpp}   — 触摸 → 拖拽/发射命令，基于回调
    LevelManager.{h,cpp}      — 关卡生命周期：加载、清理、过渡、失败判定
                                注意: _ballCounter 与 GameModel::_ballCount 重复

  model/
    GameModel.{h,cpp}         — 纯数据模型：分数、球数、目标数、关卡索引
    ScoreManager.{h,cpp}      — 分数 + 连击状态机（计时器、倍率）
    BallManager.{h,cpp}       — 球生命周期：生成、销毁、屏幕外回收
                                注意: 位于 model/ 但直接调用 BallView (view/)
    CollisionSystem.{h,cpp}   — 物理碰撞分发，基于回调模式
    LevelData.{h,cpp}         — 关卡 JSON 加载器，TrayData 结构体

  view/
    ArenaView.{h,cpp}         — 边缘墙壁、地板传感器、区域绘制（命名空间函数）
    BallView.{h,cpp}          — 球精灵 + 特效：生成、销毁、运动模糊、高光
    TrayView.{h,cpp}          — 托盘渲染 + 目标生成（命名空间函数）
    ObstacleView.{h,cpp}      — 障碍物渲染（命名空间函数）
    VisualUtil.{h,cpp}        — 共享绘制：渐变色背景、网格、角落装饰、环境粒子
    AmbientParticles.{h,cpp}  — 浮动点粒子系统，Node 子类
    VFXHelper.{h,cpp}         — 击中粒子、浮动分数标签（对象池）
    HUD.{h,cpp}               — 顶部状态栏：分数、连击、球数量、目标数、提示
    AimLineView.{h,cpp}       — 拖拽轨迹预览线，Node 子类
    LevelButtonGrid.{h,cpp}   — 关卡选择网格 + 分页导航。未使用 (LevelMenuScene 有自己的实现)

  scene/
    GameScene.{h,cpp}         — Cocos2d 物理场景，委托给 GameController
    LevelMenuScene.{h,cpp}    — 关卡选择菜单：背景、按钮、页面、导航
```

## 数据流

```
用户触摸
    ↓
InputController ──回调──→ GameController
                            ├── GameModel (状态读写)
                            ├── ScoreManager (连击/分数)
                            ├── BallManager (球生命周期)
                            ├── CollisionSystem (碰撞分发)
                            ├── LevelManager (关卡加载/过渡)
                            └── HUD (UI 更新)
```

## 当前设计问题

### 1. LevelMenuScene 过重 (330 行)
- 包含完整的背景/标题绘制、网格按钮、分页、前进/后退导航、退出按钮
- `LevelButtonGrid` 独立存在但未被使用 — 重复代码
- `drawBackground()` 直接内联调用 VisualUtil 函数（可委托）

### 2. HUD 已移除死代码（上一轮提交）
- `showCleared()`, `showFailed()`, `showLevelIntro()`, `setOverlayParent()` 已被删除
- 仍有许多 `create*` 方法遵循相同模式（可进一步 DRY）

### 3. LevelManager 仍然职责混杂
- `_ballCounter` 与 `GameModel::_ballCount` 重复（上次清理后已减轻）
- `clearLevelNodes()` 存在 bug — 未检查 TAG_OBSTACLE 导致障碍物未被清除

### 4. BallManager 位于 model/ 但依赖 view/BallView
- 模型层直接导入视图层，违反分层隔离
- 应移到 view/，或提取球的状态到 model/BallState.h

### 5. BallView::spawn 过于冗长 (103 行)
- 每个球创建 6 个子节点（阴影、发光、运动模糊、高光、次高光、圆环）
- 每个子节点都遵循相同模式：create → setColor/Opacity/Scale → setPosition/Name → addChild
- 可用辅助函数 DRY，或提取子节点创建为独立方法

## 建议接下来的步骤

1. **删除未使用的 LevelButtonGrid** — 未被任何代码引用
2. **修复 LevelManager::clearLevelNodes** — 在清理检查中添加 TAG_OBSTACLE
3. **将 BallManager 移到 view/** — 或将模型状态提取到 model/BallState.h
4. **合并 HUD 的 create 方法** — 将重复的标签创建模式 DRY 化
5. **减少 BallView::spawn 中的重复代码** — 提取辅助函数处理重复的子节点创建
6. **统一命名空间结束注释格式** — 部分命名空间在 `}` 后有注释，部分没有（微小问题）
