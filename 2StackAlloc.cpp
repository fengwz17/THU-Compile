#include "StackAlloc.h"
#include "utils.h"
#include "set"

// //Visit the root node of AST
// antlrcpp::Any StackAlloc::visitProg(MiniDecafParser::ProgContext *ctx, varTab& varID) {
//     offset = 0;
//     // std::cout << "d" << std::endl;
//     visitChildren(ctx);
//     // std::cout << "dd" << std::endl;
//     // varID["a"] = 2;
//     return varTable;
// }

antlrcpp::Any StackAlloc::visitFunc(MiniDecafParser::FuncContext *ctx) {
   
    curFunc = ctx->Identifier(0)->getText();
    blockID = 0;
    stmtID = 0;
    offset = 0;

    // // GlobStruct& instance = GlobStruct::getInstance();
    // std::string lineStr = std::to_string(GlobStruct::getInstance().symbolTable["globl"][curFunc].get()->getLineNum());
    
    
    if (GlobStruct::getInstance().symbolTable["globl"].count(curFunc) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Conflits of function " << curFunc << "with variable defined in line " + std::to_string(GlobStruct::getInstance().symbolTable["globl"][curFunc].get()->getLineNum()) + "\n";
        exit(1);
    }

    // decl
    if (ctx->Semicolon()) 
    {    
        std::vector<std::shared_ptr<Type> > argType;
        for (auto i = 1; i < ctx->type().size(); ++i) 
        {
            argType.push_back(visit(ctx->type(i)));
        }
        GlobStruct::getInstance().funcTable[curFunc] = std::make_shared<FuncSymbol>(curFunc, visit(ctx->type(0)), argType);
    }

    // definition
    else
    {
        if (GlobStruct::getInstance().GlobStruct::getInstance().funcTable.count(curFunc) > 0)
        {
            if (GlobStruct::getInstance().funcTable[curFunc].get()->initialized()) 
            {
                std::cerr << "line " << ctx->start->getLine() << ": ";
                std::cerr << "[ERROR] Redefinition of function " << curFunc << "\n";
                exit(1);
            } 
            else 
            {
                GlobStruct::getInstance().funcTable[curFunc].get()->initialize();

                // Initialize the parameter list & the return value 
                for (auto i = 1; i < ctx->Identifier().size(); ++i) 
                {
                    std::string varName = ctx->Identifier(i)->getText();
                    if (GlobStruct::getInstance().symbolTable[curFunc].count(varName) > 0) 
                    {
                        std::cerr << "line " << ctx->start->getLine() << ": ";
                        std::cerr << "[ERROR] Function " << curFunc << "parameter " << varName << " redefined\n";
                        exit(1);
                    }
                    
                    GlobStruct::getInstance().symbolTable[curFunc][varName] = 
                        std::make_shared<Symbol>(varName, offset++, GlobStruct::getInstance().funcTable[curFunc].get()->getArgType(i-1));
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
                
                if (GlobStruct::getInstance().symbolTable[curFunc].count(varName) > 0) 
                {
                    std::cerr << "line " << ctx->start->getLine() << ": ";
                    std::cerr << "[ERROR] Function " << curFunc << "parameter " << varName << " redefined\n";
                    exit(1);
                } 
                
                argType.push_back(visit(ctx->type(i)));
                GlobStruct::getInstance().symbolTable[curFunc][varName] = std::make_shared<Symbol>(varName, offset++, argType[i-1]);
            }
            GlobStruct::getInstance().funcTable[curFunc] = std::make_shared<FuncSymbol>(curFunc, visit(ctx->type(0)), argType, true);
            for (auto item : ctx->blockItem()) 
            {
                visit(item);
            }
        }
            // visitChildren(ctx);
    } 

    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitBlock(MiniDecafParser::BlockContext *ctx) {
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
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitGlobalArry(MiniDecafParser::GlobalArryContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    // std::cout << "ddddddd" << varName << std::endl;
    if (GlobStruct::getInstance().funcTable.count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Conflits of array " << varName << "with function\n"; 
        exit(1);
    }
    if (GlobStruct::getInstance().symbolTable["globl"].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of global variable " << varName << "\n";
        exit(1);
    }

    // Array dimension cannot be 0
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
        baseType = std::make_shared<ArrayType>(baseType, volume);
    }

    GlobStruct::getInstance().symbolTable["globl"][varName] = std::make_shared<Symbol>(varName, 0, baseType, ctx->start->getLine(), volume);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitLocalArry(MiniDecafParser::LocalArryContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    if (GlobStruct::getInstance().symbolTable[curFunc].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of global variable " << varName << "\n";
        exit(1);
    }

    // Array dimension cannot be 0
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
    GlobStruct::getInstance().symbolTable[curFunc][varName] = std::make_shared<Symbol>(varName, offset-1, baseType, ctx->start->getLine(), volume);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}


antlrcpp::Any StackAlloc::visitGlobal(MiniDecafParser::GlobalContext *ctx) {
    std::string varName = ctx->Identifier()->getText();

    if (GlobStruct::getInstance().funcTable.count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Conflits of variable " << varName << " with function\n"; 
        exit(1);
    }

    if (GlobStruct::getInstance().symbolTable["globl"].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of global variable " << varName << "\n";
        exit(1);
    }

    GlobStruct::getInstance().symbolTable["globl"][varName] = std::make_shared<Symbol>(varName, 0, visit(ctx->type()), ctx->start->getLine());
    GlobStruct::getInstance().symbolTable["globl"][varName].get()->getType()->setValueType(1);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;

}

antlrcpp::Any StackAlloc::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    // allocate the defined varible
    std::string varName = ctx->Identifier()->getText();
    // varID[varName]++;
    
   
    // if (varTable[curFunc].count(varName) != 0) 
    // {
    //     std::cerr << "[ERROR] Variable Redifined" << varName << "\n";
    //     exit(1);
    // }
    if (GlobStruct::getInstance().symbolTable[curFunc].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of variable " << varName << "\n";
        exit(1);
    }


    // if (varTable[curFunc].count(varName + "#") > 0) 
    // {
    //     varTable[curFunc][varName + "#"] =  varID[varName];
    //     // varTable[curFunc][varName + "#"] = ctx->start->getLine();
    // }
    
    // // std::cout << "varTable: " << curFunc << " " << varName << " " << varTable[curFunc][varName] << '\n';
    // varTable[curFunc][varName] = offset++;
    // // std::cout << "varTable: " << curFunc << " " << varName << " " << varTable[curFunc][varName] << '\n';
    
    std::shared_ptr<Type> lType = visit(ctx->type());
    if (ctx->expr()) 
    {
        std::shared_ptr<Type> rType = visit(ctx->expr());
        if (!lType.get()->typeCheck(rType)) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] type error for variable definition\n";
            exit(1);
        }
    }
    GlobStruct::getInstance().symbolTable[curFunc][varName] = std::make_shared<Symbol>(varName, offset++, lType, ctx->start->getLine());
    GlobStruct::getInstance().symbolTable[curFunc][varName].get()->getType()->setValueType(1);
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

antlrcpp::Any StackAlloc::visitAssign(MiniDecafParser::AssignContext *ctx) {
    // std::string varName = ctx->Identifier()->getText();
    // varID[varName]++;
   
    std::string tmpFunc = curFunc;
    std::shared_ptr<Type> lType = visit(ctx->unary());
    std::shared_ptr<Type> rType = visit(ctx->expr());

    // std::string varName = ctx->expr()->getStart()->getText();

    if (!lType.get()->typeCheck(rType)) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible parameter type for assign operation\n";
        exit(1);
    }
    if (lType.get()->getValueType() == 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Assign to right value is forbiddened\n";
        exit(1);
    }
    return lType;
}

