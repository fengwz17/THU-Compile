#include "StackAlloc.h"
#include "utils.h"

//@brief: Visit the root node of the generated syntax tree
antlrcpp::Any StackAlloc::visitFunc(MiniDecafParser::FuncContext *ctx) {
    
    curFunc = ctx->Identifier(0)->getText();
    stmtID = 0; 
    blockID = 0; 
    offset = 0;

    GlobStruct& instance = GlobStruct::getInstance(); 

    if (instance.symbolTable["globl"].count(curFunc) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Conflits of function " << curFunc << "with variable defined in line " 
            + std::to_string(instance.symbolTable["globl"][curFunc].get()->getLineNum()) + "\n";
        exit(1);
    }

    // Detect redefinition error
    if (!ctx->Semicolon()) 
    {
        if (instance.funcTable.count(curFunc) > 0) 
        {
            if (instance.funcTable[curFunc].get()->initialized()) 
            {
                std::cerr << "line " << ctx->start->getLine() << ": ";
                std::cerr << "[ERROR] Redefinition of function " << curFunc << "\n";
                exit(1);
            } 
            else 
            {
                instance.funcTable[curFunc].get()->initialize();

                // Initialize the parameter list and the return value 
                for (auto i = 1; i < ctx->Identifier().size(); ++i) 
                {
                    std::string varName = ctx->Identifier(i)->getText();
                    if (instance.symbolTable[curFunc].count(varName) > 0) 
                    {
                        std::cerr << "line " << ctx->start->getLine() << ": ";
                        std::cerr << "[ERROR] Function " << curFunc << "parameter " << varName << " redefined\n";
                        exit(1);
                    }
                    instance.symbolTable[curFunc][varName] = 
                        std::make_shared<Symbol>(varName, offset++, instance.funcTable[curFunc].get()->getArgType(i-1));
                }
                for (auto item : ctx->blockItem()) 
                {
                    visit(item);
                }
            }
        } 
        else 
        {
            std::vector<std::shared_ptr<Type> > argType;
            // Initialize the parameter list & the return value 
            for (auto i = 1; i < ctx->Identifier().size(); ++i) 
            {
                std::string varName = ctx->Identifier(i)->getText();
                if (instance.symbolTable[curFunc].count(varName) > 0) 
                {
                    std::cerr << "line " << ctx->start->getLine() << ": ";
                    std::cerr << "[ERROR] Function " << curFunc << "parameter " << varName << " redefined\n";
                    exit(1);
                }
                argType.push_back(visit(ctx->type(i)));
                instance.symbolTable[curFunc][varName] = std::make_shared<Symbol>(varName, offset++, argType[i-1]);
            }
            instance.funcTable[curFunc] = std::make_shared<FuncSymbol>(curFunc, visit(ctx->type(0)), argType, true);
            for (auto item : ctx->blockItem()) 
            {
                visit(item);
            }
        }
    } 
    else 
    {
        std::vector<std::shared_ptr<Type> > argType;
        for (auto i = 1; i < ctx->type().size(); ++i) 
        {
            argType.push_back(visit(ctx->type(i)));
        }
        instance.funcTable[curFunc] = std::make_shared<FuncSymbol>(curFunc, visit(ctx->type(0)), argType);
    }
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitBlock(MiniDecafParser::BlockContext *ctx) {

    ++stmtID;
    curFunc += "_" + std::to_string(blockID) + std::to_string(stmtID);
    for (auto item : ctx->blockItem()) 
    {
        visit(item);
    }
    if (--stmtID == 0) 
    {
        ++blockID;
    }
    int pos = curFunc.rfind('_');
    curFunc = curFunc.substr(0, pos);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitRetStmt(MiniDecafParser::RetStmtContext *ctx) {

    if (curFunc == "main" && ctx->expr())
    {
        std::shared_ptr<Type> baseType = visit(ctx->expr());
        // std::cout << "type: " << baseType->getType() << std::endl;

        if (baseType->getType() != "Int")
        {
            std::cerr << "line " << ctx->start->getLine() << ": "
                    << "[ERROR] Bad return type.\n";
            exit(1);
        }
    }
    
    visit(ctx->expr());
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitIfElse(MiniDecafParser::IfElseContext *ctx) {
    
    if (ctx->expr())
    {
        std::shared_ptr<Type> baseType = visit(ctx->expr());
        // std::cout << "dddd " << ctx->expr()->getText() << std::endl;
        if (baseType->getType() != "Int")
        {
            std::cerr << "line " << ctx->start->getLine() << ": "
                    << "[ERROR] Bad if-condition type.\n";
            exit(1);
        }
    }

    // visit(ctx->expr());
    
    // If statement with else branch
    if (ctx->Else()) 
    {
        visit(ctx->stmt(0));
        visit(ctx->stmt(1));
    } 
    else 
    {
        visit(ctx->stmt(0));
    }
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitGlobalArry(MiniDecafParser::GlobalArryContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    GlobStruct& instance = GlobStruct::getInstance(); 
    if (instance.funcTable.count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Conflits of array " << varName << "with function\n"; 
        exit(1);
    }
    if (instance.symbolTable["globl"].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of global variable " << varName << "\n";
        exit(1);
    }
    // Array dimension cannot be 0
    int volume = 1;
    std::shared_ptr<Type> baseType = visit(ctx->type());
    baseType.get()->setValueType(1);
    for (int i = ctx->Integer().size()-1; i >= 0; --i) {
        int arrDim = std::stoi(ctx->Integer(i)->getText());
        if (arrDim == 0) {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Array " << varName << " has 0 dimension\n";
            exit(1);
        }
        volume *= arrDim;
        baseType = std::make_shared<ArrayType>(baseType, volume);
    }
    instance.symbolTable["globl"][varName] = std::make_shared<Symbol>(varName, 0, baseType, ctx->start->getLine(), volume);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitLocalArry(MiniDecafParser::LocalArryContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    GlobStruct& instance = GlobStruct::getInstance(); 
    if (instance.symbolTable[curFunc].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of global variable " << varName << "\n";
        exit(1);
    }

    int volume = 1;
    std::shared_ptr<Type> baseType = visit(ctx->type());
    baseType.get()->setValueType(1);

    for (int i = ctx->Integer().size()-1; i >= 0; --i) 
    {
        int arrDim = std::stoi(ctx->Integer(i)->getText());
        if (arrDim == 0) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Array " << varName << " has 0 dimension\n";
            exit(1);
        }
        volume *= arrDim;
        baseType = std::make_shared<ArrayType>(baseType, arrDim);
    }
    offset += volume;
    instance.symbolTable[curFunc][varName] = std::make_shared<Symbol>(varName, offset-1, baseType, ctx->start->getLine(), volume);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitGlobal(MiniDecafParser::GlobalContext *ctx) {
    GlobStruct& instance = GlobStruct::getInstance(); 
    std::string varName = ctx->Identifier()->getText();
    if (instance.funcTable.count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Conflits of variable " << varName << "with function\n"; 
        exit(1);
    }
    if (instance.symbolTable["globl"].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of global variable " << varName << "\n";
        exit(1);
    }
    instance.symbolTable["globl"][varName] = std::make_shared<Symbol>(varName, 0, visit(ctx->type()), ctx->start->getLine());
    instance.symbolTable["globl"][varName].get()->getType()->setValueType(1);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    GlobStruct& instance = GlobStruct::getInstance(); 
    std::string varName = ctx->Identifier()->getText();
    if (instance.symbolTable[curFunc].count(varName) > 0) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of variable " << varName << "\n";
        exit(1);
    }
    /* Solve the late declaration of a variable by counting the line number
       @TODO: Note that the implementation here only considers line number, so
       when you write your codes in one line, you also need to check the column number. 
    */
    std::shared_ptr<Type> lType = visit(ctx->type());
    if (ctx->expr()) {
        std::shared_ptr<Type> rType = visit(ctx->expr());
        if (!lType.get()->typeCheck(rType)) {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Incompatible parameter type for vardef operation\n";
            exit(1);
        }
    }
    instance.symbolTable[curFunc][varName] = std::make_shared<Symbol>(varName, offset++, lType, ctx->start->getLine());
    instance.symbolTable[curFunc][varName].get()->getType()->setValueType(1);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}



//@brief: Make sure that a variable must be defined before assigning it
antlrcpp::Any StackAlloc::visitAssign(MiniDecafParser::AssignContext *ctx) {
    std::string tmpFunc = curFunc;
    std::shared_ptr<Type> lType = visit(ctx->unary());
    std::shared_ptr<Type> rType = visit(ctx->expr());
    // The two operators must have the same data type.
    if (!lType.get()->typeCheck(rType)) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible parameter type for assign operation\n";
        exit(1);
    }
    if (lType.get()->getValueType() == 0) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Assign to right value is forbiddened\n";
        exit(1);
    }
    return lType;
}

//@brief: A forloop is also a new scope
antlrcpp::Any StackAlloc::visitForLoop(MiniDecafParser::ForLoopContext *ctx) {
    curFunc += "_" + std::to_string(blockID) + std::to_string(++stmtID);
    int exprBase = -1;
    // for (<decl|expr>; <expr>; <expr>) <stmt>
    // The Branch model is nearly the same as Dowhile
    if (ctx->declaration()) {
        visit(ctx->declaration());
    } else if (ctx->expr(0)) {
        visit(ctx->expr(0));
        exprBase = 0;
    }
    if (ctx->expr(exprBase+1)) {
        visit(ctx->expr(exprBase+1));
    } 
    visit(ctx->stmt());
    if (ctx->expr(exprBase+2)) {
        visit(ctx->expr(exprBase+2));
    }
    if (--stmtID == 0) {
        ++blockID;
    }
    int pos = curFunc.rfind('_');
    curFunc = curFunc.substr(0, pos);
    
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitWhileLoop(MiniDecafParser::WhileLoopContext *ctx) {
    
    if (ctx->expr())
    {
        std::shared_ptr<Type> baseType = visit(ctx->expr());
        // std::cout << "dddd " << ctx->expr()->getText() << std::endl;
        if (baseType->getType() != "Int")
        {
            std::cerr << "line " << ctx->start->getLine() << ": "
                    << "[ERROR] Bad while-condition type.\n";
            exit(1);
        }
    }
   
    visit(ctx->stmt());

    std::shared_ptr<Type> type = std::make_shared<IntType>();
    return type;
}

//@brief: Most of the work is type checking
antlrcpp::Any StackAlloc::visitEqual(MiniDecafParser::EqualContext *ctx) {
    std::shared_ptr<Type> ltype = visit(ctx->equality(0));
    std::shared_ptr<Type> rtype = visit(ctx->equality(1));

    
    if ((ltype.get()->getType() == "Array") && (rtype.get()->getType() == "Array")) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad (not)equality type\n";
        exit(1);
    }

    if (!ltype.get()->typeCheck(rtype)) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible parameter type for equal operation\n";
        exit(1);
    } 
    std::shared_ptr<Type> type = std::make_shared<IntType>();
    return type;
}

//@brief: Most of the work is type checking
antlrcpp::Any StackAlloc::visitAddSub(MiniDecafParser::AddSubContext *ctx) {
    GlobStruct& instance = GlobStruct::getInstance(); 
    std::shared_ptr<Type> firstAdder = visit(ctx->add(0));
    std::shared_ptr<Type> secondAdder = visit(ctx->add(1));
    instance.typeQueue.push(firstAdder);
    instance.typeQueue.push(secondAdder);
    std::shared_ptr<Type> ansType = std::make_shared<NoneType>();

    if (firstAdder.get()->getType() == "Array" && secondAdder.get()->getType() == "Int") 
    {
        if (ctx->Addition()) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Add operation between pointers is not supported\n";
            exit(1);
        } 
        else if (ctx->Minus()) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Sub operation between pointers is not supported\n";
            exit(1);
        }
    } 


    if (firstAdder.get()->getType() == "Intptr" && secondAdder.get()->getType() == "Intptr") 
    {
        if (ctx->Addition()) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Add operation between pointers is not supported\n";
            exit(1);
        } 
        else if (ctx->Minus()) 
        {
            if (firstAdder.get()->getStarNum() != secondAdder.get()->getStarNum())
            {
                std::cerr << "line " << ctx->start->getLine() << ": ";
                std::cerr << "[ERROR] Sub operation between different pointer types is not supported\n";
                exit(1);
            }
            ansType = std::make_shared<IntType>();
        }
    } 
    else if (firstAdder.get()->getType() == "Intptr") 
    {
        ansType = std::make_shared<IntptrType>(firstAdder.get()->getStarNum());
    } 
    else if (secondAdder.get()->getType() == "Intptr") 
    {
        if (ctx->Addition()) 
        {
            ansType = std::make_shared<IntptrType>(secondAdder.get()->getStarNum());
        } 
        else if (ctx->Minus())
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Integer minus pointer is not supported\n";
            exit(1);
        }
    } 
    else 
    {
        ansType = std::make_shared<IntType>();
    }
    
    return ansType;
}

