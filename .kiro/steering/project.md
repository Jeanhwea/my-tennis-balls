---
inclusion: always
---

# my-tennis-balls 项目概览

这是一个基于 cocos2d-x 3.17.2 的 C++ 物理弹球小游戏。点击屏幕即可在点击位置生成弹球，弹球在重力作用下自由弹跳。

## 技术栈

- 语言: C++11
- 引擎: cocos2d-x 3.17.2
- 构建: CMake 3.10+
- 平台: Windows (VS 2022 win32), macOS (Xcode), Linux, Android, iOS

## 项目结构

- `Classes/` — 游戏源码（AppDelegate、HelloWorldScene、Version）
- `Resources/` — 游戏资源（ball.png）
- `cocos2d/` — 引擎源码（不要修改）
- `proj.win32/` / `proj.ios_mac/` / `proj.linux/` / `proj.android/` — 平台入口
- `Classes/Version.h.in` — 版本号模板，由 CMake configure_file 生成 `Version.h`

## 构建方式

Windows:
```bat
bld.bat
```

macOS:
```bash
./bld.sh
```

通用 CMake:
```bash
cmake -B build -DIS_DEBUG=ON
cmake --build build
```

## 编码规范

- 代码风格遵循 `.clang-format`（基于 Google 风格，缩进 4 空格，列宽 108）
- 静态分析遵循 `.clang-tidy`（启用 performance / readability / bugprone / modernize 检查）
- 使用 `override` 而非 `virtual` 标记重写方法
- 物理参数等常量使用 `static constexpr` 定义在类头文件中
- 优先使用 `const` 引用传参
- 避免冗余的 `this->` 前缀
- 匿名命名空间替代文件级 `static` 变量
- `Classes/Version.h` 是自动生成的，不要手动编辑，版本号在 `CMakeLists.txt` 的 `project(VERSION ...)` 中维护

## 注意事项

- `cocos2d/` 目录是引擎源码，不要修改其中的文件
- `IS_DEBUG` 宏通过 CMake 传入，控制调试绘制和 FPS 显示
- 物理碰撞使用 bitmask 分类（BALL=0x01, EDGE=0x02）
- 新增源文件需更新 `CMakeLists.txt` 的 `GAME_SOURCE` 和 `GAME_HEADER`
- 完成后运行 `fmt.bat` / `fmt.sh` 格式化，再运行 `bld.bat` / `bld.sh` 构建