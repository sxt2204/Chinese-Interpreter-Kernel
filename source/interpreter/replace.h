#ifndef REPLACE_H
#define REPLACE_H

#include "includes.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <cctype>

#include <filesystem>
namespace fs = std::filesystem;

using json = nlohmann::json;

inline std::unordered_map<std::string, std::string> replaces;

inline std::string cleanParamPlaceholders(const std::string& str) {
    std::string res = "";
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '{' || str[i] == '}' || str[i] == '(' || str[i] == ')' || str[i] == ',' ||
            str.compare(i, 3, "（") == 0 || str.compare(i, 3, "）") == 0 || str.compare(i, 3, "，") == 0) {
            if (str[i] == '{' || str[i] == '}') continue;
            break;
        }
        res += str[i];
    }
    while (!res.empty() && (res.back() == ' ' || res.back() == '\t')) res.pop_back();
    return res;
}

inline void loadEmbeddedLangRules() {
    // Single header generator replaces this body with embedded lang/ JSON rules
}

inline void readReplaceRules() {
    loadEmbeddedLangRules();
    const std::vector<std::string> jsonFiles = {
        "lang/calc/consts.json",
        "lang/calc/op.json",
        "lang/calc/types.json",
        "lang/control/control_flow.json",
        "lang/control/comp.json",
        "lang/functions/functions_define.json",
        "lang/io/io_main.json"
    };
    
    std::cout << "[LOG] 开始解析语言规则库..." << std::endl;
    size_t totalLoaded = 0;
    
    for (const auto& path : jsonFiles) {
        std::cout << "[LOG] 正在加载文件: " << path << std::endl;
        
        std::ifstream file(path);
        if (!file.is_open()) {
            file.open("../../" + path);
        }
        
        if (!file.is_open()) {
            std::cerr << "[ERROR] 路径找不到或无法打开文件: " << path << std::endl;
            continue;
        }
        
        try {
            json data = json::parse(file);
            size_t countBefore = replaces.size();
            
            if (data.contains("keywords") && data["keywords"].is_object()) {
                for (auto& [key, val] : data["keywords"].items()) {
                    if (val.is_string()) {
                        if (key.find('{') == std::string::npos) {
                            replaces[key] = val.get<std::string>();
                        }
                    }
                }
            }
            
            if (data.contains("patterns") && data["patterns"].is_object()) {
                for (auto& [key, val] : data["patterns"].items()) {
                    if (val.is_string()) {
                        if (key.find('{') == std::string::npos) {
                            replaces[key] = val.get<std::string>();
                        }
                    }
                }
            }
            
            size_t added = replaces.size() - countBefore;
            totalLoaded += added;
            std::string title = data.value("title", path);
            std::cout << "[SUCCESS] 模块 [" << title << "] 加载完成，成功注入 " << added << " 条词条" << std::endl;
        
        } catch (const json::parse_error& e) {
            std::cerr << "[FATAL] JSON 语法错误，文件解析中断 [" << path << "]: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] 读取文件发生未捕获异常 [" << path << "]: " << e.what() << std::endl;
        }
    }

    std::vector<std::string> doDirs = {"lang/do", "../../lang/do"};
    for (const auto& doDir : doDirs) {
        if (fs::exists(doDir) && fs::is_directory(doDir)) {
            std::cout << "[LOG] 正在扫描自定义功能目录: " << doDir << std::endl;
            for (const auto& entry : fs::directory_iterator(doDir)) {
                if (entry.path().extension() == ".json") {
                    std::string path = entry.path().string();
                    std::ifstream file(path);
                    if (!file.is_open()) continue;
                    
                    try {
                        json data = json::parse(file, nullptr, true, true);
                        size_t countBefore = replaces.size();

                        if (data.contains("keywords") && data["keywords"].is_object()) {
                            for (auto& [key, val] : data["keywords"].items()) {
                                if (val.is_object()) {
                                    std::string doFile = val.value("do", "");
                                    if (doFile.empty()) continue;
                                    if (doFile.find("这里写") != std::string::npos || doFile.find("这里填") != std::string::npos || entry.path().filename() == "格式.json") {
                                        continue;
                                    }

                                    if (doFile.rfind(".h") == std::string::npos) {
                                        doFile += ".h";
                                    }

                                    fs::path targetHeaderPath = fs::path(doDir) / doFile;
                                    if (!fs::exists(targetHeaderPath)) {
                                        std::cerr << "[FATAL] 动态扩展规则配置错误 [" << path << "]: 指定的 C++ 实现文件不存在: " << targetHeaderPath.string() << "，程序已停止解释！" << std::endl;
                                        throw std::runtime_error("找不到 ./lang/do/ 下对应的 .h 实现文件: " + doFile);
                                    }

                                    std::cout << "[SUCCESS] 校验并装载自定义 C++ 拓展实现: " << targetHeaderPath.string() << std::endl;

                                    std::string toVal = val.value("to", key);
                                    std::string cleanKey = cleanParamPlaceholders(key);
                                    std::string cleanTo = cleanParamPlaceholders(toVal);
                                    if (!cleanKey.empty() && !cleanTo.empty()) {
                                        replaces[cleanKey] = cleanTo;
                                    }

                                    if (val.contains("members") && val["members"].is_array()) {
                                        for (const auto& m : val["members"]) {
                                            if (m.is_object()) {
                                                std::string mName = m.value("name", "");
                                                std::string mDo = m.value("do", "");
                                                if (!mName.empty() && !mDo.empty()) {
                                                    std::string cleanMName = cleanParamPlaceholders(mName);
                                                    std::string cleanMDo = cleanParamPlaceholders(mDo);
                                                    replaces[cleanMName] = cleanMDo;
                                                }
                                            }
                                        }
                                    }
                                } else if (val.is_string()) {
                                    if (key.find('{') == std::string::npos) {
                                        replaces[key] = val.get<std::string>();
                                    }
                                }
                            }
                        }

                        size_t added = replaces.size() - countBefore;
                        totalLoaded += added;
                        std::string title = data.value("title", path);
                        std::cout << "[SUCCESS] 自定义功能模块 [" << title << "] (" << entry.path().filename().string() << ") 加载完成，注入 " << added << " 条词条" << std::endl;
                    } catch (const json::parse_error& e) {
                        std::cerr << "[FATAL] JSON 语法错误 [" << path << "]: " << e.what() << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "[ERROR] 读取文件发生未捕获异常 [" << path << "]: " << e.what() << std::endl;
                        throw;
                    }
                }
            }
            break;
        }
    }
    
    std::cout << "[LOG] 规则初始化结束，符号映射表累计存入 " << totalLoaded << " 条映射 (总规则数: " << replaces.size() << ")" << std::endl;
}

