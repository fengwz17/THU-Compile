#pragma once

#include "MiniDecafBaseVisitor.h"
#include <string>
#include <iostream>

class CodeEmission : public MiniDecafBaseVisitor {
    public:
        antlrcpp::Any visitProg(MiniDecafParser::ProgContext *ctx);
        antlrcpp::Any visitStmt(MiniDecafParser::StmtContext *ctx);
        antlrcpp::Any visitExpr(MiniDecafParser::ExprContext *ctx);
        antlrcpp::Any visitNum(MiniDecafParser::NumContext *ctx);
        

    private:
        /*
            Stringstream used to store generated codes
        */
        std::ostringstream code_;
        /* 
            Translation of IR to ASM;

            Support IR: push, pop
        */
        const char* push = "\taddi sp, sp, -4\n"
                        "\tsw a0, (sp)\n";

        const char* pop = "\tlw t0, (sp)\n"
                        "\taddi sp, sp, 4\n";

    
        /*
            Specify return type of each operation
        */
        enum retType {UNDEF, INT};
};