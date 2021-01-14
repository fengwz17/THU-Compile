#pragma once

#include "MiniDecafBaseVisitor.h"
#include <string>
#include <map>
#include <tuple>

using symInTab = std::map<std::string, std::map<std::string, int>>;
using varTab = std::map<std::string, int>;

class StackAlloc : public MiniDecafBaseVisitor {
public:
    antlrcpp::Any visitFunc(MiniDecafParser::FuncContext *ctx);
    antlrcpp::Any visitBlock(MiniDecafParser::BlockContext *ctx);
    antlrcpp::Any visitForLoop(MiniDecafParser::ForLoopContext *ctx);

    antlrcpp::Any visitGlobal(MiniDecafParser::GlobalContext *ctx);
    antlrcpp::Any visitVarDefine(MiniDecafParser::VarDefineContext *ctx);
    antlrcpp::Any visitAssign(MiniDecafParser::AssignContext *ctx);

    antlrcpp::Any visitCondExpr(MiniDecafParser::CondExprContext *ctx);
    	    
    antlrcpp::Any visitLor(MiniDecafParser::LorContext *ctx);    
    antlrcpp::Any visitLand(MiniDecafParser::LandContext *ctx);
    antlrcpp::Any visitEqual(MiniDecafParser::EqualContext *ctx);

    antlrcpp::Any visitCompare(MiniDecafParser::CompareContext *ctx);
    antlrcpp::Any visitUnaryOp(MiniDecafParser::UnaryOpContext *ctx);
    antlrcpp::Any visitCast(MiniDecafParser::CastContext *ctx);
    antlrcpp::Any visitFuncCall(MiniDecafParser::FuncCallContext *ctx);
    antlrcpp::Any visitParen(MiniDecafParser::ParenContext *ctx);
    antlrcpp::Any visitIdentifier(MiniDecafParser::IdentifierContext *ctx);
    antlrcpp::Any visitInteger(MiniDecafParser::IntegerContext *ctx);

    antlrcpp::Any visitType(MiniDecafParser::TypeContext *ctx);
    
private:
    
    //Symbol table    
    symInTab varTable;
    varTab varID;
    varTab funcParaTable;
    std::set<std::string> funcDeName;
   
    enum retType {UNDEF, INT};

    // Current function
    std::string curFunc;

    int offset;

    int blockID;
    int stmtID;

};