antlrcpp::Any StackAlloc::visitForLoop(MiniDecafParser::ForLoopContext *ctx) {
    stmtID++;
    curFunc += "_" + std::to_string(blockID) + std::to_string(stmtID);
    
    int exprBase = -1;

    if (ctx->declaration()) 
    {
        visit(ctx->declaration());
    }
    else if (ctx->expr(0)) 
    {
        visit(ctx->expr(0));
        exprBase = 0;
    }
    if (ctx->expr(exprBase + 1)) 
    {
        visit(ctx->expr(exprBase + 1));
    } 
    visit(ctx->stmt());
    if (ctx->expr(exprBase + 2)) 
    {
        visit(ctx->expr(exprBase + 2));
    }

    stmtID--;
    if (stmtID == 0) 
    {
        blockID++;
    }
    int pos = curFunc.rfind('_');
    curFunc = curFunc.substr(0, pos);
    // return retType::UNDEF;
    std::shared_ptr<Type> type = std::make_shared<NoneType>();
    return type;
}

// type checking
antlrcpp::Any StackAlloc::visitEqual(MiniDecafParser::EqualContext *ctx) {
    std::shared_ptr<Type> ltype = visit(ctx->equality(0));
    std::shared_ptr<Type> rtype = visit(ctx->equality(1));
    if (!ltype.get()->typeCheck(rtype)) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible parameter type for equal operation\n";
        exit(1);
    } 
    std::shared_ptr<Type> type = std::make_shared<IntType>();
    return type;
}

