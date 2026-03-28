---
inclusion: fileMatch
fileMatchPattern: "**/*.cpp,**/*.h,**/*.hpp,**/*.h.in"
---

# C++ 编码约定

## cocos2d-x 特定

- 使用 `USING_NS_CC` 宏代替 `using namespace cocos2d`
- 使用 `CREATE_FUNC` 宏生成工厂方法
- 使用 `CC_CALLBACK_N` 宏绑定回调
- 日志使用 `CCLOG`，不要用 `printf` 或 `std::cout`
- 资源文件路径相对于 `Resources/` 目录

## 现代 C++ 风格

- 重写方法使用 `override`，不要写 `virtual`
- 编译期常量使用 `static constexpr`
- 成员变量使用类内初始化（`int _counter = 0;`）
- 参数传递：对象类型用 `const T&`，基本类型直接传值
- 文件作用域常量放在匿名命名空间中
- 避免魔法数字，提取为命名常量并加注释

## 格式

- 缩进: 4 空格
- 列宽: 108
- 大括号: Linux 风格（函数体换行，控制语句同行）
- 指针对齐: 左对齐（`int* p`）

## 构建

- 新增源文件需更新 `CMakeLists.txt` 的 `GAME_SOURCE` 和 `GAME_HEADER`