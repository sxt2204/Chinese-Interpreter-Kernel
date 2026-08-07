#include "./source/api.hpp"
#include <iostream>

using namespace std;

int main() {
    string chineseCode = R"(
写入文件文本（"test.txt"，"Hello Chinese Compiler File IO!"）

双精度 存在结果 赋值为 文件是否存在（"test.txt"）
打印并换行（"文件是否存在测试：" 加 存在结果）

文本 内容 赋值为 读取文件文本（"test.txt"）
打印并换行（"读取文件文本内容为：" 加 内容）

追加文件文本（"test.txt"，" Appended line!"）
文本 追加后内容 赋值为 读取文件文本（"test.txt"）
打印并换行（"追加后的完整文件内容为：" 加 追加后内容）
    )";

    cout << "=== 运行文件 I/O 扩展功能的中文代码 ===" << endl;
    chinese_compiler::runDirect(chineseCode, true);

    return 0;
}