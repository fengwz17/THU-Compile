#include "CodeEmission.h"

// Visiting AST from the root node
// return: Generated asm code
antlrcpp::Any CodeEmission::visitProg(MiniDecafParser::ProgContext *ctx, symTab& symbol_) {
    varTable = symbol_;
    label = 0;
    code_ << ".section .text\n"
        << ".globl main\n";
    visitChildren(ctx);
    return code_.str();
}

// Visit Func node, no parameters
antlrcpp::Any CodeEmission::visitFunc(MiniDecafParser::FuncContext *ctx) {
    funcName = ctx->Identifier()->getText();
    retState = true;

    // Calling convention: saving ra(return address), caller fp and allocating stack memory for local variable
    code_ << funcName << ":\n"
          << "\tsw ra, -4(sp)\n"
          << "\taddi sp, sp, -4\n"
          << "\tsw fp, -4(sp)\n"
          << "\taddi fp, sp, -4\n"
          << "\taddi sp, fp, ";

    int capacity = varTable[funcName].size();
    code_ << -4 * capacity << "\n";

    visitChildren(ctx);

    // Directly return in func 
    if (retState) 
    {
        code_ << "\tli a0, 0\n"
              << ret;
    }
    return retType::INT;
}


// Visit ReturnStmt node, son of Stmt node
antlrcpp::Any CodeEmission::visitRetStmt(MiniDecafParser::RetStmtContext *ctx) {
    visitChildren(ctx);
    code_ << ret;
    retState = false;
    return retType::UNDEF;
}

// Visit Expr node, which is a single Integer node in this step
// return: Expr type
antlrcpp::Any CodeEmission::visitInteger(MiniDecafParser::IntegerContext *ctx) {
    std::string strLiteral = ctx->Integer()->getText();
    long long numLiteral = std::stoll(strLiteral);
    if (numLiteral > INT32_MAX) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": "
                  << "[ERROR] Constant out of INT range.\n";
        exit(1);
    }
    
    code_ << "\tli a0, " << strLiteral << "\n"
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
    else if(ctx->Exclamation()) 
    {
        code_ << pop
            << "\tseqz a0, t0\n"
            << push; 
        return retType::INT;
    } 
    
    // ~
    else if(ctx->Tilde()) 
    {
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


// read var from stack
antlrcpp::Any CodeEmission::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string var = ctx->Identifier()->getText();
    code_ << "\tlw a0, " << -4 - 4 * varTable[funcName][var] << "(fp)\n" << push;
    return retType::INT;
}
     
// variable definition
antlrcpp::Any CodeEmission::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    if (ctx->expr())
    {
        visit(ctx->expr());
        code_ << "\tsw a0, " << -4 - 4 * varTable[funcName][varName] << "(fp)\n";
    }
    return retType::INT;
}
   
// assign
// find varName in stack and store varTable[varName]
antlrcpp::Any CodeEmission::visitAssign(MiniDecafParser::AssignContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    visit(ctx->expr());
    code_ << "\tsw a0, " << -4 - 4 * varTable[funcName][varName] << "(fp)\n";
    return retType::INT;
}

// if else
antlrcpp::Any CodeEmission::visitIfElse(MiniDecafParser::IfElseContext *ctx) {
    visit(ctx->expr());
    
    // only if
    if (!(ctx->Else())) 
    {
        int brEnd = label++;
        code_ << "\tbeqz a0, .L" << brEnd << "\n";
        visit(ctx->stmt(0));
        code_ << ".L" << brEnd << ":\n";
    }

    // if-else
    else
    {
        int brElse = label++;
        int brEnd = label++;
        code_ << "\tbeqz a0, .L" << brElse << "\n";
        visit(ctx->stmt(0));
        code_ << "\tj .L" << brEnd << "\n";
        code_ << ".L" << brElse << ":\n";
        visit(ctx->stmt(1));
        code_ << ".L" << brEnd << ":\n";
    } 
    
    return retType::UNDEF;
}

antlrcpp::Any CodeEmission::visitCondExpr(MiniDecafParser::CondExprContext *ctx) {
    visit(ctx->expr(0));
    int brElse = label++;
    int brEnd = label++;
    code_ << "\tbeqz a0, .L_" << brElse << "\n";
    visit(ctx->expr(1));
    code_ << "\tj .L_" << brEnd << "\n";
    code_ << ".L_" << brElse << ":\n";
    visit(ctx->expr(2));
    code_ << ".L_" << brEnd << ":\n";
    return retType::UNDEF;
}



