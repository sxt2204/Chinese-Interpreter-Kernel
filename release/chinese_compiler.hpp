// ==========================================================================
//   Chinese Compiler Kernel v0.2.0 - Single Header Library (单头文件发布版)
//   GitHub: https://github.com/sxt2204/Chinese-Interpreter-Kernel
// ==========================================================================

#ifndef CHINESE_COMPILER_SINGLE_HEADER_HPP
#define CHINESE_COMPILER_SINGLE_HEADER_HPP

// --- Begin File: source/interpreter/ast.h ---

#include <memory>
#include <string>
#include <vector>
#include <iostream>

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(int depth = 0) const = 0;
};

inline void printIndent(int depth) {
    for (int i = 0; i < depth; ++i) std::cout << "  │ ";
}

struct LiteralNode : public ASTNode {
    std::string value;
    explicit LiteralNode(std::string val) : value(std::move(val)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "Literal: " << value << "\n";
    }
};

struct VariableNode : public ASTNode {
    std::string name;
    explicit VariableNode(std::string n) : name(std::move(n)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "Variable: " << name << "\n";
    }
};

struct BinaryOpNode : public ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(std::string op, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(std::move(op)), left(std::move(l)), right(std::move(r)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "BinaryOp [" << op << "]\n";
        if (left) left->print(depth + 1);
        if (right) right->print(depth + 1);
    }
};

struct AssignNode : public ASTNode {
    std::string varName;
    std::unique_ptr<ASTNode> expr;

    AssignNode(std::string name, std::unique_ptr<ASTNode> e)
        : varName(std::move(name)), expr(std::move(e)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "Assign [" << varName << " =]\n";
        if (expr) expr->print(depth + 1);
    }
};

struct IfNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;

    IfNode(std::unique_ptr<ASTNode> cond, std::vector<std::unique_ptr<ASTNode>> b)
        : condition(std::move(cond)), body(std::move(b)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "IfCondition:\n";
        if (condition) condition->print(depth + 1);
        printIndent(depth);
        std::cout << "  └─ Body:\n";
        for (const auto& stmt : body) {
            if (stmt) stmt->print(depth + 2);
        }
    }
};

struct WhileNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;

    WhileNode(std::unique_ptr<ASTNode> cond, std::vector<std::unique_ptr<ASTNode>> b)
        : condition(std::move(cond)), body(std::move(b)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "WhileLoop:\n";
        if (condition) condition->print(depth + 1);
        printIndent(depth);
        std::cout << "  └─ Body:\n";
        for (const auto& stmt : body) {
            if (stmt) stmt->print(depth + 2);
        }
    }
};

struct FunctionCallNode : public ASTNode {
    std::string funcName;
    std::vector<std::unique_ptr<ASTNode>> args;

    FunctionCallNode(std::string name, std::vector<std::unique_ptr<ASTNode>> a)
        : funcName(std::move(name)), args(std::move(a)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "CallFunction [" << funcName << "]\n";
        for (const auto& arg : args) {
            if (arg) arg->print(depth + 1);
        }
    }
};

struct FunctionDefNode : public ASTNode {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<ASTNode>> body;

    FunctionDefNode(std::string n, std::vector<std::string> p, std::vector<std::unique_ptr<ASTNode>> b)
        : name(std::move(n)), params(std::move(p)), body(std::move(b)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "FunctionDef [" << name << "] Params: (";
        for (size_t i = 0; i < params.size(); ++i) {
            std::cout << params[i] << (i + 1 < params.size() ? ", " : "");
        }
        std::cout << ")\n";
        printIndent(depth);
        std::cout << "  └─ Body:\n";
        for (const auto& stmt : body) {
            if (stmt) stmt->print(depth + 2);
        }
    }
};

struct ReturnNode : public ASTNode {
    std::unique_ptr<ASTNode> expr;

    explicit ReturnNode(std::unique_ptr<ASTNode> e) : expr(std::move(e)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "Return:\n";
        if (expr) expr->print(depth + 1);
    }
};

struct MethodCallNode : public ASTNode {
    std::unique_ptr<ASTNode> object;
    std::string methodName;
    std::vector<std::unique_ptr<ASTNode>> args;

