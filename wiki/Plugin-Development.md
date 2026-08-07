# C++ 原生扩展插件开发指南 (`./lang/do/`)

**Chinese-Interpreter-Kernel** 拥有极其强大的扩展能力。开发者可以通过在 `./lang/do/` 目录下放置 JSON 配置文件与 C++ `.h` 头文件，轻松为中文语言扩充自定义函数与类，**完全无需修改解释器核心源码 (`evaluator.h`)**。

---

## 1. 插件架构设计原理

扩展插件的工作机制如下：

```
                    ┌─────────────────────────┐
                    │  ./lang/do/my_plugin.json│ (声明中文语法、映射名与指定 .h 文件)
                    └────────────┬────────────┘
                                 │
                     规则扫描预处理器 (replace.h)
                     ├─ 校验指定的 C++ .h 文件是否存在 (不存在报 [FATAL] 终止)
                     └─ 将中文函数名注册到替换映射表
                                 │
                    ┌────────────┴────────────┐
                    │   ./lang/do/my_plugin.h  │ (包含 C++ 原生实现与注册机)
                    └────────────┬────────────┘
                                 │
                      REGISTER_NATIVE_FUNC 注册机
                                 │
                                 ▼
                     全自动挂载至 NativeRegistry 引擎
```

---

## 2. 插件开发“两步法”教程

假设你想为中文解释器添加一个 **`求立方根（x）`** 的内置数学函数：

### 第一步：在 `./lang/do/` 下添加 JSON 配置文件（例如 `math_ext.json`）

```json
{
    "title": "立方根扩展",
    "keywords": {
        "求立方根（{1}）": {
            "to": "do_cbrt（{1}）",
            "type": "function",
            "params": [
                { "name": "{1}", "type": "double" }
            ],
            "do": "math_ext.h"
        }
    }
}
```

> **注意**：JSON 规则中的 `"do"` 字段必须指定 `./lang/do/` 下的 `.h` 头文件名。解释器在初始化时会检测该 `.h` 文件是否存在。如果缺失，解释器会输出致命错误日志：
> `[FATAL] 动态扩展规则配置错误 [math_ext.json]: 指定的 C++ 实现文件不存在: lang/do/math_ext.h`
> 并安全终止解释执行。

### 第二步：在 `./lang/do/` 下编写 C++ 头文件（例如 `math_ext.h`）

```cpp
#ifndef MATH_EXT_H
#define MATH_EXT_H

#include <cmath>

// 1. 原生 C++ 函数逻辑实现
inline double do_cbrt(double x) {
    return std::cbrt(x);
}

// 2. 使用 REGISTER_NATIVE_FUNC 进行静态注册
REGISTER_NATIVE_FUNC(do_cbrt, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return 0.0;
    double x = valueToDouble(args[0]); // 使用解包工具
    return do_cbrt(x);
});

#endif
```

---

## 3. `REGISTER_NATIVE_FUNC` 静态注册机

`REGISTER_NATIVE_FUNC` 利用 C++ 全局/静态对象初始化机制，在程序启动时自动将 Handler Lambda 挂载至 `NativeRegistry` 注册表中：

### 语法格式
```cpp
REGISTER_NATIVE_FUNC(C++函数标识符, [](const std::vector<Value>& args) -> Value {
    // 1. 读取 args 动态数组
    // 2. 调用原生 C++ 函数逻辑
    // 3. 返回 double 数字或 std::string 文本
});
```

---

## 4. 参数解包辅助工具

在 Lambda 闭包中，为了方便从 `args`（类型为 `const std::vector<Value>&`）中取出所需数据类型，解释器提供了以下解包辅助函数：

### 4.1 提取数字 (`valueToDouble`)
```cpp
double num = valueToDouble(args[0]);
```
- 如果 `args[0]` 为 `double`，直接返回数值；
- 如果 `args[0]` 为字符串数值（如 `"3.14"`），自动转为 `3.14`。

### 4.2 提取文本 (`valueToString`)
```cpp
std::string text = valueToString(args[0]);
```
- 如果 `args[0]` 为字符串，直接返回文本；
- 如果 `args[0]` 为数字（如 `123.0`），自动转为 `"123"`。

---

## 5. 经典扩展插件示例

### 示例 1：双参数数学运算（开平方根 / 绝对值 / 求最大值）
查看源码：[lang/do/math_ext.h](file:///Users/sxt2204/Chinese%20Complier/lang/do/math_ext.h)

```cpp
REGISTER_NATIVE_FUNC(do_max, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    double a = valueToDouble(args[0]);
    double b = valueToDouble(args[1]);
    return std::max(a, b);
});
```

### 示例 2：输入输出控制 (I/O)
查看源码：[lang/do/io_ext.h](file:///Users/sxt2204/Chinese%20Complier/lang/do/io_ext.h)

```cpp
REGISTER_NATIVE_FUNC(do_print_line, [](const std::vector<Value>& args) -> Value {
    std::string line = "";
    for (const auto& a : args) line += valueToString(a);
    std::cout << line << std::endl;
    return 0.0;
});
```
