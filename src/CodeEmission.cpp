#include "CodeEmission.h"
#include "utils.h"

// @brief: Start visiting the syntax tree from root node Prog
// @ret: Generated asm code
antlrcpp::Any CodeEmission::visitProg(MiniDecafParser::ProgContext *ctx) {
    label = 0; 
    code_ << ".section .text\n"
        << ".globl main\n";
    data_ << ".section .data\n";
    bss_ << ".section .bss\n";
    visitChildren(ctx);
    return code_.str() + data_.str() + bss_.str();
}


antlrcpp::Any CodeEmission::visitFunc(MiniDecafParser::FuncContext *ctx) {

    GlobStruct& instance = GlobStruct::getInstance();
    if (!ctx->Semicolon()) 
    {
        curFunc = ctx->Identifier(0)->getText();
        retState = false;
        stmtID = 0; blockID = 0;
        // Calling convention: saving ra(return address), caller fp and allocating stack memory for local variable
        code_ << curFunc << ":\n"
            << "\tsw ra, -4(sp)\n"
            << "\taddi sp, sp, -4\n"
            << "\tsw fp, -4(sp)\n"
            << "\taddi fp, sp, -4\n"
            << "\taddi sp, fp, ";

        int capacity = 0;
        for (auto& func : instance.symbolTable) {
            if (func.first.substr(0, curFunc.length()) == curFunc) {
                for (auto& var : func.second) {
                    capacity += var.second.get()->getVolume();
                }
            }
        }
        code_ << -4 * capacity << "\n";

        // Calling convection: How to pass funcion args
        // When the number of arguments is greater than 7, we use stack to pass them, otherwise we use register a0-a6 instead.
        for (int i = 1; i < ctx->Identifier().size(); ++i) {
            if (ctx->Identifier().size() > 8) {
                code_ << "\tlw t0, " << 8 + 4 * instance.symbolTable[curFunc][ctx->Identifier(i)->getText()].get()->getOffset() << "(fp)\n";
                code_ << "\tsw t0, " << -4 - 4 * instance.symbolTable[curFunc][ctx->Identifier(i)->getText()].get()->getOffset() << "(fp)\n";
            } else {
                code_ << "\tsw a" << i-1 << ", " << -4 - 4 * instance.symbolTable[curFunc][ctx->Identifier(i)->getText()].get()->getOffset() << "(fp)\n";
            }
        } 
        visitChildren(ctx);

        // Dealing with no return in main()
        if (!retState) {
            code_ << "\tli a0, 0\n"
                << ret;
        }
    }
    return retType::UNDEF;
}

//@brief: Visit FuncCall node
antlrcpp::Any CodeEmission::visitFuncCall(MiniDecafParser::FuncCallContext *ctx) {
    // Before calling a function, we should compute the arguments and pass them using stack or register
    if (ctx->expr().size() > 7) { 
        for (int i = ctx->expr().size()-1; i >= 0; --i) {
            retType type = visit(ctx->expr(i));
            if (type == retType::LEFT) {
                code_ << popReg("t0")
                      << "\tlw t0, (t0)\n"
                      << pushReg("t0");
            }
        }
    } else {
        for (int i = ctx->expr().size()-1; i >= 0; --i) {
            retType type = visit(ctx->expr(i));
            code_ << popReg("t0");
            if (type == retType::LEFT) {
                code_ << "\tlw t0, (t0)\n"
                      << pushReg("t0");
            }
            code_ << "\tmv a" << i << ", t0\n"
                  << pushReg("t0");
        }
    }
    code_ << "\tcall " << ctx->Identifier()->getText() << "\n"
        << "\taddi sp, sp, " << 4 + 4 * ctx->expr().size() << "\n"
        << pushReg("a0");
    return retType::RIGHT;
}

//@brief: Visit BlockStmt node, update the scope information
antlrcpp::Any CodeEmission::visitBlock(MiniDecafParser::BlockContext *ctx) {
    // When entering a new scope, update block depth
    ++stmtID;
    curFunc += "_" + std::to_string(blockID) + std::to_string(stmtID);
    for (auto item : ctx->blockItem()) {
        visit(item);
    }
    // When exiting a scope, update the block depth & block order(only top level)
    if (--stmtID == 0) {
        ++blockID;
    }
    int pos = curFunc.rfind('_');
    curFunc = curFunc.substr(0, pos);
    return retType::UNDEF;
}

