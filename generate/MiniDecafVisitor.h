
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

    virtual antlrcpp::Any visitBlockItem(MiniDecafParser::BlockItemContext *context) = 0;

    virtual antlrcpp::Any visitGlobal(MiniDecafParser::GlobalContext *context) = 0;

    virtual antlrcpp::Any visitGlobalArry(MiniDecafParser::GlobalArryContext *context) = 0;

    virtual antlrcpp::Any visitVarDefine(MiniDecafParser::VarDefineContext *context) = 0;

    virtual antlrcpp::Any visitLocalArry(MiniDecafParser::LocalArryContext *context) = 0;

    virtual antlrcpp::Any visitRetStmt(MiniDecafParser::RetStmtContext *context) = 0;

    virtual antlrcpp::Any visitExprStmt(MiniDecafParser::ExprStmtContext *context) = 0;

    virtual antlrcpp::Any visitIfElse(MiniDecafParser::IfElseContext *context) = 0;

    virtual antlrcpp::Any visitBlock(MiniDecafParser::BlockContext *context) = 0;

    virtual antlrcpp::Any visitForLoop(MiniDecafParser::ForLoopContext *context) = 0;

    virtual antlrcpp::Any visitWhileLoop(MiniDecafParser::WhileLoopContext *context) = 0;

    virtual antlrcpp::Any visitDoWhile(MiniDecafParser::DoWhileContext *context) = 0;

    virtual antlrcpp::Any visitBreak(MiniDecafParser::BreakContext *context) = 0;

    virtual antlrcpp::Any visitContinue(MiniDecafParser::ContinueContext *context) = 0;

    virtual antlrcpp::Any visitSemicolon(MiniDecafParser::SemicolonContext *context) = 0;

    virtual antlrcpp::Any visitAssign(MiniDecafParser::AssignContext *context) = 0;

    virtual antlrcpp::Any visitCond(MiniDecafParser::CondContext *context) = 0;

    virtual antlrcpp::Any visitCondExpr(MiniDecafParser::CondExprContext *context) = 0;

    virtual antlrcpp::Any visitLor_op(MiniDecafParser::Lor_opContext *context) = 0;

    virtual antlrcpp::Any visitLand_op(MiniDecafParser::Land_opContext *context) = 0;

    virtual antlrcpp::Any visitLor(MiniDecafParser::LorContext *context) = 0;

    virtual antlrcpp::Any visitLand(MiniDecafParser::LandContext *context) = 0;

    virtual antlrcpp::Any visitEqual_op(MiniDecafParser::Equal_opContext *context) = 0;

    virtual antlrcpp::Any visitEqual(MiniDecafParser::EqualContext *context) = 0;

    virtual antlrcpp::Any visitRela_op(MiniDecafParser::Rela_opContext *context) = 0;

    virtual antlrcpp::Any visitCompare(MiniDecafParser::CompareContext *context) = 0;

    virtual antlrcpp::Any visitAdd_op(MiniDecafParser::Add_opContext *context) = 0;

    virtual antlrcpp::Any visitAddSub(MiniDecafParser::AddSubContext *context) = 0;

    virtual antlrcpp::Any visitMul_op(MiniDecafParser::Mul_opContext *context) = 0;

    virtual antlrcpp::Any visitUnary_op(MiniDecafParser::Unary_opContext *context) = 0;

    virtual antlrcpp::Any visitMulDiv(MiniDecafParser::MulDivContext *context) = 0;

    virtual antlrcpp::Any visitUnaryOp(MiniDecafParser::UnaryOpContext *context) = 0;

    virtual antlrcpp::Any visitCast(MiniDecafParser::CastContext *context) = 0;

    virtual antlrcpp::Any visitPostfix_op(MiniDecafParser::Postfix_opContext *context) = 0;

    virtual antlrcpp::Any visitArryIndex(MiniDecafParser::ArryIndexContext *context) = 0;

    virtual antlrcpp::Any visitFuncCall(MiniDecafParser::FuncCallContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_op(MiniDecafParser::Primary_opContext *context) = 0;

    virtual antlrcpp::Any visitParen(MiniDecafParser::ParenContext *context) = 0;

    virtual antlrcpp::Any visitIdentifier(MiniDecafParser::IdentifierContext *context) = 0;

    virtual antlrcpp::Any visitInteger(MiniDecafParser::IntegerContext *context) = 0;

    virtual antlrcpp::Any visitType(MiniDecafParser::TypeContext *context) = 0;


};

