#pragma once

#include "MiniDecafBaseVisitor.h"
#include <string>
#include <map>
#include <tuple>

using symTab = std::map<std::string, std::map<std::string, int>>;
using varTab = std::map<std::string, int>;

class StackAlloc : public MiniDecafBaseVisitor {
public:
    antlrcpp::Any visitProg(MiniDecafParser::ProgContext *ctx, varTab& varID);
    antlrcpp::Any visitFunc(MiniDecafParser::FuncContext *ctx);
        antlrcpp::Any visitFuncCall(MiniDecafParser::FuncCallContext *ctx);
    antlrcpp::Any visitVarDefine(MiniDecafParser::VarDefineContext *ctx);
    antlrcpp::Any visitIdentifier(MiniDecafParser::IdentifierContext *ctx);
    antlrcpp::Any visitAssign(MiniDecafParser::AssignContext *ctx);
    antlrcpp::Any visitBlock(MiniDecafParser::BlockContext *ctx);
    antlrcpp::Any visitForLoop(MiniDecafParser::ForLoopContext *ctx);


private:
    
    //Symbol table    
    symTab varTable;
    varTab varID;
    varTab funcTable;
   
    enum retType {UNDEF, INT};

    // Current function
    std::string curFunc;

    int offset;

    int blockID;
    int stmtID;

};