#include "CodeEmission.h"

// Visiting AST from the root node
// return: Generated asm code
antlrcpp::Any CodeEmission::visitProg(MiniDecafParser::ProgContext *ctx, symTab& symbol_, varTab& varID) {
    varTable = symbol_;
    label = 0;
    code_ << ".section .text\n"
        << ".globl main\n";
    visitChildren(ctx);
    return code_.str();
}

antlrcpp::Any CodeEmission::visitFunc(MiniDecafParser::FuncContext *ctx) {

    if (ctx->Semicolon()) 
    {
        return retType::INT;
    }
    else
    {
        curFunc = ctx->Identifier(0)->getText();
        retState = true;
        blockID = 0;
        stmtID = 0;
    
        code_ << curFunc << ":\n"
            << "\tsw ra, -4(sp)\n"
            << "\taddi sp, sp, -4\n"
            << "\tsw fp, -4(sp)\n"
            << "\taddi fp, sp, -4\n"
            << "\taddi sp, fp, ";

        int capacity = -1;
        for (auto& var : varTable) 
        {
            if (var.first.substr(0, curFunc.length()) == curFunc) 
            {
                capacity += varTable[var.first].size();
            }
        }
        code_ << -4 * capacity << "\n";

        for (int i = 1; i < ctx->Identifier().size(); ++i) 
        {
            if (ctx->Identifier().size() > 8) 
            {
                code_ << "\tlw t0, " << 8 + 4 * varTable[curFunc][ctx->Identifier(i)->getText()] << "(fp)\n";
                code_ << "\tsw t0, " << -4 - 4 * varTable[curFunc][ctx->Identifier(i)->getText()] << "(fp)\n";
            } 
            else 
            {
                code_ << "\tsw a" << i-1 << ", " << -4 - 4 * varTable[curFunc][ctx->Identifier(i)->getText()] << "(fp)\n";
            }
        } 
        visitChildren(ctx);

        // Dealing with no return in main()
        if (retState) 
        {
            code_ << "\tli a0, 0\n"
                  << ret;
        }
    }
    return retType::INT;
}

//@brief: Visit FuncCall node
antlrcpp::Any CodeEmission::visitFuncCall(MiniDecafParser::FuncCallContext *ctx) {
    // Before calling a function, we should compute the arguments and pass them using stack or register
    if (ctx->expr().size() > 7) 
    { 
        for (int i = ctx->expr().size()-1; i >= 0; --i) 
        {
            visit(ctx->expr(i));
        }
    } 
    else 
    {
        for (int i = ctx->expr().size() - 1; i >= 0; --i) 
        {
            visit(ctx->expr(i));
            code_ << "\tmv a" << i << ", a0\n";
        }
    }
    code_ << "\tcall " << ctx->Identifier()->getText() << "\n"
        << "\taddi sp, sp, " << 4 + 4 * ctx->expr().size() << "\n"
        << push;
    return retType::UNDEF;
}

antlrcpp::Any CodeEmission::visitBlock(MiniDecafParser::BlockContext *ctx) {
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

// variable definition
antlrcpp::Any CodeEmission::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    varID[varName]++;

    if (ctx->expr())
    {
        visit(ctx->expr());
        code_ << "\tsw a0, " << -4 - 4 * varTable[curFunc][varName] << "(fp)\n";
    }
    return retType::INT;
}

// read var from stack
antlrcpp::Any CodeEmission::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::string tmpFunc = curFunc;
    
    int tmpStmt = stmtID;

    if (varTable[curFunc].count(varName + "#") > 0) 
    {
        if (varID[varName] < varTable[curFunc][varName + "#"]) 
        {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            tmpStmt--;
        }
    }
    
    for (int i = 0; i <= tmpStmt; i++) 
    {   
        if (varTable[tmpFunc].count(varName) == 0) 
        {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            continue;
        }

        // std::cout << "tmp varTable: " << tmpFunc << " " << varName << " " << varTable[tmpFunc][varName] << '\n';
        code_ << "\tlw a0, " << -4 - 4 * varTable[tmpFunc][varName] << "(fp)\n"
              << push;
        // std::cout << "tmp varTable: " << tmpFunc << " " << varName << " " << varTable[tmpFunc][varName] << '\n';
        return retType::INT;
    }
   
    return retType::INT;
}