antlrcpp::Any StackAlloc::visitMulDiv(MiniDecafParser::MulDivContext *ctx) {

    std::shared_ptr<Type> firstAdder = visit(ctx->mul(0));
    std::shared_ptr<Type> secondAdder = visit(ctx->mul(1));
    std::shared_ptr<Type> ansType = std::make_shared<NoneType>();

    if (firstAdder.get()->getType() == "Intptr" && secondAdder.get()->getType() == "Intptr") 
    {
    
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Mul and Div operation between pointers is not supported\n";
        exit(1);
    } 
    else if (firstAdder.get()->getType() == "Intptr") 
    {
        ansType = std::make_shared<IntptrType>(firstAdder.get()->getStarNum());
    } 
    else if (secondAdder.get()->getType() == "Intptr") 
    {
        if (ctx->Multiplication()) 
        {
            ansType = std::make_shared<IntptrType>(secondAdder.get()->getStarNum());
        } 
        else if (ctx->Division())
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Div pointer is not supported\n";
            exit(1);
        }
    } 
    else 
    {
        ansType = std::make_shared<IntType>();
    }
    
    return ansType;
}
       
antlrcpp::Any StackAlloc::visitUnaryOp(MiniDecafParser::UnaryOpContext *ctx) {

    std::shared_ptr<Type> src = visit(ctx->unary());
    std::shared_ptr<Type> type = std::make_shared<IntType>();
    int starNum = src.get()->getStarNum();

    if (ctx->Multiplication()) 
    {
        if (src.get()->typeCheckLiteral("Intptr")) 
        {
            if (starNum == 1) 
            {
                type = std::make_shared<IntType>(1);
            } 
            else 
            {
                type = std::make_shared<IntptrType>(starNum-1, 1);
            }
        } 
        else 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Invalid operation '*' on non-pointer type\n";
            exit(1);
        }
    } 
    else if (ctx->AND()) 
    {
        if (src.get()->getValueType() == 0) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Invalid operation '&' on right value\n";
            exit(1);
        } 
        else 
        {
            type = std::make_shared<IntptrType>(starNum+1);
        }
    } 
    else 
    {
        if (!(src.get()->getType() == "Int")) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Bad type in basic unary operation\n";
            exit(1);
        }
    }
    return type;
}

