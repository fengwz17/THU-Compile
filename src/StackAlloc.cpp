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
    
    // // funcDeName.insert(curFunc);
    // if (varTable["globl"].count(curFunc) > 0)
    // {
    //     std::cerr << "[ERROR] funcName and global conflict\n";
    //     exit(1);
    // }

    // decl
    if (ctx->Semicolon()) 
    {
        // for (auto i = 1; i < ctx->Identifier().size(); ++i) 
        // {
        //     // std::string varName = ctx->Identifier(i)->getText();
        //     // std::cout << "name: " << curFunc << " " << funcParaTable[curFunc]++ << '\n';
        //     funcParaTable[curFunc]++;
        // }
   
        // varTable[curFunc]["$"] = 0;

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
        // if (varTable.count(curFunc) > 0 && varTable[curFunc]["$"] == 1) 
        // {
        //     std::cerr << "[ERROR] function redefined " << curFunc << "\n";
        //     exit(1);
        // } 
        /*
            varTable[curFunc]["$"] = 1;
            if (funcParaTable.count(curFunc) > 0)
            {
                if (funcParaTable[curFunc] != ctx->Identifier().size() - 1)
                {
                    std::cerr << "[ERROR] Conflict decl and def \n";
                    exit(1);
                } 
            }
            else
            {
                for (auto i = 1; i < ctx->Identifier().size(); ++i) 
                {
                    funcParaTable[curFunc]++;
                }
            }
            
            std::set<std::string> tmpVar;
            for (auto i = 1; i < ctx->Identifier().size(); ++i) 
            {
                std::string varName = ctx->Identifier(i)->getText();
                if (tmpVar.count(varName) > 0)
                {
                    std::cerr << "[ERROR] Conflict parameters \n";
                    exit(1);
                }
                tmpVar.insert(varName);
                varTable[curFunc][varName] = offset++;
            }
        */

        if (GlobStruct::getInstance().GlobStruct::getInstance().funcTable.count(curFunc) > 0)
        {
            if (GlobStruct::getInstance().GlobStruct::getInstance().funcTable[curFunc].get()->initialized()) 
            {
                std::cerr << "line " << ctx->start->getLine() << ": ";
                std::cerr << "[ERROR] Redefinition of function " << curFunc << "\n";
                exit(1);
            } 
            else 
            {
                GlobStruct::getInstance().GlobStruct::getInstance().funcTable[curFunc].get()->initialize();

                // Initialize the parameter list & the return value 
                for (auto i = 1; i < ctx->Identifier().size(); ++i) 
                {
                    std::string varName = ctx->Identifier(i)->getText();
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

antlrcpp::Any StackAlloc::visitGlobal(MiniDecafParser::GlobalContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    // if (funcDeName.count(varName) > 0)
    // {
    //     std::cerr << "[ERROR] Global and funcName conflict\n";
    //     exit(1);
    // }
    // else
    // {
    //     funcDeName.insert(varName);
    // }
    // if (varTable.count(varName) > 0)
    // {
    //     std::cerr << "[ERROR] Global and funcName conflict\n";
    //     exit(1);
    // }
    
    // if (varTable["globl"].count(varName) > 0) 
    // {
    //     std::cerr << "[ERROR] Global variable " << varName << "redifined \n";
    //     exit(1);
    // }
    // varTable["globl"][varName] = 0;

    if (GlobStruct::getInstance().symbolTable[curFunc].count(varName) > 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of variable " << varName << "\n";
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

    // std::string tmpFunc = curFunc;
    // // Search the symbol table from inner scope to the outer, for the target symbol
    // for (int i = 0; i <= stmtID; i++) 
    // {
    //     if (varTable[tmpFunc].count(varName) == 0) 
    //     {
    //         int pos = tmpFunc.rfind('_');
    //         tmpFunc = tmpFunc.substr(0, pos);
    //         continue;
    //     }
    //     if (varTable[curFunc].count(varName) == 0) 
    //     {
    //         varTable[curFunc][varName + "#"] = 0;
    //     }
    //     return retType::INT;
    // }

    // if (varTable["globl"].count(varName) == 0)
    // {
    //     std::cerr << "[ERROR] Using: Variable " << varName << " have not been defined\n";
    //     exit(1);
    // }
    // return retType::INT;
}

antlrcpp::Any StackAlloc::visitForLoop(MiniDecafParser::ForLoopContext *ctx) {
    stmtID++;
    curFunc += "_" + std::to_string(blockID) + std::to_string(stmtID);
    visitChildren(ctx);
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

//@brief: Most of the work is type checking
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
        } 
        else 
        {
            type = std::make_shared<IntptrType>(starNum+1);
        }
    }
    return type;
}

antlrcpp::Any StackAlloc::visitCast(MiniDecafParser::CastContext *ctx) {
    std::shared_ptr<Type> dst = visit(ctx->type());
    std::shared_ptr<Type> src = visit(ctx->unary());
    return dst.get()->typeCast(0);
}

antlrcpp::Any StackAlloc::visitFuncCall(MiniDecafParser::FuncCallContext *ctx) {
    // std::string funcName = ctx->Identifier()->getText();
    // if (funcParaTable[funcName] != ctx->expr().size())
    // {
    //     std::cerr << funcName << " " << funcParaTable[funcName] << " " << ctx->expr().size() << '\n';
    //     exit(1);
    // }
    // if (varTable.count(funcName) == 0) 
    // {
    //     std::cerr << "[ERROR] Using: function " << curFunc << " have not been defined\n";
    //     exit(1);
    // }
    // return retType::UNDEF;
    std::string funcName = ctx->Identifier()->getText();
    if (GlobStruct::getInstance().funcTable.count(funcName) == 0) 
    {
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Use of undeclared function " << funcName << "\n";
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
    visitChildren(ctx);
    return GlobStruct::getInstance().funcTable[funcName].get()->getRetType();
}

antlrcpp::Any StackAlloc::visitParen(MiniDecafParser::ParenContext *ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any StackAlloc::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::string tmp = curFunc;

    // for (int i = 0; i <= stmtID; i++) 
    // {
    //     if (varTable[tmp].count(varName) == 0) 
    //     {
    //         int pos = tmp.rfind('_');
    //         tmp = tmp.substr(0, pos);
    //         continue;
    //     }
    
    //     if (varTable[curFunc].count(varName) == 0) 
    //     {
    //         varTable[curFunc][varName + "#"] = 0;
    //     }
    //     return retType::INT;
    // }

    // if (varTable["globl"].count(varName) == 0)
    // {
    //     std::cerr << "[ERROR] Using: Variable " << varName << " have not been defined\n";
    //     exit(1);
    // }
    // return retType::INT;
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