// assign
// find varName in stack and store varTable[varName]
antlrcpp::Any CodeEmission::visitAssign(MiniDecafParser::AssignContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    varID[varName]++;
    visit(ctx->expr());
    std::string tmpFunc = curFunc;
    int tmpStmt = stmtID;
    if (varTable[curFunc].count(varName + "#") > 0) 
    {
        if (varID[varName] < varTable[curFunc][varName + "#"]) 
        {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            tmpStmt--;
        }
    }
    for (int i = 0; i <= tmpStmt; i++) 
    {
        if (varTable[tmpFunc].count(varName) == 0) 
        {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            continue;
        }
        code_ << "\tsw a0, " << -4 - 4 * varTable[tmpFunc][varName] << "(fp)\n";
        return retType::INT;
    }
    // Load the value from stack
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

    code_ << "\tbeqz a0, .L" << brElse << "\n";

    visit(ctx->expr(1));

    code_ << "\tj .L" << brEnd << "\n";
    code_ << ".L" << brElse << ":\n";
    
    visit(ctx->expr(2));
    code_ << ".L" << brEnd << ":\n";
    return retType::UNDEF;
}

// for 
antlrcpp::Any CodeEmission::visitForLoop(MiniDecafParser::ForLoopContext *ctx) {
    stmtID++;
    curFunc += "_" + std::to_string(blockID) + std::to_string(stmtID);
    int startBranch = label++;
    int endBranch = label++;
    int continueBranch = label++;

    breakTarget.push_back(endBranch);
    continueTarget.push_back(continueBranch);

    int exprNum = -1;

    // for (condition) <stmt>
    // condition: (<decl | expr>; <expr>; <expr> )

    if (ctx->declaration()) 
    {
        visit(ctx->declaration());
    } 
    else if (ctx->expr(0)) 
    {
        visit(ctx->expr(0));
        exprNum = 0;
    }

    code_ << ".L" << startBranch << ":\n";
    if (ctx->expr(exprNum + 1)) 
    {
        visit(ctx->expr(exprNum + 1));
        code_ << "\tbeqz a0, .L" << endBranch << "\n";
    } 
    visit(ctx->stmt());
    code_ << ".L" << continueBranch << ":\n";

    if (ctx->expr(exprNum + 2)) 
    {
        visit(ctx->expr(exprNum + 2));
    }
    code_ << "\tj .L" << startBranch << "\n"
            << ".L" << endBranch << ":\n";
    breakTarget.pop_back();
    continueTarget.pop_back();

    if (--stmtID == 0) 
    {
        ++blockID;
    }
    int pos = curFunc.rfind('_');
    curFunc = curFunc.substr(0, pos);
    return retType::UNDEF;
}

// while
antlrcpp::Any CodeEmission::visitWhileLoop(MiniDecafParser::WhileLoopContext *ctx) {
    int startBranch = label++;
    int endBranch = label++;

    breakTarget.push_back(endBranch);
    continueTarget.push_back(startBranch);

    code_ << ".L" << startBranch << ":\n";
    visit(ctx->expr());
    code_ << "\tbeqz a0, .L" << endBranch << "\n";
    visit(ctx->stmt());
    code_ << "\tj .L" << startBranch << "\n"
            << ".L" << endBranch << ":\n";

    breakTarget.pop_back();
    continueTarget.pop_back();
    return retType::UNDEF;
}

// do while
antlrcpp::Any CodeEmission::visitDoWhile(MiniDecafParser::DoWhileContext *ctx) {
    
    int startBranch = label++;
    int endBranch = label++;
    breakTarget.push_back(endBranch);
    continueTarget.push_back(startBranch);

    code_ << ".L" << startBranch << ":\n";
    visit(ctx->stmt());
    visit(ctx->expr());
    code_ << "\tbnez a0, .L" << startBranch << "\n";
    code_ << ".L" << endBranch << ":\n";

    breakTarget.pop_back();
    continueTarget.pop_back();
    return retType::UNDEF;
}

// break
antlrcpp::Any CodeEmission::visitBreak(MiniDecafParser::BreakContext *ctx) {
    code_ << "\tj .L" << breakTarget.back() << "\n";
    return retType::UNDEF;
}

// continue
antlrcpp::Any CodeEmission::visitContinue(MiniDecafParser::ContinueContext *ctx) {
    code_ << "\tj .L" << continueTarget.back() << "\n";
    return retType::UNDEF; 
}