antlrcpp::Any StackAlloc::visitArryIndex(MiniDecafParser::ArryIndexContext *ctx) {
    std::shared_ptr<Type> postType = visit(ctx->postfix());
    std::shared_ptr<Type> type = visit(ctx->expr());
    GlobStruct::getInstance().typeQueue.push(postType);
    std::shared_ptr<Type> returnType;
    if (type.get()->getType() != "Int") 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Array index must be integer\n";
        exit(1);
    }
    if (postType.get()->getType() == "Intptr") 
    {
        int starNum = postType.get()->getStarNum();
        if (starNum == 1) 
        {
            type = std::make_shared<IntType>(1);
        } 
        else 
        {
            type = std::make_shared<IntptrType>(starNum-1, 1);
        }
        return type;
    } 
    else 
    {
        return postType.get()->getBaseType();
    }
}

antlrcpp::Any StackAlloc::visitCast(MiniDecafParser::CastContext *ctx) {
    std::shared_ptr<Type> dst = visit(ctx->type());
    std::shared_ptr<Type> src = visit(ctx->unary());
    return dst.get()->typeCast(src.get()->getValueType());
}

//@brief: Make sure functions are declared before calling them
antlrcpp::Any StackAlloc::visitFuncCall(MiniDecafParser::FuncCallContext *ctx) {
    GlobStruct& instance = GlobStruct::getInstance(); 
    std::string funcName = ctx->Identifier()->getText();
    if (instance.funcTable.count(funcName) == 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Use of undeclared function " << funcName << "\n";
        exit(1);
    }
    if (ctx->expr().size() != instance.funcTable[funcName].get()->getArgSize()) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible parameter number for function " << funcName << "\n";
        exit(1);
    }
    for (int i = ctx->expr().size()-1; i >= 0; --i) 
    {
        if (!instance.funcTable[funcName].get()->getArgType(i).get()->typeCheck(visit(ctx->expr(i)))) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Incompatible parameter type for function " << funcName << "\n";
            exit(1);
        }
    }
    return instance.funcTable[funcName].get()->getRetType();
}

