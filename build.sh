#!/bin/bash
set -e

echo "[BUILD] 正在打包单头文件库 release/chinese_compiler.hpp ..."
python3 pack_hpp.py

echo "[BUILD] 正在编译中文解释器测试代码 (C++17/C++20 标准兼容模式)..."
c++ -o test test.cpp -std=c++17 -I.

echo "[BUILD] 编译成功！"
