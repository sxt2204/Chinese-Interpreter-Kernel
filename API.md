# 中文解释器 (Chinese Compiler) API 与插件拓展文档

本文档介绍如何在 C++ 项目中集成中文解释器 API，以及如何使用 `./lang/do/` C++ 扩展插件机制为中文语言自定义函数与类。

---

## 目录

1. [C++ API 使用指南](#1-c-api-使用指南)
   - [包含头文件](#11-包含头文件)
   - [直接运行中文代码 (`runDirect`)](#12-直接运行中文代码-rundirect)
   - [打印语法树运行 (`run`)](#13-打印语法树运行-run)
   - [获取结构化执行结果 (`execute`)](#14-获取结构化执行结果-execute)
   - [读取中文源文件并执行](#15-读取中文源文件并执行)
2. [中文语言特性一览](#2-中文语言特性一览)
3. [C++ 原生扩展插件开发 (`./lang/do/`)](#3-c-原生扩展插件开发-langdo)
   - [插件开发两步法](#31-插件开发两步法)
   - [使用 `REGISTER_NATIVE_FUNC` 自动注册](#32-使用-register_native_func-自动注册)
   - [类型解包辅助工具](#33-类型解包辅助工具)
4. [编译与构建说明](#4-编译与构建说明)

---

## 1. C++ API 使用指南

### 1.1 包含头文件

```cpp
#include "./source/api.hpp"
```

### 1.2 直接运行中文代码 (`runDirect`)

```cpp
#include "./source/api.hpp"
#include <string>

int main() {
    std::string code = R"(
定义函数 阶乘（数值）
    如果 数值 小于 2 那么
        返回 1
    返回 数值 乘 阶乘（数值 减 1）

整数 结果 等于 阶乘（5）
输出并换行（"5的阶乘为：" 加 结果）
)";

    // 执行中文代码 (参数 2 为 false 表示静默执行，只输出程序运行结果)
    chinese_compiler::runDirect(code, false);
    return 0;
}
```

#### 函数签名
```cpp
void runDirect(const std::string& code, bool showLogs = false);
```
- `code`：中文代码字符串
- `showLogs`：是否输出解释器初始化及词条替换日志（默认 `false`）

---

### 1.3 打印语法树运行 (`run`)

```cpp
chinese_compiler::run(code, true, true);
```

#### 函数签名
```cpp
void run(const std::string& code, bool printAST = false, bool showLogs = false);
```
- `code`：中文代码字符串
- `printAST`：是否在控制台打印 AST 语法树结构
- `showLogs`：是否输出运行日志

---

### 1.4 获取结构化执行结果 (`execute`)

用于前端 UI、IDE 或桌面客户端集成，返回包含捕获输出、词法 Token、预处理代码、AST 字符串的结构化对象：

```cpp
#include "./source/api.hpp"
#include <iostream>

int main() {
    std::string code = R"(
文本 消息 等于 "Hello World"
输出并换行（消息）
)";

    chinese_compiler::ExecutionResult res = chinese_compiler::execute(code);
    if (res.success) {
        std::cout << "程序控制台输出:\n" << res.output << std::endl;
        std::cout << "替换后的中间代码:\n" << res.replacedCode << std::endl;
    } else {
        std::cerr << "执行失败: " << res.errorMessage << std::endl;
    }
    return 0;
}
```

#### `ExecutionResult` 结构体字段说明

| 字段 | 类型 | 说明 |
| :--- | :--- | :--- |
| `success` | `bool` | 执行是否成功 |
| `output` | `std::string` | 程序内部 `cout` 输出的控制台文本捕获 |
| `replacedCode` | `std::string` | 规则预处理替换后的标准中间代码 |
| `astTree` | `std::string` | 格式化的 AST 语法树结构字符串 |
| `errorMessage` | `std::string` | 异常或错误捕捉日志 |
| `tokens` | `std::vector<Token>` | 词法分析产生的 Token 序列 |

---

### 1.5 读取中文源文件并执行

```cpp
#include "./source/api.hpp"
#include <fstream>
#include <sstream>

std::string readSourceFile(const std::string& path) {
    std::ifstream in(path);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

int main() {
    std::string code = readSourceFile("./test.txt");
    chinese_compiler::runDirect(code, false);
    return 0;
}
```

---

## 2. 中文语言特性一览

- **Python 式自然缩进**：无需编写 `结束如果`、`结束当`、`结束函数`，通过自然缩进管理代码块作用域。
- **中文赋值**：支持使用 `等于` 进行变量赋值（如：`整数 计数 等于 10`）。
- **完整运算符**：支持 `加` (+)、`减` (-)、`乘` (*)、`除` (/)、`取模` (%)、`等于` (==)、`不等于` (!=)、`大于` (>)、`小于` (<)、`大于等于` (>=)、`小于等于` (<=) 及单目负号 (`-`)。
- **面向对象方法点调用**：支持 `对象.方法（参数）`（如 `消息.获取长度（）`）。

---

## 3. C++ 原生扩展插件开发 (`./lang/do/`)

任何开发者均可通过在 `./lang/do/` 目录下添加配置与 C++ 头文件，为解释器拓展全新的自定义函数或类，**完全无需修改解释器核心源码 (`evaluator.h`)**。

### 3.1 插件开发两步法

#### 第一步：在 `./lang/do/` 下添加 JSON 配置文件（例如 `math_ext.json`）

```json
{
    "title": "立方根扩展",
    "keywords": {
        "求立方根（{1}）": {
            "to": "do_cbrt（{1}）",
            "type": "function",
            "do": "math_ext.h"
        }
    }
}
```
*注：如果指定 `"do"` 的 `.h` 文件不存在，解释器在启动时会自动抛出 `[FATAL]` 级致命错误并安全终止。*

#### 第二步：在 `./lang/do/` 下编写对应的 C++ 头文件（例如 `math_ext.h`）

```cpp
#ifndef MATH_EXT_H
#define MATH_EXT_H

#include <cmath>

inline double do_cbrt(double x) {
    return std::cbrt(x);
}

// 使用 REGISTER_NATIVE_FUNC 实现静态自动注册
REGISTER_NATIVE_FUNC(do_cbrt, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return 0.0;
    double x = valueToDouble(args[0]); // 参数解包
    return do_cbrt(x);
});

#endif
```

---

### 3.2 使用 `REGISTER_NATIVE_FUNC` 自动注册

`REGISTER_NATIVE_FUNC` 利用 C++ 静态对象初始化机制，在程序启动时自动挂载函数至 `NativeRegistry` 注册表：

```cpp
REGISTER_NATIVE_FUNC(注册的函数名, [](const std::vector<Value>& args) -> Value {
    // 1. 读取 args 动态数组
    // 2. 执行 C++ 逻辑
    // 3. 返回 double 或 std::string
});
```

---

### 3.3 类型解包辅助工具

在 Lambda 闭包中可使用以下辅助转换函数：

- **`valueToDouble(args[i])`**：提取第 `i` 个参数为 `double` 数字。
- **`valueToString(args[i])`**：提取第 `i` 个参数为 `std::string` 字符串。

---

## 4. 编译与构建说明

推荐使用 C++20/C++17 标准编译项目，需包含当前根目录搜索路径 (`-I.`):

```bash
c++ -o test test.cpp -std=c++20 -I.
```

构建脚本在项目根目录为 [build.sh](file:///Users/sxt2204/Chinese%20Complier/build.sh)。直接运行以下命令即可：

```bash
bash build.sh && ./test
```
