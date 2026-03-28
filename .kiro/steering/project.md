---
inclusion: always
---

# my-tennis-balls

cocos2d-x 3.17.2 C++11 物理弹球游戏。

## 构建

- Windows: `bld.bat`
- macOS: `./bld.sh`

## 关键约束

- `cocos2d/` 是引擎源码，禁止修改
- `Classes/Version.h` 自动生成，版本号在 `CMakeLists.txt` 的 `project(VERSION ...)` 维护
- 新增源文件需更新 `CMakeLists.txt` 的 `GAME_SOURCE` / `GAME_HEADER`
- 完成后先 `fmt.bat` / `fmt.sh` 格式化，再构建

## 编码规范

- 遵循 `.clang-format`（Google 风格，缩进 4，列宽 108）和 `.clang-tidy`
- `override` 替代 `virtual`；`const` 引用传参；匿名命名空间替代文件级 `static`
- 常量用 `static constexpr`；避免冗余 `this->`
