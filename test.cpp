#include "./source/api.hpp"
using namespace std;
int main(){
    string a = R"(
        设置窗口标题（"中文编程 - OpenGL丰富指令演示"）
        设置背景颜色（0.9，0.9，0.9）
        打印并换行（"即将打开 OpenGL 绘图窗口..."）
        
        设置画笔粗细（3.0）
        设置画笔颜色（1.0，0.0，0.0）
        绘制三角形（100，100，300，100，200，300）
        
        设置画笔颜色（0.0，0.5，1.0）
        绘制圆形（500，300，100）
        
        保存画布状态（）
        平移画布（400，400）
        旋转画布（45）
        设置画笔颜色（1.0，0.5，0.0）
        绘制矩形（-50，-50，50，50）
        恢复画布状态（）
        
        设置画笔颜色（0.0，0.0，0.0）
        开始绘制多边形（）
        添加多边形顶点（600，100）
        添加多边形顶点（700，100）
        添加多边形顶点（750，200）
        添加多边形顶点（650，250）
        添加多边形顶点（550，200）
        结束绘制多边形（）

        显示绘图窗口（）
    )";
    chinese_compiler::Interpreter interpreter;
    auto res = interpreter.execute(a);
    std::cout << "[DEBUG] Replaced Code:\n" << res.replacedCode << "\n";
    std::cout << "[DEBUG] Tokens count: " << res.tokens.size() << "\n";
    for(auto t : res.tokens) {
        std::cout << "Token: type=" << (int)t.type << ", value='" << t.value << "'\n";
    }
    std::cout << "[DEBUG] AST:\n" << res.astTree << "\n";
    if (!res.errorMessage.empty()) std::cout << "[ERROR] " << res.errorMessage << "\n";
    return 0;
}