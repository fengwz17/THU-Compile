#include "StackAlloc.h"

//Visit the root node of AST
antlrcpp::Any StackAlloc::visitProg(MiniDecafParser::ProgContext *ctx, varTab& varID) {
    offset = 0;
    // std::cout << "d" << std::endl;
    visitChildren(ctx);
    // std::cout << "dd" << std::endl;
    // varID["a"] = 2;
    return varTable;
}

antlrcpp::Any StackAlloc::visitFunc(MiniDecafParser::FuncContext *ctx) {
    // std::cout << "dddddddd" << std::endl;
    curFunc = ctx->Identifier()->getText();
    // std::cout << curFunc << std::endl;
    blockID = 0;
    stmtID = 0;

    // error: redefinition
    if (varTable.count(curFunc) != 0) 
    {
        std::cerr << "[ERROR] Function Redifined" << curFunc << "\n";
        exit(1);
    }
    
    visitChildren(ctx);

    return retType::INT;
}

antlrcpp::Any StackAlloc::visitBlock(MiniDecafParser::BlockContext *ctx) {
    stmtID++;
    curFunc += "_" + std::to_string(blockID) + std::to_string(stmtID);
    for (auto it : ctx->blockItem()) 
    {
        visit(it);
    }

    stmtID--;
    if (stmtID == 0) 
    {
        blockID++;
    }
    int pos = curFunc.rfind('_');
    curFunc = curFunc.substr(0, pos);
    return retType::UNDEF;
}

antlrcpp::Any StackAlloc::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    // allocate the defined varible
    std::string varName = ctx->Identifier()->getText();
    varID[varName]++;
   
   
    if (varTable[curFunc].count(varName) != 0) 
    {
        std::cerr << "[ERROR] Variable Redifined" << varName << "\n";
        exit(1);
    }

    if (varTable[curFunc].count(varName + "#") > 0) 
    {
        varTable[curFunc][varName + "#"] =  varID[varName];
        // varTable[curFunc][varName + "#"] = ctx->start->getLine();
    }
    
    // std::cout << "varTable: " << curFunc << " " << varName << " " << varTable[curFunc][varName] << '\n';
    varTable[curFunc][varName] = offset++;
    // std::cout << "varTable: " << curFunc << " " << varName << " " << varTable[curFunc][varName] << '\n';

    return retType::INT;
}

antlrcpp::Any StackAlloc::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::string tmp = curFunc;
    
    for (int i = 0; i <= stmtID; i++) 
    {
        if (varTable[tmp].count(varName) == 0) 
        {
            int pos = tmp.rfind('_');
            tmp = tmp.substr(0, pos);
            continue;
        }
    
        if (varTable[curFunc].count(varName) == 0) 
        {
            varTable[curFunc][varName + "#"] = 0;
        }
        return retType::INT;
    }
    std::cerr << "[ERROR] Using: Variable " << varName << " have not been defined\n";
    exit(1);
}

antlrcpp::Any StackAlloc::visitAssign(MiniDecafParser::AssignContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    varID[varName]++;
   

    std::string tmpFunc = curFunc;
    // Search the symbol table from inner scope to the outer, for the target symbol
    for (int i = 0; i <= stmtID; i++) 
    {
        if (varTable[tmpFunc].count(varName) == 0) 
        {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            continue;
        }
        if (varTable[curFunc].count(varName) == 0) 
        {
            varTable[curFunc][varName + "#"] = 0;
        }
        return retType::INT;
    }
    std::cerr << "[ERROR] Assign: Variable " << varName << " have not been defined\n";
    exit(1);
}

antlrcpp::Any StackAlloc::visitForLoop(MiniDecafParser::ForLoopContext *ctx) {
    stmtID++;
    curFunc += "_" + std::to_string(blockID) + std::to_string(stmtID);
    visitChildren(ctx);
    stmtID--;
    if (stmtID == 0) 
    {
        blockID++;
    }
    int pos = curFunc.rfind('_');
    curFunc = curFunc.substr(0, pos);
    return retType::UNDEF;
}