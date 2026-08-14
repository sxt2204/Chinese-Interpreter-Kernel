#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
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
            bool hasDot = false;
            while (i < code.length() && (std::isdigit(static_cast<unsigned char>(code[i])) || (!hasDot && code[i] == '.'))) {
                if (code[i] == '.') hasDot = true;
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

#endif