    MethodCallNode(std::unique_ptr<ASTNode> obj, std::string method, std::vector<std::unique_ptr<ASTNode>> a)
        : object(std::move(obj)), methodName(std::move(method)), args(std::move(a)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "MethodCall [" << methodName << "]\n";
        if (object) object->print(depth + 1);
        for (const auto& arg : args) {
            if (arg) arg->print(depth + 1);
        }
    }
};

struct MemberAccessNode : public ASTNode {
    std::unique_ptr<ASTNode> object;
    std::string memberName;

    MemberAccessNode(std::unique_ptr<ASTNode> obj, std::string member)
        : object(std::move(obj)), memberName(std::move(member)) {}

    void print(int depth = 0) const override {
        printIndent(depth);
        std::cout << "MemberAccess [" << memberName << "]\n";
        if (object) object->print(depth + 1);
    }
};

#endif// --- End File: source/interpreter/ast.h ---

// --- Begin File: source/interpreter/parser.h ---

#include <vector>
#include <string>
#include <memory>
#include <cctype>
#include <iostream>

enum class TokenType { Number, Identifier, StringLiteral, Operator, Punctuation, Indent, Dedent, Eof };

struct Token {
    TokenType type;
    std::string value;
};

inline std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::vector<int> indentStack = {0};

    size_t i = 0;
    bool lineStart = true;

    while (i < code.length()) {
        if (lineStart) {
            lineStart = false;
            size_t spaceCount = 0;
            size_t tempPos = i;
            while (tempPos < code.length()) {
                char c = code[tempPos];
                if (c == ' ') spaceCount++;
                else if (c == '\t') spaceCount += 4;
                else break;
                tempPos++;
            }

            if (tempPos < code.length() && (code[tempPos] == '\n' || code[tempPos] == '\r')) {
                i = tempPos;
                if (i < code.length() && code[i] == '\r') i++;
                if (i < code.length() && code[i] == '\n') i++;
                lineStart = true;
                continue;
            }

            int currentIndent = static_cast<int>(spaceCount);
            int lastIndent = indentStack.back();

            if (currentIndent > lastIndent) {
                indentStack.push_back(currentIndent);
                tokens.push_back({TokenType::Indent, "INDENT"});
            } else if (currentIndent < lastIndent) {
                while (!indentStack.empty() && indentStack.back() > currentIndent) {
                    indentStack.pop_back();
                    tokens.push_back({TokenType::Dedent, "DEDENT"});
                }
            }
            i = tempPos;
        }

        if (i >= code.length()) break;

        char c = code[i];
        if (c == '\n' || c == '\r') {
            if (c == '\r' && i + 1 < code.length() && code[i + 1] == '\n') i++;
            i++;
            lineStart = true;
            continue;
        }

        if (std::isspace(static_cast<unsigned char>(c))) {
            i++;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c))) {
            std::string num = "";
            while (i < code.length() && std::isdigit(static_cast<unsigned char>(code[i]))) {
                num += code[i++];
            }
            tokens.push_back({TokenType::Number, num});
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            std::string ident = "";
            while (i < code.length() && (std::isalnum(static_cast<unsigned char>(code[i])) || code[i] == '_')) {
                ident += code[i++];
            }
            if (ident.rfind("__STR_", 0) == 0 && ident.back() == '_') {
                tokens.push_back({TokenType::StringLiteral, ident});
            } else {
                tokens.push_back({TokenType::Identifier, ident});
            }
            continue;
        }

        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>') {
            std::string op(1, c);
            if (i + 1 < code.length() && code[i + 1] == '=') {
                op += '=';
                i++;
            }
            tokens.push_back({TokenType::Operator, op});
            i++;
            continue;
        }

        if (c == '(' || c == ')' || c == ',' || c == '.') {
            tokens.push_back({TokenType::Punctuation, std::string(1, c)});
            i++;
            continue;
        }

        i++;
    }

    while (indentStack.size() > 1) {
        indentStack.pop_back();
        tokens.push_back({TokenType::Dedent, "DEDENT"});
    }

    tokens.push_back({TokenType::Eof, ""});
    return tokens;
}

class Parser {
    std::vector<Token> tokens;
    size_t pos = 0;

    Token peek() const {
        if (pos >= tokens.size()) return {TokenType::Eof, ""};
        return tokens[pos];
    }

