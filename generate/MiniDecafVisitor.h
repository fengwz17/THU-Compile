
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

    virtual antlrcpp::Any visitRetStmt(MiniDecafParser::RetStmtContext *context) = 0;

    virtual antlrcpp::Any visitParen(MiniDecafParser::ParenContext *context) = 0;

    virtual antlrcpp::Any visitAddSub(MiniDecafParser::AddSubContext *context) = 0;

    virtual antlrcpp::Any visitUnary(MiniDecafParser::UnaryContext *context) = 0;

    virtual antlrcpp::Any visitInteger(MiniDecafParser::IntegerContext *context) = 0;

    virtual antlrcpp::Any visitMulDiv(MiniDecafParser::MulDivContext *context) = 0;


};

