#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "ast.h"
#include <unordered_map>
#include <variant>
#include <string>
#include <iostream>
#include <memory>
#include <functional>

using Value = std::variant<double, std::string>;

struct ReturnException {
    Value value;
};

inline void printValue(const Value& val) {
    if (std::holds_alternative<double>(val)) {
        std::cout << std::get<double>(val);
    } else {
        std::cout << std::get<std::string>(val);
    }
}

inline std::string valueToString(const Value& val) {
    if (std::holds_alternative<double>(val)) {
        double d = std::get<double>(val);
        if (d == static_cast<long long>(d)) {
            return std::to_string(static_cast<long long>(d));
        }
        return std::to_string(d);
    }
    return std::get<std::string>(val);
}

inline double valueToDouble(const Value& val) {
    if (std::holds_alternative<double>(val)) return std::get<double>(val);
    try { return std::stod(std::get<std::string>(val)); } catch (...) { return 0.0; }
}

inline std::string readInputLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

using NativeFunction = std::function<Value(const std::vector<Value>& args)>;

class NativeRegistry {
private:
    std::unordered_map<std::string, NativeFunction> functions;

    NativeRegistry() = default;

public:
    static NativeRegistry& instance() {
        static NativeRegistry reg;
        return reg;
    }

    void registerFunc(const std::string& name, NativeFunction fn) {
        functions[name] = std::move(fn);
    }

    bool hasFunc(const std::string& name) const {
        return functions.find(name) != functions.end();
    }

    Value call(const std::string& name, const std::vector<Value>& args) const {
        auto it = functions.find(name);
        if (it != functions.end()) {
            return it->second(args);
        }
        return 0.0;
    }
};

struct NativeFunctionRegistrar {
    NativeFunctionRegistrar(const std::string& name, NativeFunction fn) {
        NativeRegistry::instance().registerFunc(name, fn);
    }
};

#define REGISTER_NATIVE_FUNC(name, fn) \
    static NativeFunctionRegistrar _registrar_##name(#name, fn);

#include "../../lang/do/do_add.h"
#include "../../lang/do/string.h"
#include "../../lang/do/math_ext.h"
#include "../../lang/do/io_ext.h"
#include "../../lang/do/system_ext.h"

class Environment : public std::enable_shared_from_this<Environment> {
    std::unordered_map<std::string, Value> vars;
    std::shared_ptr<Environment> parent;

public:
    explicit Environment(std::shared_ptr<Environment> p = nullptr) : parent(std::move(p)) {}

    void set(const std::string& name, Value val) {
        if (vars.count(name) || !parent) {
            vars[name] = std::move(val);
        } else {
            parent->set(name, val);
        }
    }

    void define(const std::string& name, Value val) {
        vars[name] = std::move(val);
    }

    Value get(const std::string& name) const {
        auto it = vars.find(name);
        if (it != vars.end()) return it->second;
        if (parent) return parent->get(name);
        return 0.0;
    }
};

class Evaluator {
    std::unordered_map<std::string, std::string> stringMap;
    std::unordered_map<std::string, std::string> varMap;
    std::shared_ptr<Environment> globalEnv;
    std::unordered_map<std::string, FunctionDefNode*> functionTable;

public:
    Evaluator(std::unordered_map<std::string, std::string> strM,
              std::unordered_map<std::string, std::string> varM)
        : stringMap(std::move(strM)), varMap(std::move(varM)) {
        globalEnv = std::make_shared<Environment>();
    }

