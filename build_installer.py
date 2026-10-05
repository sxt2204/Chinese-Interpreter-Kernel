import base64
import zlib
import os

def build():
    # 确保 release/chinese_compiler.hpp 存在
    header_path = "release/chinese_compiler.hpp"
    if not os.path.exists(header_path):
        print("未找到 release/chinese_compiler.hpp！请先运行 bash build.sh")
        return

    print("正在读取 11MB 的单头文件...")
    with open(header_path, "rb") as f:
        header_data = f.read()

    print("正在压缩数据...")
    compressed = zlib.compress(header_data, level=9)
    b64_data = base64.b64encode(compressed).decode("utf-8")

    # 创建一个通用的 CLI 运行器 main.cpp
    main_cpp = """
#include <iostream>
#include <fstream>
#include <sstream>
#include "chinese_compiler.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "用法: chinese-c <文件路径.txt>\\n";
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "无法打开文件: " << argv[1] << "\\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();

    chinese_compiler::Interpreter interpreter;
    auto res = interpreter.execute(code);
    std::cout << res.output;
    return 0;
}
    """
    
    main_cpp_compressed = zlib.compress(main_cpp.encode('utf-8'), level=9)
    main_b64 = base64.b64encode(main_cpp_compressed).decode('utf-8')

    installer_code = f"""#!/usr/bin/env python3
import os
import sys
import zlib
import base64
import subprocess
import time
import threading

HEADER_B64 = "{b64_data}"
MAIN_B64 = "{main_b64}"

def extract_files():
    print("[1/3] 正在解压 11MB 的中文编译器核心代码库...")
    with open("chinese_compiler.hpp", "wb") as f:
        f.write(zlib.decompress(base64.b64decode(HEADER_B64)))
    with open("main.cpp", "wb") as f:
        f.write(zlib.decompress(base64.b64decode(MAIN_B64)))

def show_progress():
    phases = [
        ("预处理庞大的中华成语字典 (9.8MB)", 3),
        ("编译 AST 抽象语法树解析器", 2),
        ("编译 Evaluator 核心执行器", 2),
        ("编译 OpenGL 原生图形驱动模块", 3),
        ("编译 SQLite 模块与系统拓展", 2),
        ("正在进行最后的链接优化 (-O3)", 4)
    ]
    total_len = 40
    print("[2/3] 开始分块编译组件，这可能需要几十秒时间，请耐心等待...")
    
    # 进度条模拟，同时 g++ 在后台真正跑
    for phase_name, t in phases:
        sys.stdout.write(f"\\r[=>\\t] 正在编译: {{phase_name}}")
        sys.stdout.flush()
        steps = int(t / 0.2)
        for _ in range(steps):
            if not getattr(threading.current_thread(), "keep_running", True):
                break
            time.sleep(0.2)
        if not getattr(threading.current_thread(), "keep_running", True):
            break

def compile_code():
    # 检测系统
    if sys.platform == "darwin":
        cmd = ["c++", "-o", "chinese-c", "main.cpp", "-std=c++20", "-O3", "-framework", "OpenGL", "-framework", "GLUT"]
    elif sys.platform.startswith("linux"):
        cmd = ["c++", "-o", "chinese-c", "main.cpp", "-std=c++20", "-O3", "-lGL", "-lX11"]
    else:
        cmd = ["c++", "-o", "chinese-c", "main.cpp", "-std=c++20", "-O3", "-lopengl32", "-lgdi32"]

    try:
        proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return proc
    except Exception as e:
        print(f"编译异常: {{e}}")
        sys.exit(1)

def main():
    print("=========================================")
    print("      中文编程语言 (Chinese C) 安装器      ")
    print("=========================================")
    
    extract_files()
    
    progress_thread = threading.Thread(target=show_progress)
    progress_thread.keep_running = True
    progress_thread.start()
    
    # 真正的编译过程
    proc = compile_code()
    
    # 停止进度条
    progress_thread.keep_running = False
    progress_thread.join()
    sys.stdout.write("\\r" + " " * 80 + "\\r") # 清除进度行
    
    if proc.returncode != 0:
        print("[错误] 编译失败！")
        print(proc.stderr.decode('utf-8', errors='ignore'))
        sys.exit(1)
        
    print("[3/3] 编译完成！可执行文件已生成。")
    print("\\n安装成功！你可以通过以下命令运行中文代码文件：")
    print("  ./chinese-c <你的文件.txt>")
    
    # 清理临时文件
    try:
        os.remove("chinese_compiler.hpp")
        os.remove("main.cpp")
    except:
        pass

if __name__ == "__main__":
    main()
"""

    with open("install.py", "w", encoding="utf-8") as f:
        f.write(installer_code)
    
    os.chmod("install.py", 0o755)
    print("成功生成安装器！文件名为: install.py")
    print("大小: {:.2f} MB".format(os.path.getsize("install.py") / (1024 * 1024)))

if __name__ == "__main__":
    build()
