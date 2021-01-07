#include "StackAlloc.h"

//Visit the root node of AST
antlrcpp::Any StackAlloc::visitProg(MiniDecafParser::ProgContext *ctx) {
    offset = 0;
    // std::cout << "d" << std::endl;
    visitChildren(ctx);
    // std::cout << "dd" << std::endl;
    return varTable;
}

antlrcpp::Any StackAlloc::visitFunc(MiniDecafParser::FuncContext *ctx) {
    // std::cout << "dddddddd" << std::endl;
    curFunc = ctx->Identifier()->getText();
    // std::cout << curFunc << std::endl;

    // error: redefinition
    if (varTable.count(curFunc) != 0) 
    {
        std::cerr << "[ERROR] Function Redifined" << curFunc << "\n";
        exit(1);
    }
    // std::cout << "dddddddd" << std::endl;
    visitChildren(ctx);
    // std::cout << "ddd" << std::endl;
    return retType::INT;
}

antlrcpp::Any StackAlloc::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    // allocate the defined varible
    std::string varName = ctx->Identifier()->getText();
    if (varTable[curFunc].count(varName) != 0) 
    {
        std::cerr << "[ERROR] Variable Redifined" << varName << "\n";
        exit(1);
    }
    varTable[curFunc][varName] = offset++;
    return retType::INT;
}

antlrcpp::Any StackAlloc::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();

    // error: var used without difination
    if (varTable[curFunc].count(varName) == 0) 
    {
        std::cerr << "[ERROR] Using: Variable " << varName << " have not been defined\n";
        exit(1);
    } 
    return retType::INT;
}

antlrcpp::Any StackAlloc::visitAssign(MiniDecafParser::AssignContext *ctx) {
    std::string varName = ctx->Identifier()->getText();

    // error: var used without assignment
    if (varTable[curFunc].count(varName) == 0) 
    {
        std::cerr << "[ERROR] Assign: Variable " << varName << " have not been defined\n";
        exit(1);
    }
    return retType::INT;
}

