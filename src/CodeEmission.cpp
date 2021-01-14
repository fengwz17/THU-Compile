#include "CodeEmission.h"
#include "utils.h"

// Visiting AST from the root node
// return: Generated asm code
antlrcpp::Any CodeEmission::visitProg(MiniDecafParser::ProgContext *ctx){ //, varTab& varID) {
    // varTable = symbol_;
    // std::cout << "ddd" << std::endl;
    label = 0;
    code_ << ".section .text\n"
        << ".globl main\n";
    data_ << ".section .data\n";
    bss_ << ".section .bss\n";
    visitChildren(ctx);
    std::string result = code_.str() + data_.str() + bss_.str();  
    return result;
}

antlrcpp::Any CodeEmission::visitFunc(MiniDecafParser::FuncContext *ctx) {

    if (!ctx->Semicolon())
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

        int capacity = 0;
        // for (auto& var : varTable) 
        // {
        //     if (var.first.substr(0, curFunc.length()) == curFunc) 
        //     {
        //         capacity += varTable[var.first].size();
        //     }
        // }
        for (auto& var : GlobStruct::getInstance().symbolTable) 
        {
            if (var.first.substr(0, curFunc.length()) == curFunc) 
            {
                capacity += GlobStruct::getInstance().symbolTable[var.first].size();
            }
        }

        code_ << -4 * capacity << "\n";

        for (int i = 1; i < ctx->Identifier().size(); ++i) 
        {
            if (ctx->Identifier().size() > 8) 
            {
                code_ << "\tlw t0, " << 
                    8 + 4 * GlobStruct::getInstance().symbolTable[curFunc][ctx->Identifier(i)->getText()].get()->getOffset() << "(fp)\n";
                code_ << "\tsw t0, " << 
                    -4 - 4 * GlobStruct::getInstance().symbolTable[curFunc][ctx->Identifier(i)->getText()].get()->getOffset() << "(fp)\n";
            } 
            else 
            {
                code_ << "\tsw a" << i-1 << ", " 
                    << -4 - 4 * GlobStruct::getInstance().symbolTable[curFunc][ctx->Identifier(i)->getText()].get()->getOffset() << "(fp)\n";
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
    return retType::UNDEF;
}

//@brief: Visit FuncCall node
antlrcpp::Any CodeEmission::visitFuncCall(MiniDecafParser::FuncCallContext *ctx) {
    // Before calling a function, we should compute the arguments and pass them using stack or register
    if (ctx->expr().size() >= 8) 
    { 
        for (int i = ctx->expr().size()-1; i >= 0; --i) 
        {
            // visit(ctx->expr(i));
            retType type = visit(ctx->expr(i));
            if (type == retType::LEFT) 
            {
                code_ << popReg("t0")
                      << "\tlw t0, (t0)\n"
                      << pushReg("t0");
            }
        }
    } 
    else 
    {
        for (int i = ctx->expr().size() - 1; i >= 0; --i) 
        {
            // visit(ctx->expr(i));
            // code_ << "\tmv a" << i << ", a0\n";
            retType type = visit(ctx->expr(i));
            if (type == retType::LEFT) {
                code_ << popReg("a0")
                      << "\tlw a0, (a0)\n";
            }
            code_ << "\tmv a" << i << ", a0\n";
        }
    }
    code_ << "\tcall " << ctx->Identifier()->getText() << "\n"
        << "\taddi sp, sp, " << 4 + 4 * ctx->expr().size() << "\n"
        << pushReg("a0");
    return retType::RIGHT;
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
    retType type = visit(ctx->expr());
    std::string retTypeStr = ctx->expr()->getStart()->getText();
    if (retTypeStr == "&")
    {
        std::cerr << "line " << ctx->start->getLine() << ": "
                  << "[ERROR] Bad return type.\n";
        exit(1);
    }
    code_ << popReg("a0");
    // std::cout << "dddddd" << std::endl;
    if (type == retType::LEFT) 
    {
        code_ << "\tlw a0, (a0)\n";
    }
    code_ << ret;
    retState = false;
    return retType::UNDEF;
}

// Visit Expr node, which is a single Integer node in this step
// return: Expr type
antlrcpp::Any CodeEmission::visitInteger(MiniDecafParser::IntegerContext *ctx) {
    std::string strLiteral = ctx->getText();
    long long numLiteral = std::stoll(strLiteral);
    if (numLiteral > INT32_MAX) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": "
                  << "[ERROR] Constant out of INT range.\n";
        exit(1);
    }
    
    code_ << "\tli a0, " << strLiteral << "\n"
          << pushReg("a0");
    // std::cout << strLiteral << std::endl;
    return retType::RIGHT;
}

antlrcpp::Any CodeEmission::visitUnaryOp(MiniDecafParser::UnaryOpContext *ctx) {
    retType UnaryType = visit(ctx->unary());
    code_ << popReg("t0");

    if (ctx->AND()) 
    {
        code_ << pushReg("t0");
        return retType::RIGHT;
    }
    if (UnaryType == retType::LEFT) 
    {
        code_ << "\tlw t0, (t0)\n";
    }

    // -
    if (ctx->Minus()) 
    {
        code_ << "\tsub a0, x0, t0\n"
            << pushReg("a0");
        return retType::RIGHT;
    } 

    // !
    else if(ctx->Exclamation()) 
    {
        code_ << "\tseqz a0, t0\n"
            << pushReg("a0"); 
        return retType::RIGHT;
    } 
    
    // ~
    else if(ctx->Tilde()) 
    {
        code_ << "\tnot a0, t0\n"
            << pushReg("a0");
        return retType::RIGHT;
    }
    
    else if(ctx->Multiplication()) 
    {
        code_ << pushReg("t0");
        return retType::LEFT;
    }

    return retType::UNDEF;
}

// ( expr )
antlrcpp::Any CodeEmission::visitParen(MiniDecafParser::ParenContext *ctx) {

    return visit(ctx->expr());
}

// + -
antlrcpp::Any CodeEmission::visitAddSub(MiniDecafParser::AddSubContext *ctx) {
    retType Expr1 = visit(ctx->add(0));
    retType Expr2 = visit(ctx->add(1));
  
    code_ << popReg("t0", "t1");
    if (Expr1 == retType::LEFT) 
    {
        code_ << "\tlw t0, (t0)\n";
    }
    if (Expr2 == retType::LEFT) 
    {
        code_ << "\tlw t1, (t1)\n";
    }
    if (ctx->Addition()) 
    {
        code_ << "\tadd a0, t0, t1\n"
              << pushReg("a0");
        return retType::RIGHT;
    } 
    else if (ctx->Minus()) 
    {
        code_ << "\tsub a0, t0, t1\n"
              << pushReg("a0");
        return retType::RIGHT;
    }
    return retType::UNDEF;
}



// * / %
antlrcpp::Any CodeEmission::visitMulDiv(MiniDecafParser::MulDivContext *ctx) {
    retType Expr1 = visit(ctx->mul(0));
    retType Expr2 = visit(ctx->mul(1));

    code_ << popReg("t0", "t1");
    if (Expr1 == retType::LEFT) 
    {
        code_ << "\tlw t0, (t0)\n";
    }
    if (Expr2 == retType::LEFT) 
    {
        code_ << "\tlw t1, (t1)\n";
    }


    if (ctx->Multiplication())
    {
        code_ << "\tmul a0, t0, t1\n" << pushReg("a0");
        return retType::RIGHT;
    }
    else if (ctx->Division())
    {
        code_ << "\tdiv a0, t0, t1\n" << pushReg("a0");
        return retType::RIGHT;
    }
    else if (ctx->Modulo())
    {
        code_ << "\trem a0, t0, t1\n" << pushReg("a0");
        return retType::RIGHT;
    }
    return retType::UNDEF;
}

// equal node
antlrcpp::Any CodeEmission::visitEqual(MiniDecafParser::EqualContext *ctx) {
    retType Expr1 = visit(ctx->equality(0));
    retType Expr2 = visit(ctx->equality(1));
    
    code_ << popReg("t0", "t1");

    if (Expr1 == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (Expr2 == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }
    code_ << "\tsub t0, t0, t1\n";

    if (ctx->EQ()) 
    {
        code_ << "\tseqz a0, t0\n";    
    }
    else if (ctx->NEQ()) 
    {
        code_ << "\tsnez a0, t0\n";
    }
    code_ << pushReg("a0");
    return retType::RIGHT;
}

// compare node
antlrcpp::Any CodeEmission::visitCompare(MiniDecafParser::CompareContext *ctx) {
    retType Expr1 = visit(ctx->relational(0));
    retType Expr2 = visit(ctx->relational(1));

    code_ << popReg("t0", "t1");
    if (Expr1 == retType::LEFT) 
    {
        code_ << "\tlw t0, (t0)\n";
    }
    if (Expr2 == retType::LEFT) 
    {
        code_ << "\tlw t1, (t1)\n";
    }

    if (ctx->LE()) 
    {
        code_ << "\tsgt a0, t0, t1\n"
              << "\txori a0, a0, 1\n";
    } 
    else if (ctx->LT()) 
    {
        code_ << "\tslt a0, t0, t1\n";
    } 
    else if (ctx->GE()) 
    {
        code_ << "\tslt a0, t0, t1\n"
              << "\txori a0, a0, 1\n";
    } 
    else if (ctx->GT()) 
    {
        code_ << "\tsgt a0, t0, t1\n";
    }
    code_ << pushReg("a0");
    return retType::RIGHT;
}



// &&
antlrcpp::Any CodeEmission::visitLand(MiniDecafParser::LandContext *ctx) {
    retType Expr1 = visit(ctx->logical_and(0));
    retType Expr2 = visit(ctx->logical_and(1));
    
    code_ << popReg("t0", "t1");

    if (Expr1 == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (Expr2 == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }

    code_ << "\tsnez t0, t0\n"
          << "\tsnez t1, t1\n"
          << "\tand a0, t0, t1\n"
          << pushReg("a0");

    return retType::RIGHT;
}

// ||
antlrcpp::Any CodeEmission::visitLor(MiniDecafParser::LorContext *ctx) {
    retType Expr1 = visit(ctx->logical_or(0));
    retType Expr2 = visit(ctx->logical_or(1));
    
    code_ << popReg("t0", "t1");
    if (Expr1 == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (Expr2 == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }

    code_ << "\tor a0, t0, t1\n"
          << "\tsnez a0, a0\n"
          << pushReg("a0");
    return retType::RIGHT;
}


antlrcpp::Any CodeEmission::visitGlobal(MiniDecafParser::GlobalContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    if (ctx->Integer()) 
    {
        data_ << ".globl " << varName << "\n"
              << ".align 4\n"
              << ".size " << varName << ", 4\n"
              << varName << ":\n" 
              << "\t.word " << ctx->Integer()->getText() << "\n";
    }
    else 
    {
        bss_ << ".globl " << varName << "\n"
             << varName << ":\n" 
             << ".space 4\n";
    }
    return retType::UNDEF;
}

// variable definition
antlrcpp::Any CodeEmission::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    // varID[varName]++;

    if (ctx->expr())
    {
        retType type = visit(ctx->expr());
        // Store the rvalue to the address of current variable
        code_ << popReg("a0");
        if (type == retType::LEFT) 
        {
            code_ << "\tlw a0, (a0)\n";
        }
        code_ << "\tsw a0, " << -4 - 4 * GlobStruct::getInstance().symbolTable[curFunc][varName].get()->getOffset() << "(fp)\n";
    }
    return retType::UNDEF;
}

// read var from stack
antlrcpp::Any CodeEmission::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::string tmpFunc = curFunc;
    
    int tmpStmt = stmtID;
    int line = ctx->start->getLine();

    // if (varTable[curFunc].count(varName + "#") > 0) 
    // {
    //     if (varID[varName] < varTable[curFunc][varName + "#"]) 
    //     {
    //         int pos = tmpFunc.rfind('_');
    //         tmpFunc = tmpFunc.substr(0, pos);
    //         tmpStmt--;
    //     }
    // }
    
    for (int i = 0; i <= tmpStmt; i++) 
    {   
        if (GlobStruct::getInstance().symbolTable[tmpFunc].count(varName) == 0 || GlobStruct::getInstance().symbolTable[tmpFunc][varName].get()->getLineNum() > line) 
        {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            continue;
        }

        // std::cout << "tmp varTable: " << tmpFunc << " " << varName << " " << varTable[tmpFunc][varName] << '\n';
        code_ << "\taddi a0, fp, " << -4 - 4 * GlobStruct::getInstance().symbolTable[tmpFunc][varName].get()->getOffset() << "\n"
              << pushReg("a0");
        // std::cout << "tmp varTable: " << tmpFunc << " " << varName << " " << varTable[tmpFunc][varName] << '\n';
        return retType::LEFT;
    }

    if (GlobStruct::getInstance().symbolTable["globl"].count(varName) > 0) 
    {
        code_ << "\tla a0, " << varName << "\n"
              << pushReg("a0");
    }
   
    return retType::LEFT;
}

// assign
// find varName in stack and store varTable[varName]
antlrcpp::Any CodeEmission::visitAssign(MiniDecafParser::AssignContext *ctx) {
    // std::string varName = ctx->Identifier()->getText();
    // varID[varName]++;
    // visit(ctx->expr());
    // std::string tmpFunc = curFunc;
    // int tmpStmt = stmtID;
    // if (varTable[curFunc].count(varName + "#") > 0) 
    // {
    //     if (varID[varName] < varTable[curFunc][varName + "#"]) 
    //     {
    //         int pos = tmpFunc.rfind('_');
    //         tmpFunc = tmpFunc.substr(0, pos);
    //         tmpStmt--;
    //     }
    // }
    // for (int i = 0; i <= tmpStmt; i++) 
    // {
    //     if (varTable[tmpFunc].count(varName) == 0) 
    //     {
    //         int pos = tmpFunc.rfind('_');
    //         tmpFunc = tmpFunc.substr(0, pos);
    //         continue;
    //     }
    //     code_ << "\tsw a0, " << -4 - 4 * varTable[tmpFunc][varName] << "(fp)\n";
    //     return retType::INT;
    // }

    // if (varTable["globl"].count(varName) > 0) 
    // {
    //     code_ << "\tla t0, " << varName << "\n"
    //           << "\tsw a0, 0(t0)\n";
    // }

    // // Load the value from stack
    // return retType::INT;
    retType lType = visit(ctx->unary());
    retType rType = visit(ctx->expr());
    code_ << popReg("t1");
    if (rType == retType::LEFT) 
    {
        code_ << "\tlw t1, (t1)\n";
    } 
    code_ << popReg("t0")
          << "\tsw t1, (t0)\n"
          << pushReg("t0");
    return retType::LEFT;
}

// if else
antlrcpp::Any CodeEmission::visitIfElse(MiniDecafParser::IfElseContext *ctx) {

     
    std::string t = ctx->expr()->getStart()->getText();

    if (GlobStruct::getInstance().symbolTable[curFunc].count(t) > 0)
    {
        // std::cout << "type: " << GlobStruct::getInstance().symbolTable[curFunc][t]->getType()->getStarNum() << std::endl;
        if (GlobStruct::getInstance().symbolTable[curFunc][t]->getType()->typeCheckLiteral("Intptr") && t != "np")
        {
           
            std::cerr << "line " << ctx->start->getLine() << ": "
                    << "[ERROR] Condition type error in if condition.\n";
            exit(1);
        }
    }

    retType type = visit(ctx->expr());

    code_ << popReg("a0");

    if (type == retType::LEFT) {
        code_ << "\tlw a0, (a0)\n";
    }

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
    retType type = visit(ctx->logical_or());
    int brElse = label++;
    int brEnd = label++;

    code_ << popReg("a0");
    if (type == retType::LEFT) 
    {
        code_ << "\tlw a0, (a0)\n";
    }

    code_ << "\tbeqz a0, .L" << brElse << "\n";

    visit(ctx->expr());

    code_ << "\tj .L" << brEnd << "\n";
    code_ << ".L" << brElse << ":\n";
    
    visit(ctx->conditional());
    code_ << ".L" << brEnd << ":\n";
    return retType::UNDEF;
}

// for 
antlrcpp::Any CodeEmission::visitForLoop(MiniDecafParser::ForLoopContext *ctx) {
    stmtID++;
    std::string tmpFunc = curFunc;
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


        std::string t = ctx->expr(0)->getStart()->getText();
    
        if (!GlobStruct::getInstance().symbolTable[tmpFunc][t]->getType()->typeCheckLiteral("Int"))
        {
            std::cerr << "line " << ctx->start->getLine() << ": "
                    << "[ERROR] Loop condition type error in for-loop.\n";
            exit(1);
        }

        exprNum = 0;
    }

    code_ << ".L" << startBranch << ":\n";
    if (ctx->expr(exprNum + 1)) 
    {
        retType type = visit(ctx->expr(exprNum + 1));

        code_ << popReg("a0");
        if (type == retType::LEFT) 
        {
            code_ << "\tlw a0, (a0)\n";
        }

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

    std::string t = ctx->expr()->getStart()->getText();
    

    if (GlobStruct::getInstance().symbolTable[curFunc].count(t) > 0)
    {
        if (!GlobStruct::getInstance().symbolTable[curFunc][t]->getType()->typeCheckLiteral("Int"))
        {
            std::cerr << "line " << ctx->start->getLine() << ": "
                    << "[ERROR] Condition type error in while loop.\n";
            exit(1);
        }
    }
    

    code_ << ".L" << startBranch << ":\n";
    retType type = visit(ctx->expr());

    

    code_ << popReg("a0");

    if (type == retType::LEFT) {
        code_ << "\tlw a0, (a0)\n";
    }

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
    retType type = visit(ctx->expr()); 

    std::string t = ctx->expr()->getStart()->getText();

    if (!GlobStruct::getInstance().symbolTable[curFunc][t]->getType()->typeCheckLiteral("Int"))
    {
        std::cerr << "line " << ctx->start->getLine() << ": "
                  << "[ERROR] Loop condition type error in do-while.\n";
        exit(1);
    }
    
    code_ << popReg("a0");
    if (type == retType::LEFT) {
        code_ << "\tlw a0, (a0)\n";
    }

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

antlrcpp::Any CodeEmission::visitExprStmt(MiniDecafParser::ExprStmtContext *ctx) {
    if (ctx->expr()) {
        visit(ctx->expr());
        code_ << popReg("t0");
    }
    return retType::UNDEF;
}

antlrcpp::Any CodeEmission::visitCast(MiniDecafParser::CastContext *ctx) {
    retType type = visit(ctx->unary());
    return type;
}

std::string CodeEmission::pushReg(std::string reg) 
{
    return "\taddi sp, sp, -4\n\tsw " + reg + ", 0(sp)\n";
}

std::string CodeEmission::popReg(std::string reg) 
{
    return "\tlw " + reg + ", 0(sp)\n\taddi sp, sp, 4\n";
}

std::string CodeEmission::popReg(std::string reg0, std::string reg1) 
{
    return "\tlw " + reg0 + ", 4(sp)\n\tlw " + reg1 + ", 0(sp)\n\taddi sp, sp, 8\n";
}
