#ifndef AST_H
#define AST_H

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

#endif