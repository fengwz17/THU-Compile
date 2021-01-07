#pragma once

#include "MiniDecafBaseVisitor.h"
#include <string>
#include <iostream>


using symTab = std::map<std::string, std::map<std::string, int>>; 

class CodeEmission : public MiniDecafBaseVisitor {
    public:
        antlrcpp::Any visitProg(MiniDecafParser::ProgContext *ctx, symTab& symbol_);
        antlrcpp::Any visitFunc(MiniDecafParser::FuncContext *ctx);
        antlrcpp::Any visitRetStmt(MiniDecafParser::RetStmtContext *ctx);
        antlrcpp::Any visitUnary(MiniDecafParser::UnaryContext *ctx);
        
        antlrcpp::Any visitAddSub(MiniDecafParser::AddSubContext *ctx);
        antlrcpp::Any visitMulDiv(MiniDecafParser::MulDivContext *ctx);
        antlrcpp::Any visitCompare(MiniDecafParser::CompareContext *ctx);
        antlrcpp::Any visitEqual(MiniDecafParser::EqualContext *ctx);
        antlrcpp::Any visitLand(MiniDecafParser::LandContext *ctx);
        antlrcpp::Any visitLor(MiniDecafParser::LorContext *ctx);
        
        antlrcpp::Any visitParen(MiniDecafParser::ParenContext *ctx);
        antlrcpp::Any visitInteger(MiniDecafParser::IntegerContext *ctx);

        antlrcpp::Any visitIdentifier(MiniDecafParser::IdentifierContext *ctx);
        antlrcpp::Any visitVarDefine(MiniDecafParser::VarDefineContext *ctx);
        antlrcpp::Any visitAssign(MiniDecafParser::AssignContext *ctx);


        
    private:
        /*
            Stringstream used to store generated codes
        */
        std::ostringstream code_;

        std::string funcName;
        symTab varTable;
        bool retState;

        /* 
            Translation of IR to ASM;

            Support IR: push, pop
        */
        const char* push = "\taddi sp, sp, -4\n"
                        "\tsw a0, (sp)\n";

        const char* pop = "\tlw t0, 0(sp)\n"
                        "\taddi sp, sp, 4\n";

        const char* pop2 = "\tlw t0, 4(sp)\n"
                        "\tlw t1, 0(sp)\n"
                        "\taddi sp, sp, 8\n";

    
        /*
            Specify return type of each operation
        */
        enum retType {UNDEF, INT};
};