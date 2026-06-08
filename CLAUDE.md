# CLAUDE.md

This file provides guidance to Claude Code about this project.

## Project Overview

my-tennis-balls — a Cocos2D-x v3 C++ game (~3300 lines, 34 source files). MVC + Mediator pattern.

## Build Commands

```bash
./bld.sh                    # Build (cmake + make, Release)
./pkg.sh                    # Package into .dmg (macOS Release build + code sign + DMG)
./bld-android.sh release    # Build Android APK
./fmt.sh                    # Format all C++ sources with clang-format
```

CMake targets: `my-tennis-balls` (macOS/Linux/Windows executable) or `MyGame` (Android shared lib).

## Architecture

```
AppDelegate → LevelMenuScene → GameScene(GameController)
                                  ├── GameModel + ScoreManager   (pure data, no rendering)
                                  ├── ArenaView / BallView / TrayView / etc.  (namespace functions)
                                  ├── HUD / AimLineView / AmbientParticles   (Node subclasses)
                                  ├── InputController   (touch → callbacks)
                                  ├── LevelData   (JSON level loader, static singleton)
                                  └── VFXHelper   (static particle/label pools)
```

- **Model layer**: `Classes/model/` — GameModel, LevelData, ScoreManager. No cocos2d rendering dependencies.
- **View layer**: `Classes/view/` — namespace+free function factories (BallView, ArenaView, TrayView, ObstacleView, VFXHelper) and Node subclasses (HUD, AimLineView, AmbientParticles).
- **Controller layer**: `Classes/controller/` — GameController (mediator), InputController (touch).
- **Util layer**: `Classes/util/` — VisualUtil (shared drawing helpers for gradients, grids, corners).
- **Scene layer**: `Classes/scene/` — GameScene, LevelMenuScene.
- **Common**: `Classes/common/` — GameConstants.h, Random.h.

## Communication Pattern

- `std::function` callbacks (InputController launch/drag, ScoreManager onChange, HUD back button).
- Deferred removal via `_pendingRemoval` vector to avoid mutating scene graph during collision callbacks.
- Level coordinates use relative ratios (0~1), resolution-independent.

## Key Constants (GameConstants.h)

- Fonts: `FONT_UI = "fonts/arial.ttf"`, `FONT_TITLE = "fonts/Marker Felt.ttf"`
- Tags: TAG_BALL(100), TAG_TARGET(200), TAG_FLOOR(300), TAG_TRAY(400), TAG_OBSTACLE(500)
- Physics categories: CATEGORY_BALL(0x01), EDGE(0x02), TARGET(0x04), FLOOR(0x08), TRAY(0x10), OBSTACLE(0x20)
- Gravity: -980.0
- Launch zone: rightmost 20% of screen width

## Code Style

- Files use `///` Doxygen-style comments on headers
- Source files use `//` section comments
- Namespace-scoped free functions for view factories, classes for stateful objects
- `USING_NS_CC;` in .cpp files
- All C++14/17 features available
- Avoid speculative abstractions; prefer simple functions over premature utilities