    Token consume() {
        if (pos >= tokens.size()) return {TokenType::Eof, ""};
        return tokens[pos++];
    }

public:
    explicit Parser(std::vector<Token> tok) : tokens(std::move(tok)) {}

    std::unique_ptr<ASTNode> parseExpression() {
        auto left = parsePrimary();
        if (!left) return nullptr;

        while (peek().type == TokenType::Operator) {
            std::string op = consume().value;
            auto right = parsePrimary();
            if (!right) break;
            left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
        }
        return left;
    }

    std::unique_ptr<ASTNode> parsePrimary() {
        Token t = peek();
        if (t.type == TokenType::Eof) return nullptr;

        std::unique_ptr<ASTNode> expr = nullptr;

        if (t.type == TokenType::Operator && t.value == "-") {
            consume();
            auto right = parsePrimary();
            if (auto lit = dynamic_cast<LiteralNode*>(right.get())) {
                return std::make_unique<LiteralNode>("-" + lit->value);
            }
            return std::make_unique<BinaryOpNode>("*", std::make_unique<LiteralNode>("-1"), std::move(right));
        }

        if (t.type == TokenType::Number || t.type == TokenType::StringLiteral) {
            consume();
            expr = std::make_unique<LiteralNode>(t.value);
        } else if (t.type == TokenType::Identifier) {
            consume();
            if (peek().type == TokenType::Punctuation && peek().value == "(") {
                consume();
                std::vector<std::unique_ptr<ASTNode>> args;
                if (peek().value != ")" && peek().type != TokenType::Eof) {
                    while (peek().type != TokenType::Eof) {
                        auto arg = parseExpression();
                        if (arg) args.push_back(std::move(arg));
                        if (peek().value == ",") consume();
                        else break;
                    }
                }
                if (peek().value == ")") consume();
                expr = std::make_unique<FunctionCallNode>(t.value, std::move(args));
            } else {
                expr = std::make_unique<VariableNode>(t.value);
            }
        } else if (t.type == TokenType::Punctuation && t.value == "(") {
            consume();
            expr = parseExpression();
            if (peek().value == ")") consume();
        }

        while (expr && peek().type == TokenType::Punctuation && peek().value == ".") {
            consume();
            if (peek().type == TokenType::Identifier) {
                std::string member = consume().value;
                if (peek().type == TokenType::Punctuation && peek().value == "(") {
                    consume();
                    std::vector<std::unique_ptr<ASTNode>> args;
                    if (peek().value != ")" && peek().type != TokenType::Eof) {
                        while (peek().type != TokenType::Eof) {
                            auto arg = parseExpression();
                            if (arg) args.push_back(std::move(arg));
                            if (peek().value == ",") consume();
                            else break;
                        }
                    }
                    if (peek().value == ")") consume();
                    expr = std::make_unique<MethodCallNode>(std::move(expr), member, std::move(args));
                } else {
                    expr = std::make_unique<MemberAccessNode>(std::move(expr), member);
                }
            } else {
                break;
            }
        }

        return expr;
    }

