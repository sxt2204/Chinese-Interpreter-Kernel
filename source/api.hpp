#ifndef API_HPP
#define API_HPP

#include "interpreter/replace.h"
#include "interpreter/parser.h"
#include "interpreter/evaluator.h"
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

#endif // CHINESE_COMPILER_API_HPP
