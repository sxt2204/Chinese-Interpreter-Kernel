#ifndef CUSTOM_IDIOM_EXT_H
#define CUSTOM_IDIOM_EXT_H

#include "../../source/interpreter/native_registry.h"
#include "../../source/interpreter/json.hpp"
#include "idioms_data.h"
#include <vector>
#include <string>
#include <iostream>

using json = nlohmann::json;

static json idiom_db;
static bool idiom_db_loaded = false;

REGISTER_NATIVE_FUNC(idiom_query, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 1 || !std::holds_alternative<std::string>(args[0])) return 0.0;
    std::string word = std::get<std::string>(args[0]);
    
    if (!idiom_db_loaded) {
        std::cout << "[INFO] 首次查询成语，正在加载中华成语大词典 (3万+词条)..." << std::endl;
        try {
            idiom_db = json::parse(builtin_idioms);
            idiom_db_loaded = true;
        } catch (...) {
            std::cerr << "[错误] 成语词典数据解析失败！" << std::endl;
            return 0.0;
        }
    }
    
    for (const auto& item : idiom_db) {
        if (item.contains("word") && item["word"] == word) {
            std::cout << "============== 成语查询结果 ==============" << std::endl;
            std::cout << "【成语】" << item.value("word", "") << std::endl;
            std::cout << "【拼音】" << item.value("pinyin", "") << std::endl;
            std::cout << "【释义】" << item.value("explanation", "") << std::endl;
            std::cout << "【出处】" << item.value("derivation", "") << std::endl;
            std::cout << "==========================================" << std::endl;
            return 1.0;
        }
    }
    
    std::cout << "未找到成语: " << word << std::endl;
    return 0.0;
});

#endif
