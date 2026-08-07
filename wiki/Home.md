# 欢迎来到 中文解释器 (Chinese-Interpreter-Kernel) 官方 Wiki

**Chinese-Interpreter-Kernel** 是一款基于 C++20 构建的高性能、易拓展的全中文自然语言解释器内核。

它打破了传统编程语言必须依赖英文关键字的限制，通过 **Python 式自然缩进作用域**、**精准前缀替换预处理算法** 以及 **“即插即用”的 C++ 原生插件扩展引擎 (`./lang/do/`)**，让中文代码不仅具备自然的阅读体验，更能拥有原生 C++ 的强大性能与丰富生态。

---

## 核心特性

- **全中文自然语法**：支持全中文关键字（`定义函数`, `如果`, `当`, `返回`, `等于`）及丰富运算符。
- **Python 式自然缩进**：支持代码块缩进感知，无需繁琐地手写 `结束如果` / `结束当` / `结束函数`。
- **高性能原生 C++ 插件引擎 (`./lang/do/`)**：可通过简单的 JSON 配置文件与 C++ `.h` 头文件，无缝扩展自定义函数与类。
- **`REGISTER_NATIVE_FUNC` 注册机**：扩展函数在 C++ 启动时自动静态注册到引擎中，**零硬编码、无需修改解释器核心源码**。
- **装载期严格安全校验**：JSON 规则引用的 C++ 头文件不存在时自动拦截并抛出 `[FATAL]` 级致命错误，防止运行时崩溃。
- **开箱即用的 C++ API**：通过 `source/api.hpp` 提供极其简洁的集成接口（支持直接运行、打印 AST 以及获取结构化输出结果）。

---

## Wiki 导航

为了帮助你更高效地了解、使用与二次开发本项目，Wiki 包含以下详细章节：

1. **[中文语法与特性指南](Language-Syntax)**
   - 变量声明、`等于` 赋值、数据类型
   - 运算符与比较表达
   - 自然缩进、条件判断与循环
   - 函数定义与递归调用
   - 对象与方法链调用 (`消息.获取长度（）`)

2. **[C++ 原生插件开发指南 (`./lang/do/`)](Plugin-Development)**
   - 插件架构设计原理
   - 插件开发“两步法”（JSON 映射 + `.h` 头文件）
   - `REGISTER_NATIVE_FUNC` 注册机使用详解
   - 参数解包辅助工具（`valueToDouble` / `valueToString`）
   - 常见插件范例（数学扩展、IO 扩展、自定义类型）

3. **[C++ API 集成手册 (`source/api.hpp`)](API-Reference)**
   - 包含头文件与环境要求
   - `runDirect()`（静默/调试运行）
   - `run()`（打印 AST 语法树）
   - `execute()`（结构化 UI 结果捕获与 `ExecutionResult` 说明）
   - 构建与编译参数 (`build.sh`)

---

## 快速开始

### 1. 克隆仓库
```bash
git clone https://github.com/sxt2204/Chinese-Interpreter-Kernel.git
cd Chinese-Interpreter-Kernel
```

### 2. 运行构建脚本与测试
```bash
bash build.sh && ./test
```

### 3. 编写第一段中文代码
在 `test.cpp` 中引入 `api.hpp`：

```cpp
#include "./source/api.hpp"

int main() {
    std::string code = R"(
定义函数 阶乘（数值）
    如果 数值 小于 2 那么
        返回 1
    返回 数值 乘 阶乘（数值 减 1）

整数 结果 等于 阶乘（5）
输出并换行（"5的阶乘结果为：" 加 结果）
)";

    chinese_compiler::runDirect(code, false);
    return 0;
}
```
运行输出：
```
5的阶乘结果为：120
```