// @brief: Visit ReturnStmt node, son of Stmt node
antlrcpp::Any CodeEmission::visitRetStmt(MiniDecafParser::RetStmtContext *ctx) {
    code_ << "\t#return\n";
    retType type = visit(ctx->expr());

    code_ << popReg("a0");
    if (type == retType::LEFT) {
        code_ << "\tlw a0, (a0)\n";
    }
    code_ << "\taddi sp, fp, 4\n"
        << "\tlw ra, (sp)\n" 
        << "\tlw fp, -4(sp)\n"
        << "\tret\n";
    retState = true;
    return retType::UNDEF;
}

// @brief: Visit Integer node, which loads an immediate number into register
// @ret: Expr type: Int
antlrcpp::Any CodeEmission::visitInteger(MiniDecafParser::IntegerContext *ctx) {
    code_ << "\tli a0, " << ctx->getText() << "\n"
          << pushReg("a0");
    return retType::RIGHT; 
}

// @brief: Visit unaryOp node, like '-1', '~12', '!89' and etc. 
// @ret: Expr type: Int
antlrcpp::Any CodeEmission::visitUnaryOp(MiniDecafParser::UnaryOpContext *ctx) {
    retType unaryType = visit(ctx->unary());
    code_ << popReg("t0");
    if (ctx->AND()) {
        code_ << pushReg("t0");
        return retType::RIGHT;
    }
    if (unaryType == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    /* 
        ctx-><token_name>() is a ptr point to the token
        ptr == nullptr indicates that the token is undeclared
    */
    if (ctx->Minus()) {
        code_ << "\tsub a0, x0, t0\n"
            << pushReg("a0");
        return retType::RIGHT;
    } else if(ctx->Exclamation()) {
        code_ << "\tseqz a0, t0\n"
            << pushReg("a0"); 
        return retType::RIGHT;
    } else if(ctx->Tilde()) {
        code_ << "\tnot a0, t0\n"
            << pushReg("a0");
        return retType::RIGHT;
    } else if(ctx->Multiplication()) {
        code_ << pushReg("t0");
        return retType::LEFT;
    }
    return retType::UNDEF;
}

//@brief: Visit Expr with parenthesis, like '3-(1+2)'
//@ret: Expr type: Inner expr
antlrcpp::Any CodeEmission::visitParen(MiniDecafParser::ParenContext *ctx) {
    /*
        Cause the parenthesis only specifies the priority of the expr,
        what we need to do is visit the inner Expr.
        While the parsing process of antlr4 defines the priority.
    */
    return visit(ctx->expr());
}

//@brief: visit AddSub node, including addition & subtraction between expressions
//@ret: Expr type: depending on the left & right expressions
antlrcpp::Any CodeEmission::visitAddSub(MiniDecafParser::AddSubContext *ctx) {
    // Get the type of left & right expressions
    retType lExpr = visit(ctx->add(0)), rExpr = visit(ctx->add(1));
    std::shared_ptr<Type> lType = GlobStruct::getInstance().typeQueue.front();
    GlobStruct::getInstance().typeQueue.pop();
    std::shared_ptr<Type> rType = GlobStruct::getInstance().typeQueue.front();
    GlobStruct::getInstance().typeQueue.pop();

    /*
        The use of token ptr is the same as FUNC<visitUnaryOp>.
        In this step, stack machine first pops the 2 operators at the top of stack,
        then computes the result, finally pushes the result to the stack.
    */
    code_ << popReg("t0", "t1");
    if (lExpr == retType::LEFT) 
    {
        code_ << "\tlw t0, (t0)\n";
    }
    if (rExpr == retType::LEFT) 
    {
        code_ << "\tlw t1, (t1)\n";
    }
    if (lType.get()->getType() == "Intptr" && rType.get()->getType() == "Int") 
    {
        code_ << "\tslli t1, t1, 2\n";
    } 
    else if (rType.get()->getType() == "Intptr" && lType.get()->getType() == "Int") 
    {
        code_ << "\tslli t0, t0, 2\n";
    }
    if (ctx->Addition()) 
    {
        code_ << "\tadd a0, t0, t1\n"
              << pushReg("a0");
        return retType::RIGHT;
    } 
    else if (ctx->Minus()) 
    {
        code_ << "\tsub a0, t0, t1\n";
        if (lType.get()->getType() == "Intptr" && rType.get()->getType() == "Intptr") 
        {
            code_ << "\tsrli a0, a0, 2\n";
        }
        code_ << pushReg("a0");
        return retType::RIGHT;
    }
    return retType::UNDEF;
}

//@brief: visit MulDiv node, including multiplication, division & modulo between expressions
//@ret: Expr type: depending on the left & right expressions
antlrcpp::Any CodeEmission::visitMulDiv(MiniDecafParser::MulDivContext *ctx) {
    
    
    // Get the type of left & right expressions
    retType lExpr = visit(ctx->mul(0)), rExpr = visit(ctx->mul(1));
    /*
        Totally the same as FUNC<visitAddSub>.
    */
    code_ << popReg("t0", "t1");
    if (lExpr == retType::LEFT) 
    {
        code_ << "\tlw t0, (t0)\n";
    }
    if (rExpr == retType::LEFT) 
    {
        code_ << "\tlw t1, (t1)\n";
    }
    if (ctx->Multiplication()) 
    {
        code_ << "\tmul a0, t0, t1\n"
              << pushReg("a0");
        return retType::RIGHT;
    } 
    else if (ctx->Division()) 
    {
        code_ << "\tdiv a0, t0, t1\n"
              << pushReg("a0");
        return retType::RIGHT;
    } 
    else if (ctx->Modulo()) 
    {
        code_ << "\trem a0, t0, t1\n"
              << pushReg("a0");
        return retType::RIGHT;
    }
    return retType::UNDEF;
}

//@brief: visit Equal node, including '==' & '!=' between expressions
//@ret: Expr type: INT
antlrcpp::Any CodeEmission::visitEqual(MiniDecafParser::EqualContext *ctx) {
    // Get the type of left & right expressions
    retType lExpr = visit(ctx->equality(0));
    retType rExpr = visit(ctx->equality(1));
    /*
        Nearly the same as FUNC<visitAddSub>
    */
    code_ << popReg("t0", "t1");
    if (lExpr == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (rExpr == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }
    code_ << "\tsub t0, t0, t1\n";
    if (ctx->EQ()) {
        code_ << "\tseqz a0, t0\n";
    } else if (ctx->NEQ()) {
        code_ << "\tsnez a0, t0\n";
    }
    code_ << pushReg("a0");
    return retType::RIGHT;
}

//@brief: visit LessGreat node, including '<=', '<', '>' & '>=' between expressions
//@ret: Expr type: INT
antlrcpp::Any CodeEmission::visitCompare(MiniDecafParser::CompareContext *ctx) {
    // Get the type of left & right expressions
    retType lExpr = visit(ctx->relational(0)), rExpr = visit(ctx->relational(1));
    /*
        Nearly the same as FUNC<visitAddSub>
    */
    code_ << popReg("t0", "t1");
    if (lExpr == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (rExpr == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }
    if (ctx->LE()) {
        code_ << "\tsgt a0, t0, t1\n"
              << "\txori a0, a0, 1\n";
    } else if (ctx->LT()) {
        code_ << "\tslt a0, t0, t1\n";
    } else if (ctx->GE()) {
        code_ << "\tslt a0, t0, t1\n"
              << "\txori a0, a0, 1\n";
    } else if (ctx->GT()) {
        code_ << "\tsgt a0, t0, t1\n";
    }
    code_ << pushReg("a0");
    return retType::RIGHT;
}

//@brief: visit Land node, including '&&' between expressions
//@ret: Expr type: INT
antlrcpp::Any CodeEmission::visitLand(MiniDecafParser::LandContext *ctx) {
    // Get the type of left & right expressions
    retType lExpr = visit(ctx->logical_and(0)), rExpr = visit(ctx->logical_and(1));
    /*
        Nearly the same as FUNC<visitAddSub>
    */
    code_ << popReg("t0", "t1");
    if (lExpr == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (rExpr == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }
    code_ << "\tmul a0, t0, t1\n"
          << "\tsnez a0, a0\n"
          << pushReg("a0");
    return retType::RIGHT;
}

//@brief: visit Lor node, including '||' between expressions
//@ret: Expr type: INT
antlrcpp::Any CodeEmission::visitLor(MiniDecafParser::LorContext *ctx) {
    // Get the type of left & right expressions
    retType lExpr = visit(ctx->logical_or(0)), rExpr = visit(ctx->logical_or(1));
    /*
        Nearly the same as FUNC<visitAddSub>
    */
    code_ << popReg("t0", "t1");
    if (lExpr == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (rExpr == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }
    code_ << "\tor a0, t0, t1\n"
          << pushReg("a0");
    return retType::RIGHT;
}

//@brief: visit GlobalArrDef node, global array is stored in .bss segment
antlrcpp::Any CodeEmission::visitGlobalArry(MiniDecafParser::GlobalArryContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::shared_ptr<Symbol> arr = GlobStruct::getInstance().symbolTable["globl"][varName];
    bss_ << ".global " << varName << "\n"
         << varName << ":\n"
         << "\t.space " << 4 * arr.get()->getVolume() << "\n";
    return retType::UNDEF;
}

antlrcpp::Any CodeEmission::visitLocalArry(MiniDecafParser::LocalArryContext *ctx) {
    return retType::UNDEF;
}

//@brief: visit GlobalVar node
antlrcpp::Any CodeEmission::visitGlobal(MiniDecafParser::GlobalContext *ctx) {
    /*
       Global varibles are stored in .bss or .data segments
       Initialized: .data; Uninitialized: .bss
       They can be accessed by their label
    */
    std::string varName = ctx->Identifier()->getText();
    if (ctx->Integer()) {
        data_ << ".globl " << varName << "\n"
              << varName << ":\n" << "\t.word " << ctx->Integer()->getText() << "\n";
    } else {
        bss_ << ".globl " << varName << "\n"
             << varName << ":\n" << ".space 4\n";
    }
    return retType::UNDEF;
}

//@brief: visit VarDef node, using rvalue to initialize the defined variable 
//@ret: Variable type
antlrcpp::Any CodeEmission::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    if (ctx->expr()) {
        retType type = visit(ctx->expr());
        // Store the rvalue to the address of current variable
        code_ << popReg("a0");
        if (type == retType::LEFT) {
            code_ << "\tlw a0, (a0)\n";
        }
        code_ << "\tsw a0, " << -4 - 4 * GlobStruct::getInstance().symbolTable[curFunc][varName].get()->getOffset() << "(fp)\n";
    }
    return retType::UNDEF;
}

//@brief: visit ArrayIndex node, the returned subscripted array is left value
antlrcpp::Any CodeEmission::visitArryIndex(MiniDecafParser::ArryIndexContext *ctx) {
    retType lType = visit(ctx->postfix());
    retType indexType = visit(ctx->expr());
    std::shared_ptr<Type> postType = GlobStruct::getInstance().typeQueue.front();
    std::shared_ptr<Type> baseType = postType.get()->getBaseType();
    GlobStruct::getInstance().typeQueue.pop();
    code_ << popReg("t1");
    code_ << popReg("t0");
    if (indexType == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    }
    if (lType == retType::LEFT) {
        code_ << "\tlw t0, (t0)\n";
    }
    if (postType.get()->getType() == "Array") {
        code_ << "\tli t2, " + std::to_string(baseType.get()->getSize()) << "\n"
              << "\tmul t1, t1, t2\n"
              << "\tadd t0, t0, t1\n";
    } else if (postType.get()->getType() == "Intptr") {
        code_ << "\tslli t1, t1, 2\n"
              << "\tadd t0, t0, t1\n";
    }
    code_ << pushReg("t0");
    if (postType.get()->getBaseType()->getType() == "Array") {
        return retType::RIGHT;
    }
    return retType::LEFT;
}

//@brief: visit Identifier node, we read the value of variable from stack 
//@ret: Variable type
antlrcpp::Any CodeEmission::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::string tmpFunc = curFunc;
    int tmpDep = stmtID;
    int line = ctx->start->getLine();
    for (int i = 0; i <= tmpDep; ++i) {
        if (GlobStruct::getInstance().symbolTable[tmpFunc].count(varName) == 0 || GlobStruct::getInstance().symbolTable[tmpFunc][varName].get()->getLineNum() > line) {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            continue;
        } 
        // Load the value from stack
        code_ << "\taddi a0, fp, " << -4 - 4 * GlobStruct::getInstance().symbolTable[tmpFunc][varName].get()->getOffset() << "\n"
              << pushReg("a0");
        if (GlobStruct::getInstance().symbolTable[tmpFunc][varName].get()->getType()->getType() == "Array") {
            return retType::RIGHT;
        }
        return retType::LEFT;
    }
    // Finally, we retrieve target varible from global scope
    // "la t0, <label>", load the label address to the register
    if (GlobStruct::getInstance().symbolTable["globl"].count(varName) > 0) {
        code_ << "\tla a0, " << varName << "\n"
              << pushReg("a0");
    }
    if (GlobStruct::getInstance().symbolTable["globl"][varName].get()->getType()->getType() == "Array") {
        return retType::RIGHT;
    }
    return retType::LEFT;
}

//@brief: Visit Assign node, eg. for expr "a = 3", we first find a's position in stack,
// then store 3 into specified memory.
//@ret: Variable type
antlrcpp::Any CodeEmission::visitAssign(MiniDecafParser::AssignContext *ctx) {
    code_ << "\t#assign\n";
    retType lType = visit(ctx->unary());
    retType rType = visit(ctx->expr());
    code_ << popReg("t1");
    if (rType == retType::LEFT) {
        code_ << "\tlw t1, (t1)\n";
    } 
    code_ << popReg("t0")
          << "\tsw t1, (t0)\n"
          << pushReg("t0");
    return retType::LEFT;
}

//@brief: Visit Ifstmt node, generate corresponding branch & jump instructions
antlrcpp::Any CodeEmission::visitIfElse(MiniDecafParser::IfElseContext *ctx) {

    // std::string t = ctx->expr()->getStart()->getText();
    // if (GlobStruct::getInstance().symbolTable[curFunc].count(t) > 0)
    // {
    //     // std::cout << "type: " << GlobStruct::getInstance().symbolTable[curFunc][t]->getType()->getStarNum() << std::endl;
    //     if (GlobStruct::getInstance().symbolTable[curFunc][t]->getType()->typeCheckLiteral("Intptr")) 
    //     {
    //        if (t != "np")
    //        {
    //             std::cerr << "line " << ctx->start->getLine() << ": "
    //                     << "[ERROR] Condition type error in if condition.\n";
    //             exit(1);
    //        }
    //     }
    // }
    
    retType type = visit(ctx->expr());
    code_ << popReg("a0");
    if (type == retType::LEFT) 
    {
        code_ << "\tlw a0, (a0)\n";
    }
    // If statement with else branch
    if (ctx->Else()) 
    {
        /* Different branch label, in RV-32 assembly, a branch operaion is like:
            ...
            beqz a0, label1
            label0:
                <instructions>
            label1:
                <instructions>
            ...
        */
        int elseBranch = label++;
        int endBranch = label++;
        code_ << "\tbeqz a0, .L" << elseBranch << "\n";
        visit(ctx->stmt(0));
        code_ << "\tj .L" << endBranch << "\n";
        code_ << ".L" << elseBranch << ":\n";
        visit(ctx->stmt(1));
        code_ << ".L" << endBranch << ":\n";
    } 
    else 
    {
        // If statement without else branch
        int endBranch = label++;
        code_ << "\tbeqz a0, .L" << endBranch << "\n";
        visit(ctx->stmt(0));
        code_ << ".L" << endBranch << ":\n";
    }
    return retType::UNDEF;
}

antlrcpp::Any CodeEmission::visitCondExpr(MiniDecafParser::CondExprContext *ctx) {
    /*
        Conditional tenary expression (x = a : b ? c) can be translated as:
        if (a) x = b; else x = c;
        Noted that the tenary expression has short-circuit property, which means
        if a is true, expression c cannot be reached.
    */
    // Code generation part is nearly the same as <ifStmt>
    retType type = visit(ctx->logical_or());
    int elseBranch = label++;
    int endBranch = label++;
    code_ << popReg("a0");
    if (type == retType::LEFT) {
        code_ << "\tlw a0, (a0)\n";
    }
    code_ << "\tbeqz a0, .L" << elseBranch << "\n";
    visit(ctx->expr());
    code_ << "\tj .L" << endBranch << "\n";
    code_ << ".L" << elseBranch << ":\n";
    visit(ctx->conditional());
    code_ << ".L" << endBranch << ":\n";
    return retType::RIGHT;
}

//@brief: Visit forloop node, generate branch model in RV32
antlrcpp::Any CodeEmission::visitForLoop(MiniDecafParser::ForLoopContext *ctx) {
    // According to our SPEC, forloop is also a new scope
    std::string tmpFunc = curFunc;
    curFunc += "_" + std::to_string(blockID) + std::to_string(++stmtID);
    int startBranch = label++;
    int endBranch = label++;
    int continueBranch = label++;
    breakTarget.push_back(endBranch);
    continueTarget.push_back(continueBranch);

    int exprBase = -1;
    // for (<decl|expr>; <expr>; <expr>) <stmt>
    // The Branch model is nearly the same as Dowhile
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

        exprBase = 0;
    }
    code_ << ".L" << startBranch << ":\n";
    if (ctx->expr(exprBase + 1)) 
    {
        retType type = visit(ctx->expr(exprBase+1));
        code_ << popReg("a0");
        if (type == retType::LEFT) {
            code_ << "\tlw a0, (a0)\n";
        }
        code_ << "\tbeqz a0, .L" << endBranch << "\n";
    } 
    visit(ctx->stmt());
    code_ << ".L" << continueBranch << ":\n";
    if (ctx->expr(exprBase+2)) {
        visit(ctx->expr(exprBase+2));
        code_ << popReg("t0");
    }
    code_ << "\tj .L" << startBranch << "\n"
                << ".L" << endBranch << ":\n";
    breakTarget.pop_back();
    continueTarget.pop_back();
    // When exit for loop scope, maintain the block depth & block order
    if (--stmtID == 0) {
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
    
    code_ << ".L" << startBranch << ":\n";
    retType type = visit(ctx->expr());
    code_ << popReg("a0");

    if (type == retType::LEFT) 
    {
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

//@brief: do <some statements> while <expr>
antlrcpp::Any CodeEmission::visitDoWhile(MiniDecafParser::DoWhileContext *ctx) {
    /*
        A loop model in RV32 assembly is like:
        ...
        .L0:
            <instructions>
            bnez a0, .L0
        .L1:
        ...
    */
    int startBranch = label++, endBranch = label++;
    breakTarget.push_back(endBranch);
    continueTarget.push_back(startBranch);

    code_ << ".L" << startBranch << ":\n";
    visit(ctx->stmt());
    retType type = visit(ctx->expr());


    std::string t = ctx->expr()->getStart()->getText();
    // std::cout << "t " << t << std::endl;
    if (GlobStruct::getInstance().symbolTable[curFunc].count(t) > 0)
    {
        if (GlobStruct::getInstance().symbolTable[curFunc][t]->getType()->typeCheckLiteral("Intptr"))
        {
            std::cerr << "line " << ctx->start->getLine() << ": "
                    << "[ERROR] Loop condition type error in do-while.\n";
            exit(1);
        }
    }

    code_ << popReg("a0");
    if (type == retType::LEFT) {
        code_ << "\tlw a0, (a0)\n";
    }
    code_ << "\tbnez a0, .L" << startBranch << "\n";
    code_ << ".L" << endBranch << ":\n";

    // We push the label into vector to deal with nested loop
    breakTarget.pop_back();
    continueTarget.pop_back();
    return retType::UNDEF;
}

//@brief: Break out of the current loop layer
antlrcpp::Any CodeEmission::visitBreak(MiniDecafParser::BreakContext *ctx) {
    code_ << "\tj .L" << breakTarget.back() << "\n";
    return retType::UNDEF;
}

//@brief: Jump to the tail of the current loop layer
antlrcpp::Any CodeEmission::visitContinue(MiniDecafParser::ContinueContext *ctx) {
    code_ << "\tj .L" << continueTarget.back() << "\n";
    return retType::UNDEF; 
}

//@brief: Pop unused element at the top of stack
antlrcpp::Any CodeEmission::visitExprStmt(MiniDecafParser::ExprStmtContext *ctx) {

    if (ctx->expr()) 
    {
        visit(ctx->expr());
        code_ << popReg("t0");
    }
    return retType::UNDEF;
}

//@brief: Deal with cast operation
antlrcpp::Any CodeEmission::visitCast(MiniDecafParser::CastContext *ctx) {
    retType type = visit(ctx->unary());
    return type;
}

std::string CodeEmission::pushReg(std::string reg) {
    return "\t#push\n\taddi sp, sp, -4\n\tsw " + reg + ", (sp)\n";
}

std::string CodeEmission::popReg(std::string reg) {
    return "\t#pop\n\tlw " + reg + ", (sp)\n\taddi sp, sp, 4\n";
}

std::string CodeEmission::popReg(std::string reg0, std::string reg1) {
    return "\t#pop2\n\tlw " + reg0 + ", 4(sp)\n\tlw " + reg1 + ", (sp)\n\taddi sp, sp, 8\n";
}