#include "CodeEmission.h"

// Visiting AST from the root node
// return: Generated asm code
antlrcpp::Any CodeEmission::visitProg(MiniDecafParser::ProgContext *ctx) {
    code_ << ".section .text\n"
        << ".globl main\n"
        << "main:\n"; 
    visitChildren(ctx);
    return code_.str();
}

// Visit ReturnStmt node, son of Stmt node
antlrcpp::Any CodeEmission::visitStmt(MiniDecafParser::StmtContext *ctx) {
    visitChildren(ctx);
    
    code_ << "\tret";
    return retType::UNDEF;
}

// Visit Expr node, which is a single Integer node in this step
// return: Expr type
antlrcpp::Any CodeEmission::visitNum(MiniDecafParser::NumContext *ctx) {
    std::string strLiteral = ctx->Integer()->getText();
    long long numLiteral = std::stoll(strLiteral);
    if (numLiteral > INT32_MAX) {
        std::cerr << "line " << ctx->start->getLine() << ": "
                  << "[ERROR] Constant out of INT range.\n";
        exit(1);
    }
    
    code_ << "\tli a0, " << ctx->Integer()->getText() << "\n"
          << push;

    return retType::INT;
}

antlrcpp::Any CodeEmission::visitExpr(MiniDecafParser::ExprContext *ctx) {
    visitChildren(ctx);
    
    // -
    if (ctx->Minus()) 
    {
        code_ << pop
            << "\tsub a0, x0, t0\n"
            << push;
        return retType::INT;
    } 

    // !
    else if(ctx->Exclamation()) {
        code_ << pop
            << "\tseqz a0, t0\n"
            << push; 
        return retType::INT;
    } 
    
    // ~
    else if(ctx->Tilde()) {
        code_ << pop
            << "\tnot a0, t0\n"
            << push;
        return retType::INT;
    }
    return retType::UNDEF;
}