antlrcpp::Any StackAlloc::visitAddSub(MiniDecafParser::AddSubContext *ctx) {
    std::shared_ptr<Type> add1 = visit(ctx->add(0));
    std::shared_ptr<Type> add2 = visit(ctx->add(1));
    GlobStruct::getInstance().typeQueue.push(add1);
    GlobStruct::getInstance().typeQueue.push(add2);
    std::shared_ptr<Type> ansType = std::make_shared<NoneType>();

    if (add1.get()->getType() == "Intptr" && add2.get()->getType() == "Intptr") 
    {
        if (ctx->Addition()) 
        {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Add operation between pointers is not supported\n";
            exit(1);
        } 
        else if (ctx->Minus()) 
        {
            ansType = std::make_shared<IntType>();
        }
    } 
    else if (add1.get()->getType() == "Intptr") 
    {
        ansType = std::make_shared<IntptrType>(add1.get()->getStarNum());
    } 
    else if (add2.get()->getType() == "Intptr") 
    {
        if (ctx->Addition()) 
        {
            ansType = std::make_shared<IntptrType>(add2.get()->getStarNum());
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


antlrcpp::Any StackAlloc::visitUnaryOp(MiniDecafParser::UnaryOpContext *ctx) {
    
    // error: if (ctx->Exclamation() || ctx->Minus() || ctx->Tilde()) 
    std::shared_ptr<Type> src = visit(ctx->unary());
    std::shared_ptr<Type> type = std::make_shared<IntType>();
    int starNum = src.get()->getStarNum();

    // '*' operator must operate on pointer type
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
        // '&' operator must operate on left value 
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
            type = std::make_shared<IntptrType>(starNum - 1, 1);
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

antlrcpp::Any StackAlloc::visitFuncCall(MiniDecafParser::FuncCallContext *ctx) {
    
    std::string funcName = ctx->Identifier()->getText();
    if (GlobStruct::getInstance().funcTable.count(funcName) == 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Use of undeclared function " << funcName << "\n";
        exit(1);
    }
    if (ctx->expr().size() != GlobStruct::getInstance().funcTable[funcName].get()->getArgSize()) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible parameter number for function " << funcName << "\n";
        exit(1);
    }
    for (auto i = 0; i < ctx->expr().size(); ++i) 
    {
        if (!GlobStruct::getInstance().funcTable[funcName].get()->getArgType(i).get()->typeCheck(visit(ctx->expr(i)))) {
            std::cerr << "line " << ctx->start->getLine() << ": ";
            std::cerr << "[ERROR] Incompatible parameter type for function " << funcName << "\n";
            exit(1);
        }
    }
    // visitChildren(ctx);
    return GlobStruct::getInstance().funcTable[funcName].get()->getRetType();
}

antlrcpp::Any StackAlloc::visitParen(MiniDecafParser::ParenContext *ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any StackAlloc::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::string tmp = curFunc;

    for (int i = 0; i <= stmtID; ++i) 
    {
        if (GlobStruct::getInstance().symbolTable[tmp].count(varName) == 0) 
        {
            int pos = tmp.rfind('_');
            tmp = tmp.substr(0,pos);
            continue;
        }
        return GlobStruct::getInstance().symbolTable[tmp][varName].get()->getType();
    }

    // Search in global scope at last
    if (GlobStruct::getInstance().symbolTable["globl"].count(varName) == 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Variable " << varName << " is used without definition\n";
        exit(1);
    }
    return GlobStruct::getInstance().symbolTable["globl"][varName].get()->getType();
}

antlrcpp::Any StackAlloc::visitInteger(MiniDecafParser::IntegerContext *ctx) {
    std::shared_ptr<Type> type = std::make_shared<IntType>(); 
    return type;
}

antlrcpp::Any StackAlloc::visitType(MiniDecafParser::TypeContext *ctx) {
    int starNum = ctx->Multiplication().size();
    std::shared_ptr<Type> type;
    if (starNum == 0) 
    {
        type =  std::make_shared<IntType>();
    } 
    else if (starNum > 0) 
    {
        type = std::make_shared<IntptrType>(starNum);
    } 
    else 
    {
        type = std::make_shared<NoneType>();
    }
    return type;
}

antlrcpp::Any StackAlloc::visitCondExpr(MiniDecafParser::CondExprContext *ctx) {
    std::shared_ptr<Type> pairType = std::make_shared<IntType>();
    std::shared_ptr<Type> lorType = visit(ctx->logical_or()), exprType = visit(ctx->expr()), 
    condType = visit(ctx->conditional());
    if(!pairType.get()->typeCheck(visit(ctx->logical_or()))) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad conditional type\n";
        exit(1);
    }
    if(!exprType.get()->typeCheck(condType)) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Incompatible conditional branches\n";
        exit(1);
    }
    return lorType;
}

antlrcpp::Any StackAlloc::visitCompare(MiniDecafParser::CompareContext *ctx) {
    std::shared_ptr<Type> lType = visit(ctx->relational(0)), rType = visit(ctx->relational(1));
    if (!(lType.get()->getType() == "Int") || !(rType.get()->getType() == "Int")) {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad great or less type\n";
        exit(1);
    }
    std::shared_ptr<Type> retType = std::make_shared<IntType>();
    return retType;
}

antlrcpp::Any StackAlloc::visitLor(MiniDecafParser::LorContext *ctx) {
    std::shared_ptr<Type> lType = visit(ctx->logical_or(0)), rType = visit(ctx->logical_or(1));
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
    std::shared_ptr<Type> lType = visit(ctx->logical_and(0)), rType = visit(ctx->logical_and(1));
    if (!(lType.get()->getType() == "Int") || !(rType.get()->getType() == "Int")) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Bad great or less type\n";
        exit(1);
    }
    std::shared_ptr<Type> retType = std::make_shared<IntType>();
    return retType;
} 	 