    std::unique_ptr<ASTNode> parseStatement() {
        Token t = peek();
        if (t.type == TokenType::Eof) return nullptr;

        if (t.type == TokenType::Indent || t.type == TokenType::Dedent) {
            consume();
            return nullptr;
        }

        if (t.type == TokenType::Identifier && (t.value == "int" || t.value == "double" || t.value == "string")) {
            consume();
            if (peek().type == TokenType::Eof) return nullptr;
            std::string varName = consume().value;
            std::unique_ptr<ASTNode> initExpr = nullptr;
            if (peek().type == TokenType::Operator && peek().value == "=") {
                consume();
                initExpr = parseExpression();
            }
            return std::make_unique<AssignNode>(varName, std::move(initExpr));
        }

        if (t.type == TokenType::Identifier && pos + 1 < tokens.size() && tokens[pos + 1].value == "=") {
            std::string varName = consume().value;
            consume();
            auto expr = parseExpression();
            return std::make_unique<AssignNode>(varName, std::move(expr));
        }

        if (t.type == TokenType::Identifier && t.value == "if") {
            consume();
            auto cond = parseExpression();
            if (peek().type == TokenType::Identifier && peek().value == "then") consume();

            bool hasIndent = (peek().type == TokenType::Indent);
            if (hasIndent) consume();

            std::vector<std::unique_ptr<ASTNode>> body;
            while (peek().type != TokenType::Eof) {
                if (hasIndent && peek().type == TokenType::Dedent) {
                    consume();
                    break;
                }
                if (peek().type == TokenType::Identifier && peek().value == "endif") {
                    consume();
                    break;
                }
                if (!hasIndent && peek().type == TokenType::Identifier && (peek().value == "function" || peek().value == "endoffunction" || peek().value == "while" || peek().value == "endwhile")) {
                    break;
                }

                auto stmt = parseStatement();
                if (stmt) body.push_back(std::move(stmt));
                else {
                    if (peek().type == TokenType::Eof) break;
                    consume();
                }
            }
            if (!hasIndent && peek().type == TokenType::Identifier && peek().value == "endif") {
                consume();
            }
            return std::make_unique<IfNode>(std::move(cond), std::move(body));
        }

        if (t.type == TokenType::Identifier && t.value == "while") {
            consume();
            auto cond = parseExpression();
            if (peek().type == TokenType::Identifier && (peek().value == "do" || peek().value == "then")) consume();

            bool hasIndent = (peek().type == TokenType::Indent);
            if (hasIndent) consume();

            std::vector<std::unique_ptr<ASTNode>> body;
            while (peek().type != TokenType::Eof) {
                if (hasIndent && peek().type == TokenType::Dedent) {
                    consume();
                    break;
                }
                if (peek().type == TokenType::Identifier && peek().value == "endwhile") {
                    consume();
                    break;
                }
                if (!hasIndent && peek().type == TokenType::Identifier && (peek().value == "function" || peek().value == "endoffunction")) {
                    break;
                }

                auto stmt = parseStatement();
                if (stmt) body.push_back(std::move(stmt));
                else {
                    if (peek().type == TokenType::Eof) break;
                    consume();
                }
            }
            if (!hasIndent && peek().type == TokenType::Identifier && peek().value == "endwhile") {
                consume();
            }
            return std::make_unique<WhileNode>(std::move(cond), std::move(body));
        }

        if (t.type == TokenType::Identifier && t.value == "function") {
            consume();
            std::string funcName = "";
            if (peek().type == TokenType::Identifier) {
                funcName = consume().value;
            }
            std::vector<std::string> params;
            if (peek().type == TokenType::Punctuation && peek().value == "(") {
                consume();
                while (peek().type != TokenType::Eof && peek().value != ")") {
                    if (peek().type == TokenType::Identifier) {
                        params.push_back(consume().value);
                    }
                    if (peek().value == ",") consume();
                }
                if (peek().value == ")") consume();
            }

            bool hasIndent = (peek().type == TokenType::Indent);
            if (hasIndent) consume();

            std::vector<std::unique_ptr<ASTNode>> body;
            while (peek().type != TokenType::Eof) {
                if (hasIndent && peek().type == TokenType::Dedent) {
                    consume();
                    break;
                }
                if (peek().type == TokenType::Identifier && peek().value == "endoffunction") {
                    consume();
                    break;
                }
                if (peek().type == TokenType::Identifier && peek().value == "function") {
                    break;
                }

                auto stmt = parseStatement();
                if (stmt) body.push_back(std::move(stmt));
                else {
                    if (peek().type == TokenType::Eof) break;
                    consume();
                }
            }
            if (!hasIndent && peek().type == TokenType::Identifier && peek().value == "endoffunction") {
                consume();
            }
            return std::make_unique<FunctionDefNode>(funcName, params, std::move(body));
        }

        if (t.type == TokenType::Identifier && t.value == "return") {
            consume();
            auto expr = parseExpression();
            return std::make_unique<ReturnNode>(std::move(expr));
        }

        return parseExpression();
    }

    std::vector<std::unique_ptr<ASTNode>> parseProgram() {
        std::vector<std::unique_ptr<ASTNode>> stmts;
        while (peek().type != TokenType::Eof) {
            auto stmt = parseStatement();
            if (stmt) stmts.push_back(std::move(stmt));
            else {
                if (peek().type == TokenType::Eof) break;
                consume();
            }
        }
        return stmts;
    }
};

#endif// --- End File: source/interpreter/parser.h ---

// --- Begin File: lang/do/string.h ---