inline std::tuple<std::string, std::unordered_map<std::string, std::string>, std::unordered_map<std::string, std::string>> doReplace(const std::string& sourceCode) {
    std::unordered_map<std::string, std::string> stringMap;
    std::unordered_map<std::string, std::string> varMap;
    
    std::string codeNoStr = "";
    bool inString = false;
    std::string currentStr = "";
    
    for (size_t i = 0; i < sourceCode.length(); ++i) {
        char c = sourceCode[i];
        if (c == '"' && (i == 0 || sourceCode[i - 1] != '\\')) {
            currentStr += c;
            if (inString) {
                std::string placeholder = "__STR_" + std::to_string(stringMap.size()) + "__";
                stringMap[placeholder] = currentStr;
                codeNoStr += placeholder;
                currentStr = "";
                inString = false;
            } else {
                inString = true;
            }
        } else if (inString) {
            currentStr += c;
        } else {
            codeNoStr += c;
        }
    }
    if (inString) {
        codeNoStr += currentStr;
    }
    
    std::vector<std::pair<std::string, std::string>> puncts = {
        {"（", "("}, {"）", ")"}, {"，", ","}, {"；", ";"}, {"：", ":"}
    };
    for (const auto& [cn, en] : puncts) {
        size_t pos = 0;
        while ((pos = codeNoStr.find(cn, pos)) != std::string::npos) {
            codeNoStr.replace(pos, cn.length(), en);
            pos += en.length();
        }
    }
    
    std::vector<std::pair<std::string, std::string>> sortedRules(replaces.begin(), replaces.end());
    std::sort(sortedRules.begin(), sortedRules.end(), [](const auto& a, const auto& b) {
        return a.first.length() > b.first.length();
    });
    
    std::string finalCode = "";
    std::unordered_map<std::string, std::string> origVarToPlaceholder;
    
    size_t i = 0;
    while (i < codeNoStr.length()) {
        unsigned char c = static_cast<unsigned char>(codeNoStr[i]);
        
        bool isIdentifierStart = (c > 127) || std::isalpha(c) || c == '_';
        if (isIdentifierStart) {
            bool matchedKeyword = false;
            for (const auto& [key, val] : sortedRules) {
                if (codeNoStr.compare(i, key.length(), key) == 0) {
                    size_t nextPos = i + key.length();
                    if (nextPos < codeNoStr.length()) {
                        unsigned char nextC = static_cast<unsigned char>(codeNoStr[nextPos]);
                        unsigned char keyLastC = static_cast<unsigned char>(key.back());
                        if ((keyLastC > 127 || std::isalnum(keyLastC) || keyLastC == '_') &&
                            (nextC > 127 || std::isalnum(nextC) || nextC == '_')) {
                            continue;
                        }
                    }
                    finalCode += val;
                    i += key.length();
                    matchedKeyword = true;
                    break;
                }
            }
            if (matchedKeyword) continue;
            
            size_t start = i;
            while (i < codeNoStr.length()) {
                unsigned char ch = static_cast<unsigned char>(codeNoStr[i]);
                if (ch <= 127) {
                    if (!std::isalnum(ch) && ch != '_') break;
                    i++;
                } else {
                    if ((ch & 0xE0) == 0xC0) i += 2;
                    else if ((ch & 0xF0) == 0xE0) i += 3;
                    else if ((ch & 0xF8) == 0xF0) i += 4;
                    else i++;
                }
            }
            std::string token = codeNoStr.substr(start, i - start);

            if (token.rfind("__STR_", 0) == 0 && token.back() == '_') {
                finalCode += token;
            } else {
                if (origVarToPlaceholder.find(token) == origVarToPlaceholder.end()) {
                    std::string placeholder = "__VAR_" + std::to_string(varMap.size()) + "__";
                    varMap[placeholder] = token;
                    origVarToPlaceholder[token] = placeholder;
                }
                finalCode += origVarToPlaceholder[token];
            }
        } else {
            finalCode += codeNoStr[i];
            i++;
        }
    }

    return std::make_tuple(finalCode, stringMap, varMap);
}

#endif