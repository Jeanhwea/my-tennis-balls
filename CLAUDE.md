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


## Code Style

- **No comments.** Code should be self-documenting with clear, descriptive names. Do not write Doxygen (`///`), section (`//`), or inline comments. Let identifiers speak for themselves.
- Namespace-scoped free functions for view factories, classes for stateful objects
- `USING_NS_CC;` in .cpp files
- All C++14/17 features available
- Avoid speculative abstractions; prefer simple functions over premature utilities
