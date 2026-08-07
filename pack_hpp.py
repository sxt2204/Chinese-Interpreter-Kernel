import os
import re

def bundle():
    root_dir = os.path.dirname(os.path.abspath(__file__))
    output_path = os.path.join(root_dir, "release", "chinese_compiler.hpp")
    os.makedirs(os.path.join(root_dir, "release"), exist_ok=True)

    included_files = set()
    header_content = []

    header_content.append("// ==========================================================================\n")
    header_content.append("//   Chinese Compiler Kernel v0.2.0 - Single Header Library (单头文件发布版)\n")
    header_content.append("//   GitHub: https://github.com/sxt2204/Chinese-Interpreter-Kernel\n")
    header_content.append("// ==========================================================================\n\n")
    header_content.append("#ifndef CHINESE_COMPILER_SINGLE_HEADER_HPP\n")
    header_content.append("#define CHINESE_COMPILER_SINGLE_HEADER_HPP\n\n")

    files_to_bundle = [
        "source/interpreter/ast.h",
        "source/interpreter/parser.h",
        "lang/do/string.h",
        "lang/do/do_add.h",
        "lang/do/math_ext.h",
        "lang/do/io_ext.h",
        "lang/do/system_ext.h",
        "lang/do/file_ext.h",
        "source/interpreter/evaluator.h",
        "source/interpreter/replace.h",
        "source/api.hpp"
    ]

    for rel_path in files_to_bundle:
        full_path = os.path.join(root_dir, rel_path)
        if not os.path.exists(full_path):
            continue
        
        with open(full_path, "r", encoding="utf-8") as f:
            lines = f.readlines()
            header_content.append(f"// --- Begin File: {rel_path} ---\n")
            for line in lines:
                # Strip internal include directives to bundled local headers
                if line.strip().startswith('#include "') or line.strip().startswith('#include "../../'):
                    continue
                if line.strip().startswith("#ifndef ") or line.strip().startswith("#define ") or line.strip().startswith("#endif"):
                    if any(guard in line for guard in ["AST_H", "PARSER_H", "EVALUATOR_H", "REPLACE_H", "API_HPP", "CUSTOM_STRING_H", "MATH_EXT_H", "IO_EXT_H", "SYSTEM_EXT_H", "FILE_EXT_H"]):
                        continue
                header_content.append(line)
            header_content.append(f"// --- End File: {rel_path} ---\n\n")

    header_content.append("#endif // CHINESE_COMPILER_SINGLE_HEADER_HPP\n")

    with open(output_path, "w", encoding="utf-8") as out:
        out.writelines(header_content)

    print(f"[SUCCESS] 成功打包生成单头文件库: {output_path}")

if __name__ == "__main__":
    bundle()
