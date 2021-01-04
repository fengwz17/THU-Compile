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
antlrcpp::Any CodeEmission::visitRetStmt(MiniDecafParser::RetStmtContext *ctx) {
    visitChildren(ctx);
    
    code_ << "\tret";
    return retType::UNDEF;
}

// Visit Expr node, which is a single Integer node in this step
// return: Expr type
antlrcpp::Any CodeEmission::visitInteger(MiniDecafParser::IntegerContext *ctx) {
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

antlrcpp::Any CodeEmission::visitUnary(MiniDecafParser::UnaryContext *ctx) {
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

// ( expr )
antlrcpp::Any CodeEmission::visitParen(MiniDecafParser::ParenContext *ctx) {

    return visit(ctx->expr());
}

// + -
antlrcpp::Any CodeEmission::visitAddSub(MiniDecafParser::AddSubContext *ctx) {
    retType Expr1 = visit(ctx->expr(0));
    retType Expr2 = visit(ctx->expr(1));
    if (ctx->Addition())
    {
        code_ << pop2;
        code_ << "\tadd a0, t0, t1\n" << push;
        return retType::INT;
    }
    else if (ctx->Minus())
    {
        code_ << pop2;
        code_ << "\tsub a0, t0, t1\n" << push;
        return retType::INT;
    }
    return retType::UNDEF;
}

// * / %
antlrcpp::Any CodeEmission::visitMulDiv(MiniDecafParser::MulDivContext *ctx) {
    retType Expr1 = visit(ctx->expr(0));
    retType Expr2 = visit(ctx->expr(1));
    if (ctx->Multiplication())
    {
        code_ << pop2;
        code_ << "\tmul a0, t0, t1\n" << push;
        return retType::INT;
    }
    else if (ctx->Division())
    {
        code_ << pop2;
        code_ << "\tdiv a0, t0, t1\n" << push;
        return retType::INT;
    }
    else if (ctx->Modulo())
    {
        code_ << pop2;
        code_ << "\trem a0, t0, t1\n" << push;
        return retType::INT;
    }
    return retType::UNDEF;
}

// compare node
antlrcpp::Any CodeEmission::visitCompare(MiniDecafParser::CompareContext *ctx) {
    retType Expr1 = visit(ctx->expr(0));
    retType Expr2 = visit(ctx->expr(1));

    if (ctx->LE()) 
    {
        code_ << pop2;
        code_ << "\tsgt a0, t0, t1\n"
              << "\txori a0, a0, 1\n";
    } 
    else if (ctx->LT()) 
    {
        code_ << pop2;
        code_ << "\tslt a0, t0, t1\n";
    } 
    else if (ctx->GE()) 
    {
        code_ << pop2;
        code_ << "\tslt a0, t0, t1\n"
              << "\txori a0, a0, 1\n";
    } 
    else if (ctx->GT()) 
    {
        code_ << pop2;
        code_ << "\tsgt a0, t0, t1\n";
    }
    code_ << push;
    return retType::INT;
}

// equal node
antlrcpp::Any CodeEmission::visitEqual(MiniDecafParser::EqualContext *ctx) {
    retType Expr1 = visit(ctx->expr(0));
    retType Expr2 = visit(ctx->expr(1));
    
    code_ << pop2 << "\tsub t0, t0, t1\n";
    
    if (ctx->EQ()) 
    {
        code_ << "\tseqz a0, t0\n";    
    }
    else if (ctx->NEQ()) 
    {
        code_ << "\tsnez a0, t0\n";
    }
    code_ << push;
    return retType::INT;
}

// &&
antlrcpp::Any CodeEmission::visitLand(MiniDecafParser::LandContext *ctx) {
    retType Expr1 = visit(ctx->expr(0));
    retType Expr2 = visit(ctx->expr(1));
    
    code_ << pop2
          << "\tsnez t0, t0\n"
          << "\tsnez t1, t1\n"
          << "\tand a0, t0, t1\n"
          << push;

    return retType::INT;
}

// ||
antlrcpp::Any CodeEmission::visitLor(MiniDecafParser::LorContext *ctx) {
    retType Expr1 = visit(ctx->expr(0));
    retType Expr2 = visit(ctx->expr(1));
    
    code_ << pop2
          << "\tor a0, t0, t1\n"
          << "\tsnez a0, a0\n"
          << push;
    return retType::INT;
}
