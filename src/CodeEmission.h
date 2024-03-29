#pragma once

#include "MiniDecafBaseVisitor.h"
#include <string>
#include <iostream>

// using symInTab = std::map<std::string, std::map<std::string, int>>; 
// using varTab = std::map<std::string, int>;

class CodeEmission : public MiniDecafBaseVisitor {
    public:
        antlrcpp::Any visitProg(MiniDecafParser::ProgContext *ctx);
        antlrcpp::Any visitFunc(MiniDecafParser::FuncContext *ctx);
        antlrcpp::Any visitFuncCall(MiniDecafParser::FuncCallContext *ctx);
        antlrcpp::Any visitBlock(MiniDecafParser::BlockContext *ctx);
        antlrcpp::Any visitRetStmt(MiniDecafParser::RetStmtContext *ctx);
        antlrcpp::Any visitIfElse(MiniDecafParser::IfElseContext *ctx);
        antlrcpp::Any visitCondExpr(MiniDecafParser::CondExprContext *ctx);
        antlrcpp::Any visitExprStmt(MiniDecafParser::ExprStmtContext *ctx);
        
        antlrcpp::Any visitUnaryOp(MiniDecafParser::UnaryOpContext *ctx);
        antlrcpp::Any visitParen(MiniDecafParser::ParenContext *ctx);
        antlrcpp::Any visitAddSub(MiniDecafParser::AddSubContext *ctx);
        antlrcpp::Any visitMulDiv(MiniDecafParser::MulDivContext *ctx);
        antlrcpp::Any visitEqual(MiniDecafParser::EqualContext *ctx);
        antlrcpp::Any visitCompare(MiniDecafParser::CompareContext *ctx);
        antlrcpp::Any visitLand(MiniDecafParser::LandContext *ctx);
        antlrcpp::Any visitLor(MiniDecafParser::LorContext *ctx);
            
        antlrcpp::Any visitInteger(MiniDecafParser::IntegerContext *ctx);
        antlrcpp::Any visitIdentifier(MiniDecafParser::IdentifierContext *ctx);
        antlrcpp::Any visitArryIndex(MiniDecafParser::ArryIndexContext *ctx);
        antlrcpp::Any visitVarDefine(MiniDecafParser::VarDefineContext *ctx);
        antlrcpp::Any visitAssign(MiniDecafParser::AssignContext *ctx);
        antlrcpp::Any visitGlobalArry(MiniDecafParser::GlobalArryContext *ctx);
        antlrcpp::Any visitLocalArry(MiniDecafParser::LocalArryContext *ctx);
        antlrcpp::Any visitGlobal(MiniDecafParser::GlobalContext *ctx);
        antlrcpp::Any visitCast(MiniDecafParser::CastContext *ctx);
   
        antlrcpp::Any visitForLoop(MiniDecafParser::ForLoopContext *ctx);
        antlrcpp::Any visitWhileLoop(MiniDecafParser::WhileLoopContext *ctx);
        antlrcpp::Any visitDoWhile(MiniDecafParser::DoWhileContext *ctx);
        antlrcpp::Any visitBreak(MiniDecafParser::BreakContext *ctx);
        antlrcpp::Any visitContinue(MiniDecafParser::ContinueContext *ctx);
        
        std::string pushReg(std::string reg);
        std::string popReg(std::string reg);
        std::string popReg(std::string reg0, std::string);

    private:
     
        std::ostringstream code_;
        std::ostringstream data_;
        std::ostringstream bss_;

        std::string curFunc;
        // symInTab varTable;
        // varTab varID;
        bool retState;
        int label;
        int blockID, stmtID;

        std::vector<int> breakTarget, continueTarget;


        // /* 
        //     Translation of IR to ASM;

        //     Support IR: push, pop
        // */
        // const char* push = "\taddi sp, sp, -4\n"
        //                 "\tsw a0, 0(sp)\n";

        // const char* pop = "\tlw t0, 0(sp)\n"
        //                 "\taddi sp, sp, 4\n";

        // const char* pop2 = "\tlw t0, 4(sp)\n"
        //                 "\tlw t1, 0(sp)\n"
        //                 "\taddi sp, sp, 8\n";

        const char* ret = "\taddi sp, fp, 4\n"
                        "\tlw ra, 0(sp)\n" 
                        "\tlw fp, -4(sp)\n"
                        "\tret\n";

    
        /*
            Specify return type of each operation
        */
        enum retType {LEFT, RIGHT, PTR, ARR, UNDEF};
};