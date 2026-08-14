#!/bin/bash
set -e

echo "[BUILD] 正在打包单头文件库 release/chinese_compiler.hpp ..."
python3 pack_hpp.py

echo "[BUILD] 正在编译中文解释器测试代码 (C++17/C++20 标准兼容模式)..."

if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS: 使用内置 OpenGL/GLUT framework
    c++ -o test test.cpp -std=c++20 -I. -framework OpenGL -framework GLUT
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux: 使用 libGL, libGLU, libglut
    c++ -o test test.cpp -std=c++20 -I. -lGL -lGLU -lglut
elif [[ "$OSTYPE" == "msys"* || "$OSTYPE" == "cygwin"* ]]; then
    # Windows (MinGW/MSYS): 使用 opengl32, glu32, freeglut
    c++ -o test test.cpp -std=c++20 -I. -lopengl32 -lglu32 -lfreeglut
else
    # 默认/Fallback
    c++ -o test test.cpp -std=c++20 -I.
fi

echo "[BUILD] 编译成功！"
