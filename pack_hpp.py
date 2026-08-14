import os
import glob

def bundle():
    root_dir = os.path.dirname(os.path.abspath(__file__))
    output_path = os.path.join(root_dir, "release", "chinese_compiler.hpp")
    os.makedirs(os.path.join(root_dir, "release"), exist_ok=True)

    header_content = []

    header_content.append("// ==========================================================================\n")
    header_content.append("//   Chinese Compiler Kernel v0.2.0 - Single Header Library (全功能单头文件发布包)\n")
    header_content.append("//   内置 nlohmann/json + 独立 native_registry.h + 完整 lang/ 字典与 C++ 扩展！\n")
    header_content.append("//   GitHub: https://github.com/sxt2204/Chinese-Interpreter-Kernel\n")
    header_content.append("// ==========================================================================\n\n")
    header_content.append("#ifndef CHINESE_COMPILER_SINGLE_HEADER_HPP\n")
    header_content.append("#define CHINESE_COMPILER_SINGLE_HEADER_HPP\n\n")

    # 1. Collect all JSON files in lang/
    json_files = glob.glob(os.path.join(root_dir, "lang", "**", "*.json"), recursive=True)
    embedded_jsons = []

    for jf in json_files:
        try:
            with open(jf, "r", encoding="utf-8") as f:
                content = f.read().strip()
                if content:
                    embedded_jsons.append(content)
        except Exception as e:
            print(f"[WARN] 无法读取 JSON 文件 {jf}: {e}")

    # Build the full loadEmbeddedLangRules() code string
    embedded_func_code = []
    embedded_func_code.append("inline void loadEmbeddedLangRules() {\n")
    embedded_func_code.append("    static bool loaded = false;\n")
    embedded_func_code.append("    if (loaded) return;\n")
    embedded_func_code.append("    loaded = true;\n\n")
    embedded_func_code.append("    static const char* raw_json_rules[] = {\n")

    for raw_j in embedded_jsons:
        embedded_func_code.append(f'        R"raw_json({raw_j})raw_json",\n')

    embedded_func_code.append("    };\n\n")
    embedded_func_code.append("    for (const char* json_str : raw_json_rules) {\n")
    embedded_func_code.append("        try {\n")
    embedded_func_code.append("            json data = json::parse(json_str);\n")
    embedded_func_code.append('            if (data.contains("keywords") && data["keywords"].is_object()) {\n')
    embedded_func_code.append('                for (auto& [key, val] : data["keywords"].items()) {\n')
    embedded_func_code.append("                    if (val.is_string()) {\n")
    embedded_func_code.append("                        if (key.find('{') == std::string::npos) {\n")
    embedded_func_code.append("                            replaces[key] = val.get<std::string>();\n")
    embedded_func_code.append("                        }\n")
    embedded_func_code.append("                    } else if (val.is_object()) {\n")
    embedded_func_code.append('                        std::string toVal = val.value("to", key);\n')
    embedded_func_code.append("                        std::string cleanKey = cleanParamPlaceholders(key);\n")
    embedded_func_code.append("                        std::string cleanTo = cleanParamPlaceholders(toVal);\n")
    embedded_func_code.append("                        if (!cleanKey.empty() && !cleanTo.empty()) {\n")
    embedded_func_code.append("                            replaces[cleanKey] = cleanTo;\n")
    embedded_func_code.append("                        }\n")
    embedded_func_code.append('                        if (val.contains("members") && val["members"].is_array()) {\n')
    embedded_func_code.append('                            for (const auto& m : val["members"]) {\n')
    embedded_func_code.append("                                if (m.is_object()) {\n")
    embedded_func_code.append('                                    std::string mName = m.value("name", "");\n')
    embedded_func_code.append('                                    std::string mDo = m.value("do", "");\n')
    embedded_func_code.append("                                    if (!mName.empty() && !mDo.empty()) {\n")
    embedded_func_code.append("                                        std::string cleanMName = cleanParamPlaceholders(mName);\n")
    embedded_func_code.append("                                        std::string cleanMDo = cleanParamPlaceholders(mDo);\n")
    embedded_func_code.append("                                        replaces[cleanMName] = cleanMDo;\n")
    embedded_func_code.append("                                    }\n")
    embedded_func_code.append("                                }\n")
    embedded_func_code.append("                            }\n")
    embedded_func_code.append("                        }\n")
    embedded_func_code.append("                    }\n")
    embedded_func_code.append("                }\n")
    embedded_func_code.append("            }\n")
    embedded_func_code.append("        } catch (...) {}\n")
    embedded_func_code.append("    }\n")
    embedded_func_code.append("}\n")

    files_to_bundle = [
        "source/interpreter/includes.h",
        "source/interpreter/json.hpp",
        "source/interpreter/native_registry.h",
        "source/interpreter/ast.h",
        "source/interpreter/parser.h",
        "source/interpreter/evaluator.h",
        "source/interpreter/replace.h",
        "lang/do/string.h",
        "lang/do/do_add.h",
        "lang/do/math_ext.h",
        "lang/do/io_ext.h",
        "lang/do/system_ext.h",
        "lang/do/file_ext.h",
        "lang/do/opengl_ext.h",
        "source/api.hpp"
    ]

    for rel_path in files_to_bundle:
        full_path = os.path.join(root_dir, rel_path)
        if not os.path.exists(full_path):
            continue
        
        with open(full_path, "r", encoding="utf-8") as f:
            lines = f.readlines()
            header_content.append(f"// --- Begin File: {rel_path} ---\n")
            skip_func_body = False
            for line in lines:
                strip_line = line.strip()
                if strip_line.startswith('#include "') or strip_line.startswith('#include "../../'):
                    continue
                if "inline void loadEmbeddedLangRules() {" in line:
                    skip_func_body = True
                    header_content.extend(embedded_func_code)
                    continue
                if skip_func_body:
                    if strip_line == "}":
                        skip_func_body = False
                    continue
                header_content.append(line)
            header_content.append(f"// --- End File: {rel_path} ---\n\n")

    header_content.append("#endif // CHINESE_COMPILER_SINGLE_HEADER_HPP\n")

    with open(output_path, "w", encoding="utf-8") as out:
        out.writelines(header_content)

    print(f"[SUCCESS] 成功打包生成包含 native_registry.h 的单头文件库: {output_path}")

if __name__ == "__main__":
    bundle()