    Value eval(ASTNode* node, std::shared_ptr<Environment> env) {
        if (!node) return 0.0;

        if (auto lit = dynamic_cast<LiteralNode*>(node)) {
            if (lit->value.rfind("__STR_", 0) == 0 && stringMap.count(lit->value)) {
                std::string raw = stringMap[lit->value];
                if (raw.length() >= 2 && raw.front() == '"' && raw.back() == '"') {
                    return raw.substr(1, raw.length() - 2);
                }
                return raw;
            }
            try {
                return std::stod(lit->value);
            } catch (...) {
                return lit->value;
            }
        }

        if (auto var = dynamic_cast<VariableNode*>(node)) {
            return env->get(var->name);
        }

        if (auto assign = dynamic_cast<AssignNode*>(node)) {
            Value val = eval(assign->expr.get(), env);
            env->set(assign->varName, val);
            return val;
        }

        if (auto bin = dynamic_cast<BinaryOpNode*>(node)) {
            Value l = eval(bin->left.get(), env);
            Value r = eval(bin->right.get(), env);

            if (bin->op == "+") {
                if (std::holds_alternative<std::string>(l) || std::holds_alternative<std::string>(r)) {
                    return valueToString(l) + valueToString(r);
                }
                return std::get<double>(l) + std::get<double>(r);
            }
            if (bin->op == "-") return std::get<double>(l) - std::get<double>(r);
            if (bin->op == "*") return std::get<double>(l) * std::get<double>(r);
            if (bin->op == "/") return std::get<double>(l) / std::get<double>(r);
            if (bin->op == "%") return static_cast<double>(static_cast<long long>(std::get<double>(l)) % static_cast<long long>(std::get<double>(r)));
            if (bin->op == "<") return (std::get<double>(l) < std::get<double>(r)) ? 1.0 : 0.0;
            if (bin->op == ">") return (std::get<double>(l) > std::get<double>(r)) ? 1.0 : 0.0;
            if (bin->op == "<=") return (std::get<double>(l) <= std::get<double>(r)) ? 1.0 : 0.0;
            if (bin->op == ">=") return (std::get<double>(l) >= std::get<double>(r)) ? 1.0 : 0.0;
            if (bin->op == "==") return (l == r) ? 1.0 : 0.0;
            if (bin->op == "!=") return (l != r) ? 1.0 : 0.0;
        }

        if (auto ifNode = dynamic_cast<IfNode*>(node)) {
            Value cond = eval(ifNode->condition.get(), env);
            bool isTrue = std::holds_alternative<double>(cond) ? (std::get<double>(cond) != 0.0) : !std::get<std::string>(cond).empty();

            if (isTrue) {
                for (const auto& stmt : ifNode->body) {
                    eval(stmt.get(), env);
                }
            }
            return 0.0;
        }

        if (auto whileNode = dynamic_cast<WhileNode*>(node)) {
            while (true) {
                Value cond = eval(whileNode->condition.get(), env);
                bool isTrue = std::holds_alternative<double>(cond) ? (std::get<double>(cond) != 0.0) : !std::get<std::string>(cond).empty();
                if (!isTrue) break;

                for (const auto& stmt : whileNode->body) {
                    eval(stmt.get(), env);
                }
            }
            return 0.0;
        }

        if (auto funcDef = dynamic_cast<FunctionDefNode*>(node)) {
            functionTable[funcDef->name] = funcDef;
            return 0.0;
        }

        if (auto retNode = dynamic_cast<ReturnNode*>(node)) {
            Value val = eval(retNode->expr.get(), env);
            throw ReturnException{val};
        }

        if (auto methodNode = dynamic_cast<MethodCallNode*>(node)) {
            Value objVal = eval(methodNode->object.get(), env);
            std::string typeName = std::holds_alternative<std::string>(objVal) ? "string" : "double";
            std::string methodKey = typeName + "_" + methodNode->methodName;

            if (NativeRegistry::instance().hasFunc(methodKey)) {
                std::vector<Value> callArgs;
                callArgs.push_back(objVal);
                for (const auto& arg : methodNode->args) {
                    callArgs.push_back(eval(arg.get(), env));
                }
                Value resVal = NativeRegistry::instance().call(methodKey, callArgs);
                if (auto varNode = dynamic_cast<VariableNode*>(methodNode->object.get())) {
                    if (std::holds_alternative<std::string>(resVal) && std::holds_alternative<std::string>(objVal)) {
                        env->set(varNode->name, resVal);
                    }
                }
                return resVal;
            }
            return 0.0;
        }

        if (auto memberNode = dynamic_cast<MemberAccessNode*>(node)) {
            Value objVal = eval(memberNode->object.get(), env);
            std::string typeName = std::holds_alternative<std::string>(objVal) ? "string" : "double";
            std::string memberKey = typeName + "_" + memberNode->memberName;

            if (NativeRegistry::instance().hasFunc(memberKey)) {
                return NativeRegistry::instance().call(memberKey, {objVal});
            }
            return 0.0;
        }

        if (auto fn = dynamic_cast<FunctionCallNode*>(node)) {
            if (NativeRegistry::instance().hasFunc(fn->funcName)) {
                std::vector<Value> evalArgs;
                for (const auto& arg : fn->args) {
                    evalArgs.push_back(eval(arg.get(), env));
                }
                return NativeRegistry::instance().call(fn->funcName, evalArgs);
            }

            if (functionTable.count(fn->funcName)) {
                FunctionDefNode* def = functionTable[fn->funcName];
                auto localEnv = std::make_shared<Environment>(globalEnv);

                for (size_t i = 0; i < def->params.size() && i < fn->args.size(); ++i) {
                    Value argVal = eval(fn->args[i].get(), env);
                    localEnv->define(def->params[i], argVal);
                }

                try {
                    for (const auto& stmt : def->body) {
                        eval(stmt.get(), localEnv);
                    }
                } catch (const ReturnException& ret) {
                    return ret.value;
                }
                return 0.0;
            }
        }

        return 0.0;
    }

    void run(const std::vector<std::unique_ptr<ASTNode>>& program) {
        for (const auto& stmt : program) {
            eval(stmt.get(), globalEnv);
        }
    }
};

#endif