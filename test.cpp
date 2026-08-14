#include "./source/api.hpp"
using namespace std;
int main(){
    string a = R"(
        打印并换行（"即将打开 OpenGL 绘图窗口..."）
        设置画笔颜色（1.0，0.0，0.0）
        绘制矩形（100，100，300，300）
        设置画笔颜色（0.0，0.0，1.0）
        绘制直线（100，300，200，400）
        绘制直线（300，300，200，400）
        设置画笔颜色（0.0，1.0，0.0）
        绘制点（200，200）
        显示绘图窗口（）
    )";
    chinese_compiler::Interpreter interpreter;
    interpreter.runDirect(a, false);
    return 0;
}