antlrcpp::Any StackAlloc::visitParen(MiniDecafParser::ParenContext *ctx) {
    return visit(ctx->expr());
}

//@brief: Make sure that a variable must be defined before using it
antlrcpp::Any StackAlloc::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    GlobStruct& instance = GlobStruct::getInstance(); 
    std::string varName = ctx->Identifier()->getText();
    std::string tmpFunc = curFunc;
    // Search the symbol table from inner scope to the outer, for the target symbol
    for (int i = 0; i <= stmtID; ++i) {
        if (instance.symbolTable[tmpFunc].count(varName) == 0) {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0,pos);
            continue;
        }
        return instance.symbolTable[tmpFunc][varName].get()->getType();
    }
    // Search in global scope at last
    if (instance.symbolTable["globl"].count(varName) == 0) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Variable " << varName << " is used without definition\n";
        exit(1);
    }
    return instance.symbolTable["globl"][varName].get()->getType();
}

antlrcpp::Any StackAlloc::visitInteger(MiniDecafParser::IntegerContext *ctx) {
    std::shared_ptr<Type> type = std::make_shared<IntType>(); 
    return type;
}

//@brief: Get the type
antlrcpp::Any StackAlloc::visitType(MiniDecafParser::TypeContext *ctx) {
    // Star number means the number of '*' in the typedef
    // Naturally, we treat int type as 0 star number type
    int starNum = ctx->Multiplication().size();
    std::shared_ptr<Type> type;
    if (starNum == 0) {
        type =  std::make_shared<IntType>();
    } else if (starNum > 0) {
        type = std::make_shared<IntptrType>(starNum);
    } else {
        type = std::make_shared<NoneType>();
    }
    return type;
}

