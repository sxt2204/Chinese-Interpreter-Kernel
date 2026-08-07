#include "./source/api.hpp"
#include <iostream>

using namespace std;

int main() {
    string chineseCode = R"(
文本 消息 赋值为 "Hello World"
整数 长度 赋值为 消息 的 获取长度（）
打印并换行（"消息 的 获取长度结果：" 加 长度）

消息 的 转为大写（）
打印并换行（"转为大写后的消息：" 加 消息）

双精度 时间戳 赋值为 获取当前时间戳（）
打印并换行（"当前系统时间戳：" 加 时间戳）

双精度 随机数 赋值为 生成随机数（10，99）
打印并换行（"生成随机数（10，99）结果：" 加 随机数）

双精度 平方根 赋值为 开平方根（25）
打印并换行（"开平方根（25）结果：" 加 平方根）
    )";

    cout << "=== 运行包含系统扩展与字符串大写调用的中文代码 ===" << endl;
    chinese_compiler::runDirect(chineseCode, true);

    return 0;
}