#include <string>
#include <algorithm>
#include <cctype>

class StringObject {
public:
    std::string value;

    explicit StringObject(std::string str = "") : value(std::move(str)) {}

    double get_length() const {
        return static_cast<double>(value.length());
    }

    void append(char c) {
        value += c;
    }

    std::string to_upper() const {
        std::string res = value;
        std::transform(res.begin(), res.end(), res.begin(), ::toupper);
        return res;
    }

    std::string to_lower() const {
        std::string res = value;
        std::transform(res.begin(), res.end(), res.begin(), ::tolower);
        return res;
    }

    double contains(const std::string& sub) const {
        return (value.find(sub) != std::string::npos) ? 1.0 : 0.0;
    }
};

REGISTER_NATIVE_FUNC(string_get_length, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return 0.0;
    std::string str = valueToString(args[0]);
    return StringObject(str).get_length();
});

REGISTER_NATIVE_FUNC(string_length, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return 0.0;
    std::string str = valueToString(args[0]);
    return StringObject(str).get_length();
});

REGISTER_NATIVE_FUNC(string_to_upper, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return "";
    std::string str = valueToString(args[0]);
    return StringObject(str).to_upper();
});

REGISTER_NATIVE_FUNC(string_to_lower, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return "";
    std::string str = valueToString(args[0]);
    return StringObject(str).to_lower();
});

REGISTER_NATIVE_FUNC(string_append, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return "";
    std::string str = valueToString(args[0]);
    std::string argStr = valueToString(args[1]);
    StringObject obj(str);
    if (!argStr.empty()) obj.append(argStr[0]);
    return obj.value;
});

REGISTER_NATIVE_FUNC(string_contains, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    std::string str = valueToString(args[0]);
    std::string sub = valueToString(args[1]);
    return StringObject(str).contains(sub);
});

#endif
// --- End File: lang/do/string.h ---

// --- Begin File: lang/do/do_add.h ---
#ifndef DO_ADD_H
#define DO_ADD_H

inline double do_add(double a, double b) {
    return a + b;
}

REGISTER_NATIVE_FUNC(do_add, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_add(valueToDouble(args[0]), valueToDouble(args[1]));
});

REGISTER_NATIVE_FUNC(add, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_add(valueToDouble(args[0]), valueToDouble(args[1]));
});

#endif
// --- End File: lang/do/do_add.h ---

// --- Begin File: lang/do/math_ext.h ---

#include <cmath>
#include <algorithm>

inline double do_sqrt(double x) { return std::sqrt(x); }
inline double do_abs(double x) { return std::abs(x); }
inline double do_ceil(double x) { return std::ceil(x); }
inline double do_floor(double x) { return std::floor(x); }
inline double do_round(double x) { return std::round(x); }
inline double do_max(double a, double b) { return std::max(a, b); }
inline double do_min(double a, double b) { return std::min(a, b); }
inline double do_sin(double x) { return std::sin(x); }
inline double do_cos(double x) { return std::cos(x); }
inline double do_tan(double x) { return std::tan(x); }
inline double do_log(double x) { return std::log10(x); }
inline double do_ln(double x) { return std::log(x); }

