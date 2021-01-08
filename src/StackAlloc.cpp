#include "StackAlloc.h"
#include "set"

//Visit the root node of AST
antlrcpp::Any StackAlloc::visitProg(MiniDecafParser::ProgContext *ctx, varTab& varID) {
    offset = 0;
    // std::cout << "d" << std::endl;
    visitChildren(ctx);
    // std::cout << "dd" << std::endl;
    // varID["a"] = 2;
    return varTable;
}

antlrcpp::Any StackAlloc::visitFunc(MiniDecafParser::FuncContext *ctx) {
   
    curFunc = ctx->Identifier(0)->getText();
    blockID = 0;
    stmtID = 0;
    offset = 0;
   
    if (ctx->Semicolon()) 
    {
        for (auto i = 1; i < ctx->Identifier().size(); ++i) 
        {
            // std::string varName = ctx->Identifier(i)->getText();
            // std::cout << "name: " << curFunc << " " << funcTable[curFunc]++ << '\n';
            funcTable[curFunc]++;
        }
        
        varTable[curFunc]["$"] = 0;
    }
    else
    {
        if (varTable.count(curFunc) > 0 && varTable[curFunc]["$"] == 1) 
        {
            std::cerr << "[ERROR] function redefined " << curFunc << "\n";
            exit(1);
        } 
        else 
        {
            varTable[curFunc]["$"] = 1;
            if (funcTable.count(curFunc) > 0)
            {
                if (funcTable[curFunc] != ctx->Identifier().size() - 1)
                {
                    std::cerr << "[ERROR] Conflict decl and def \n";
                    exit(1);
                } 
            }
            else
            {
                for (auto i = 1; i < ctx->Identifier().size(); ++i) 
                {
                    funcTable[curFunc]++;
                }
            }
            
            std::set<std::string> tmpVar;
            for (auto i = 1; i < ctx->Identifier().size(); ++i) 
            {
                std::string varName = ctx->Identifier(i)->getText();
                if (tmpVar.count(varName) > 0)
                {
                    exit(1);
                }
                tmpVar.insert(varName);
                varTable[curFunc][varName] = offset++;
            }
            visitChildren(ctx);
        }
    } 
    return retType::INT;
}

antlrcpp::Any StackAlloc::visitFuncCall(MiniDecafParser::FuncCallContext *ctx) {
    std::string funcName = ctx->Identifier()->getText();
    if (funcTable[funcName] != ctx->expr().size())
    {
        std::cerr << funcName << " " << funcTable[funcName] << " " << ctx->expr().size() << '\n';
        exit(1);
    }
    if (varTable.count(funcName) == 0) 
    {
        std::cerr << "[ERROR] Using: function " << curFunc << " have not been defined\n";
        exit(1);
    }
    return retType::UNDEF;
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
    return retType::UNDEF;
}

antlrcpp::Any StackAlloc::visitVarDefine(MiniDecafParser::VarDefineContext *ctx) {
    // allocate the defined varible
    std::string varName = ctx->Identifier()->getText();
    varID[varName]++;
   
   
    if (varTable[curFunc].count(varName) != 0) 
    {
        std::cerr << "[ERROR] Variable Redifined" << varName << "\n";
        exit(1);
    }

    if (varTable[curFunc].count(varName + "#") > 0) 
    {
        varTable[curFunc][varName + "#"] =  varID[varName];
        // varTable[curFunc][varName + "#"] = ctx->start->getLine();
    }
    
    // std::cout << "varTable: " << curFunc << " " << varName << " " << varTable[curFunc][varName] << '\n';
    varTable[curFunc][varName] = offset++;
    // std::cout << "varTable: " << curFunc << " " << varName << " " << varTable[curFunc][varName] << '\n';

    return retType::INT;
}

antlrcpp::Any StackAlloc::visitIdentifier(MiniDecafParser::IdentifierContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    std::string tmp = curFunc;

    for (int i = 0; i <= stmtID; i++) 
    {
        if (varTable[tmp].count(varName) == 0) 
        {
            int pos = tmp.rfind('_');
            tmp = tmp.substr(0, pos);
            continue;
        }
    
        if (varTable[curFunc].count(varName) == 0) 
        {
            varTable[curFunc][varName + "#"] = 0;
        }
        return retType::INT;
    }
    std::cerr << "[ERROR] Using: Variable " << varName << " have not been defined\n";
    exit(1);
}

antlrcpp::Any StackAlloc::visitAssign(MiniDecafParser::AssignContext *ctx) {
    std::string varName = ctx->Identifier()->getText();
    varID[varName]++;
   

    std::string tmpFunc = curFunc;
    // Search the symbol table from inner scope to the outer, for the target symbol
    for (int i = 0; i <= stmtID; i++) 
    {
        if (varTable[tmpFunc].count(varName) == 0) 
        {
            int pos = tmpFunc.rfind('_');
            tmpFunc = tmpFunc.substr(0, pos);
            continue;
        }
        if (varTable[curFunc].count(varName) == 0) 
        {
            varTable[curFunc][varName + "#"] = 0;
        }
        return retType::INT;
    }
    std::cerr << "[ERROR] Assign: Variable " << varName << " have not been defined\n";
    exit(1);
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
    return retType::UNDEF;
}