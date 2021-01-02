
// Generated from MiniDecaf.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "MiniDecafParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by MiniDecafParser.
 */
class  MiniDecafVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by MiniDecafParser.
   */
    virtual antlrcpp::Any visitProg(MiniDecafParser::ProgContext *context) = 0;

    virtual antlrcpp::Any visitFunc(MiniDecafParser::FuncContext *context) = 0;

    virtual antlrcpp::Any visitStmt(MiniDecafParser::StmtContext *context) = 0;

    virtual antlrcpp::Any visitExpr(MiniDecafParser::ExprContext *context) = 0;

    virtual antlrcpp::Any visitNum(MiniDecafParser::NumContext *context) = 0;


};

