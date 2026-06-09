# my-tennis-balls — Architecture Overview

## Technology Stack

- **Engine**: Cocos2d-x v3 (C++, 32 files, ~3300 LOC)
- **Build**: CMake (macOS/Linux/Windows) + Android.mk (Android)
- **Audio**: Cocos2d-x AudioEngine / SimpleAudioEngine
- **Physics**: Cocos2d-x built-in physics engine (PhysicsBody, PhysicsContact)
- **JSON**: RapidJSON (bundled with Cocos2d-x)

## Directory Structure

```
Classes/
  AppDelegate.{h,cpp}     — Application lifecycle, viewport setup, scene launch
  Version.h                — Version string, generated from Version.h.in

  builder/
    GameSceneBuilder.{h,cpp} — Scene construction utilities (arena, HUD, input,
                                physics). Extracted from GameController.

  common/
    GameConstants.h           — All game-wide constants (sizes, physics, colors, tags)
    Random.h                  — Inline RNG helpers (randomFloat, randomInt)

  controller/
    GameController.{h,cpp}    — Top-level mediator: wires model, view, input, physics
    InputController.{h,cpp}   — Touch → drag/launch commands, callback-based
    LevelManager.{h,cpp}      — Level lifecycle: load, clear, transition, fail-check
                                NOTE: _ballCounter duplicates GameModel::_ballCount

  model/
    GameModel.{h,cpp}         — Pure data model: score, balls, targets, level index
    ScoreManager.{h,cpp}      — Score + combo state machine (timer, multiplier)
    BallManager.{h,cpp}       — Ball lifecycle: spawn, despawn, OOB collection
                                NOTE: Lives in model/ but calls BallView (view/) directly
    CollisionSystem.{h,cpp}   — Physics contact dispatch with callback pattern
    LevelData.{h,cpp}         — Level JSON loader, TrayData struct

  view/
    ArenaView.{h,cpp}         — Edge walls, floor sensor, zone drawing (namespace functions)
    BallView.{h,cpp}          — Ball sprite + effects: spawn, despawn, motion blur, highlights
    TrayView.{h,cpp}          — Tray rendering + target spawning (namespace functions)
    ObstacleView.{h,cpp}      — Obstacle rendering (namespace functions)
    VisualUtil.{h,cpp}        — Shared drawing: gradient, grid, corners, ambient particles
    AmbientParticles.{h,cpp}  — Floating dot particle system, Node subclass
    VFXHelper.{h,cpp}         — Hit particles, floating score labels (pooled)
    HUD.{h,cpp}               — Top-bar HUD: score, combo, balls, targets, hints
    AimLineView.{h,cpp}       — Drag trajectory preview line, Node subclass
    LevelButtonGrid.{h,cpp}   — Level selection grid with page navigation. UNUSED (LevelMenuScene has its own)

  scene/
    GameScene.{h,cpp}         — Cocos2d Scene with physics, delegates to GameController
    LevelMenuScene.{h,cpp}    — Level selection menu: background, buttons, pages, navigation
```

## Data Flow

```
User Touch
    ↓
InputController ──callbacks──→ GameController
                                   ├── GameModel (state reads/writes)
                                   ├── ScoreManager (combo/score)
                                   ├── BallManager (ball lifecycle)
                                   ├── CollisionSystem (contact dispatch)
                                   ├── LevelManager (level load/transitions)
                                   └── HUD (UI updates)
```

## Current Design Issues

### 1. LevelMenuScene over-engineered (330 lines)
- Full background/title drawing, grid-based buttons, pagination, prev/next nav, quit button
- `LevelButtonGrid` exists separately but is not used — duplication
- `drawBackground()` inlines VisualUtil calls directly (could delegate)

### 2. HUD has dead code removed (previous commit)
- `showCleared()`, `showFailed()`, `showLevelIntro()`, `setOverlayParent()` were dead code — removed
- Still has many `create*` methods that follow same pattern (could be DRY'd)

### 3. LevelManager still mixes concerns
- `_ballCounter` duplicates `GameModel::_ballCount` (less severe after previous cleanup)
- `clearLevelNodes()` has a bug — doesn't clear obstacles (TAG_OBSTACLE not checked)

### 4. BallManager in model/ but depends on view/BallView
- Model layer imports view layer, violating layer isolation
- Should be moved to view/, or ball state extracted into model/BallState.h

### 5. BallView::spawn is large (103 lines)
- Creates 6 sub-nodes per ball (shadow, glow, motion blur, highlight, sub-highlight, ring)
- Each follows the same pattern: create → setColor/Opacity/Scale → setPosition/Name → addChild
- Could be DRY'd with helper, or extract sub-node creation into methods

## Recommended Next Steps

1. **Delete unused LevelButtonGrid** — not referenced anywhere
2. **Fix LevelManager::clearLevelNodes** — add TAG_OBSTACLE to the removal check
3. **Move BallManager to view/** — or extract model state into model/BallState.h
4. **Consolidate HUD create methods** — DRY up the repeated `create*` label pattern
5. **Reduce BallView::spawn duplication** — extract a helper for repetitive sub-node creation
6. **Unify namespace color/comment format** — some namespaces have comments on `}`, some don't (minor)