REGISTER_NATIVE_FUNC(do_sqrt, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sqrt(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(sqrt, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sqrt(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_abs, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_abs(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(abs, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_abs(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_ceil, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_ceil(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(ceil, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_ceil(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_floor, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_floor(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(floor, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_floor(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_round, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_round(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(round, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_round(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_max, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_max(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(max, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_max(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(do_min, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_min(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(min, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_min(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(do_sin, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sin(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(sin, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sin(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_cos, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_cos(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(cos, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_cos(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_tan, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_tan(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(tan, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_tan(valueToDouble(args[0])); });

#endif
// --- End File: lang/do/math_ext.h ---

// --- Begin File: lang/do/io_ext.h ---

#include <iostream>
#include <string>

inline void do_print_line(const std::string& text) { std::cout << text << std::endl; }
inline void do_output(const std::string& text) { std::cout << text; }
inline std::string do_input() { std::string l; std::getline(std::cin, l); return l; }

REGISTER_NATIVE_FUNC(do_print_line, [](const auto& args) -> Value {
    std::string line = "";
    for (const auto& a : args) line += valueToString(a);
    do_print_line(line);
    return 0.0;
});
REGISTER_NATIVE_FUNC(print_line, [](const auto& args) -> Value {
    std::string line = "";
    for (const auto& a : args) line += valueToString(a);
    do_print_line(line);
    return 0.0;
});
REGISTER_NATIVE_FUNC(do_output, [](const auto& args) -> Value {
    std::string text = "";
    for (const auto& a : args) text += valueToString(a);
    do_output(text);
    return 0.0;
});
REGISTER_NATIVE_FUNC(output, [](const auto& args) -> Value {
    std::string text = "";
    for (const auto& a : args) text += valueToString(a);
    do_output(text);
    return 0.0;
});
REGISTER_NATIVE_FUNC(do_input, [](const auto& args) -> Value { return do_input(); });
REGISTER_NATIVE_FUNC(input, [](const auto& args) -> Value { return do_input(); });

#endif
// --- End File: lang/do/io_ext.h ---

// --- Begin File: lang/do/system_ext.h ---

#include <ctime>
#include <thread>
#include <chrono>
#include <random>

inline double do_time_timestamp() {
    return static_cast<double>(std::time(nullptr));
}

inline void do_sleep_ms(double ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(ms)));
}

inline double do_random_int(double minVal, double maxVal) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<long long> dis(static_cast<long long>(minVal), static_cast<long long>(maxVal));
    return static_cast<double>(dis(gen));
}

REGISTER_NATIVE_FUNC(do_time_timestamp, [](const std::vector<Value>& args) -> Value {
    return do_time_timestamp();
});

REGISTER_NATIVE_FUNC(do_sleep_ms, [](const std::vector<Value>& args) -> Value {
    if (!args.empty()) do_sleep_ms(valueToDouble(args[0]));
    return 0.0;
});

REGISTER_NATIVE_FUNC(do_random_int, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_random_int(valueToDouble(args[0]), valueToDouble(args[1]));
});

#endif
// --- End File: lang/do/system_ext.h ---

// --- Begin File: lang/do/file_ext.h ---

#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

inline std::string do_read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::ostringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

inline double do_write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) return 0.0;
    file << content;
    return 1.0;
}

inline double do_append_file(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::app);
    if (!file.is_open()) return 0.0;
    file << content;
    return 1.0;
}

inline double do_file_exists(const std::string& path) {
    return std::filesystem::exists(path) ? 1.0 : 0.0;
}

REGISTER_NATIVE_FUNC(do_read_file, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return "";
    return do_read_file(valueToString(args[0]));
});

REGISTER_NATIVE_FUNC(do_write_file, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_write_file(valueToString(args[0]), valueToString(args[1]));
});

REGISTER_NATIVE_FUNC(do_append_file, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_append_file(valueToString(args[0]), valueToString(args[1]));
});

REGISTER_NATIVE_FUNC(do_file_exists, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return 0.0;
    return do_file_exists(valueToString(args[0]));
});

#endif
// --- End File: lang/do/file_ext.h ---

// --- Begin File: source/interpreter/evaluator.h ---

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

#endif// --- End File: source/interpreter/evaluator.h ---

// --- Begin File: source/interpreter/replace.h ---

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

inline void readReplaceRules() {
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

#endif// --- End File: source/interpreter/replace.h ---

// --- Begin File: source/api.hpp ---

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <functional>

namespace chinese_compiler {

struct StreamRedirector {
    std::ostream& stream;
    std::streambuf* oldBuffer;

    StreamRedirector(std::ostream& src, std::streambuf* newBuffer)
        : stream(src), oldBuffer(src.rdbuf(newBuffer)) {}

    ~StreamRedirector() {
        stream.rdbuf(oldBuffer);
    }
};

struct ExecutionResult {
    bool success = false;
    std::string output;
    std::string replacedCode;
    std::string astTree;
    std::string errorMessage;
    std::vector<Token> tokens;
};

class Interpreter {
private:
    bool rulesLoaded = false;

public:
    Interpreter() {
        initRules();
    }

    void initRules() {
        if (!rulesLoaded) {
            readReplaceRules();
            rulesLoaded = true;
        }
    }

    ExecutionResult execute(const std::string& chineseCode) {
        ExecutionResult res;
        if (!rulesLoaded) {
            initRules();
        }

        try {
            auto [replacedCode, stringMap, varMap] = doReplace(chineseCode);
            res.replacedCode = replacedCode;

            res.tokens = tokenize(replacedCode);

            Parser parser(res.tokens);
            std::vector<std::unique_ptr<ASTNode>> ast = parser.parseProgram();

            {
                std::stringstream astBuf;
                StreamRedirector redirectAst(std::cout, astBuf.rdbuf());
                for (const auto& node : ast) {
                    if (node) node->print();
                }
                res.astTree = astBuf.str();
            }

            {
                std::stringstream outBuf;
                StreamRedirector redirectOut(std::cout, outBuf.rdbuf());
                Evaluator evaluator(stringMap, varMap);
                evaluator.run(ast);
                res.output = outBuf.str();
            }

            res.success = true;
        } catch (const std::exception& e) {
            res.success = false;
            res.errorMessage = e.what();
        } catch (...) {
            res.success = false;
            res.errorMessage = "解释执行过程发生未知运行时错误。";
        }

        return res;
    }

    std::string preprocess(const std::string& chineseCode) {
        if (!rulesLoaded) initRules();
        auto [replacedCode, stringMap, varMap] = doReplace(chineseCode);
        return replacedCode;
    }

    std::vector<Token> tokenizeCode(const std::string& chineseCode) {
        if (!rulesLoaded) initRules();
        auto [replacedCode, stringMap, varMap] = doReplace(chineseCode);
        return tokenize(replacedCode);
    }

    std::string getASTString(const std::string& chineseCode) {
        if (!rulesLoaded) initRules();
        auto [replacedCode, stringMap, varMap] = doReplace(chineseCode);
        std::vector<Token> tokens = tokenize(replacedCode);
        Parser parser(tokens);
        auto ast = parser.parseProgram();

        std::stringstream astBuf;
        StreamRedirector redirectAst(std::cout, astBuf.rdbuf());
        for (const auto& node : ast) {
            if (node) node->print();
        }
        return astBuf.str();
    }

    void runDirect(const std::string& chineseCode, bool showLogs = true) {
        if (!rulesLoaded) initRules();

        auto [replacedCode, stringMap, varMap] = doReplace(chineseCode);
        std::vector<Token> tokens = tokenize(replacedCode);
        Parser parser(tokens);
        std::vector<std::unique_ptr<ASTNode>> ast = parser.parseProgram();

        if (showLogs) {
            std::cout << "\n================ [ 解释器运行结果 ] ================\n" << std::endl;
        }

        Evaluator evaluator(stringMap, varMap);
        evaluator.run(ast);

        if (showLogs) {
            std::cout << "\n================ [ 执行完毕 ] ================\n" << std::endl;
        }
    }

    void run(const std::string& chineseCode, bool printAST = false, bool showLogs = true) {
        if (!rulesLoaded) initRules();

        auto [replacedCode, stringMap, varMap] = doReplace(chineseCode);
        std::vector<Token> tokens = tokenize(replacedCode);
        Parser parser(tokens);
        std::vector<std::unique_ptr<ASTNode>> ast = parser.parseProgram();

        if (showLogs) {
            if (printAST) {
                std::cout << "\n================ [ AST 语法树 ] ================\n" << std::endl;
                for (const auto& node : ast) {
                    if (node) node->print();
                }
            }

            std::cout << "\n================ [ 解释器运行结果 ] ================\n" << std::endl;
        }

        Evaluator evaluator(stringMap, varMap);
        evaluator.run(ast);

        if (showLogs) {
            std::cout << "\n================ [ 执行完毕 ] ================\n" << std::endl;
        }
    }
};

inline void run(const std::string& chineseCode, bool printAST = false, bool showLogs = true) {
    static Interpreter interpreter;
    interpreter.run(chineseCode, printAST, showLogs);
}

inline void runDirect(const std::string& chineseCode, bool showLogs = true) {
    static Interpreter interpreter;
    interpreter.runDirect(chineseCode, showLogs);
}

inline ExecutionResult execute(const std::string& chineseCode) {
    static Interpreter interpreter;
    return interpreter.execute(chineseCode);
}

} // namespace chinese_compiler

// --- End File: source/api.hpp ---

#endif // CHINESE_COMPILER_SINGLE_HEADER_HPP
