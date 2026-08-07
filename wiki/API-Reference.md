# C++ API 集成手册 (`source/api.hpp`)

本文档介绍如何在已有的 C++ 项目或桌面应用（如 UI / Web 嵌入）中调用中文解释器 API 接口。

---

## 1. 包含头文件与环境要求

### 包含头文件
```cpp
#include "./source/api.hpp"
```

### C++ 标准要求
需使用 **C++17** 或 **C++20** 标准编译，并在编译指令中包含项目根路径搜索选项（`-I.`）。

---

## 2. API 接口详解

API 全部包含在 `chinese_compiler` 命名空间中。

### 2.1 `runDirect` —— 直接运行中文代码

用于直接执行中文代码字符串，支持配置是否输出初始化日志。

#### 函数签名
```cpp
void chinese_compiler::runDirect(const std::string& code, bool showLogs = false);
```

#### 参数说明
- `code`: 要执行的中文源代码字符串。
- `showLogs`: 是否在控制台输出语言规则库与插件扫描装载日志（默认为 `false` 静默执行）。

#### 代码示例
```cpp
#include "./source/api.hpp"

int main() {
    std::string code = R"(
文本 消息 等于 "Hello World"
输出并换行（消息）
)";

    chinese_compiler::runDirect(code, false);
    return 0;
}
```

---

### 2.2 `run` —— 调试与 AST 语法树输出

用于调试中文代码，支持打印预处理替换后的 AST 语法树结构。

#### 函数签名
```cpp
void chinese_compiler::run(const std::string& code, bool printAST = false, bool showLogs = false);
```

#### 参数说明
- `code`: 中文源代码字符串。
- `printAST`: 是否在控制台打印解析出的 AST 节点结构树。
- `showLogs`: 是否输出模块装载日志。

#### 代码示例
```cpp
chinese_compiler::run(code, true, true);
```

---

### 2.3 `execute` —— 获取结构化执行结果对象

用于 UI/前端/客户端集成。该接口不会直接打印控制台日志，而是捕捉标准输出并包装在结构化对象 `ExecutionResult` 中返回。

#### 函数签名
```cpp
ExecutionResult chinese_compiler::execute(const std::string& code);
```

#### `ExecutionResult` 结构体定义

```cpp
struct ExecutionResult {
    bool success;                   // 执行是否成功
    std::string output;             // 程序控制台输出捕获文本
    std::string replacedCode;       // 预处理替换后的中间代码
    std::string astTree;            // 格式化的 AST 语法树结构
    std::string errorMessage;       // 错误/异常捕获日志
    std::vector<Token> tokens;      // 词法分析产生的 Token 列表
};
```

#### 代码示例
```cpp
#include "./source/api.hpp"
#include <iostream>

int main() {
    std::string code = R"(
整数 a 等于 10
整数 b 等于 20
整数 c 等于 add（a，b）
输出并换行（"计算结果：" 加 c）
)";

    chinese_compiler::ExecutionResult res = chinese_compiler::execute(code);
    if (res.success) {
        std::cout << "=== 输出内容 ===" << std::endl;
        std::cout << res.output << std::endl;
        std::cout << "=== 中间代码 ===" << std::endl;
        std::cout << res.replacedCode << std::endl;
    } else {
        std::cerr << "执行失败: " << res.errorMessage << std::endl;
    }
    return 0;
}
```

---

## 3. 构建与编译说明

### 编译指令示例

使用 `g++` 或 `clang++` 编译：

```bash
g++ -std=c++20 -I. your_main.cpp -o your_app
```

使用项目根目录下的 [build.sh](file:///Users/sxt2204/Chinese%20Complier/build.sh)：

```bash
bash build.sh && ./test
```