antlrcpp::Any StackAlloc::visitCondExpr(MiniDecafParser::CondExprContext *ctx) {
    std::shared_ptr<Type> pairType = std::make_shared<IntType>();
    std::shared_ptr<Type> lorType = visit(ctx->logical_or()), exprType = visit(ctx->expr()), 
    condType = visit(ctx->conditional());
    if(!pairType.get()->typeCheck(visit(ctx->logical_or()))) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad conditional type\n";
        exit(1);
    }
    if(!exprType.get()->typeCheck(condType)) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible conditional branches\n";
        exit(1);
    }
    return lorType;
}

antlrcpp::Any StackAlloc::visitCompare(MiniDecafParser::CompareContext *ctx) {
    std::shared_ptr<Type> lType = visit(ctx->relational(0));
    std::shared_ptr<Type> rType = visit(ctx->relational(1));


    if (!(lType.get()->getType() == "Int") || !(rType.get()->getType() == "Int")) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad great or less type\n";
        exit(1);
    }
    std::shared_ptr<Type> retType = std::make_shared<IntType>();
    return retType;
}

antlrcpp::Any StackAlloc::visitLor(MiniDecafParser::LorContext *ctx) {
    std::shared_ptr<Type> lType = visit(ctx->logical_or(0));
    std::shared_ptr<Type> rType = visit(ctx->logical_or(1));
    if (!(lType.get()->getType() == "Int") || !(rType.get()->getType() == "Int")) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad great or less type\n";
        exit(1);
    }
    std::shared_ptr<Type> retType = std::make_shared<IntType>();
    return retType;
}

antlrcpp::Any StackAlloc::visitLand(MiniDecafParser::LandContext *ctx) {
    std::shared_ptr<Type> lType = visit(ctx->logical_and(0));
    std::shared_ptr<Type> rType = visit(ctx->logical_and(1));
    if (!(lType.get()->getType() == "Int") || !(rType.get()->getType() == "Int")) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad great or less type\n";
        exit(1);
    }
    std::shared_ptr<Type> retType = std::make_shared<IntType>();
    return retType;
}