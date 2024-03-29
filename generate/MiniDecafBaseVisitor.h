
// Generated from MiniDecaf.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "MiniDecafVisitor.h"


/**
 * This class provides an empty implementation of MiniDecafVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  MiniDecafBaseVisitor : public MiniDecafVisitor {
public:

  virtual antlrcpp::Any visitProg(MiniDecafParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunc(MiniDecafParser::FuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlockItem(MiniDecafParser::BlockItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGlobal(MiniDecafParser::GlobalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGlobalArry(MiniDecafParser::GlobalArryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarDefine(MiniDecafParser::VarDefineContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLocalArry(MiniDecafParser::LocalArryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRetStmt(MiniDecafParser::RetStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprStmt(MiniDecafParser::ExprStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfElse(MiniDecafParser::IfElseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlock(MiniDecafParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitForLoop(MiniDecafParser::ForLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhileLoop(MiniDecafParser::WhileLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDoWhile(MiniDecafParser::DoWhileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBreak(MiniDecafParser::BreakContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContinue(MiniDecafParser::ContinueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSemicolon(MiniDecafParser::SemicolonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssign(MiniDecafParser::AssignContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCond(MiniDecafParser::CondContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCondExpr(MiniDecafParser::CondExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLor_op(MiniDecafParser::Lor_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLand_op(MiniDecafParser::Land_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLor(MiniDecafParser::LorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLand(MiniDecafParser::LandContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEqual_op(MiniDecafParser::Equal_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEqual(MiniDecafParser::EqualContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRela_op(MiniDecafParser::Rela_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCompare(MiniDecafParser::CompareContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdd_op(MiniDecafParser::Add_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAddSub(MiniDecafParser::AddSubContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMul_op(MiniDecafParser::Mul_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_op(MiniDecafParser::Unary_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMulDiv(MiniDecafParser::MulDivContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryOp(MiniDecafParser::UnaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCast(MiniDecafParser::CastContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPostfix_op(MiniDecafParser::Postfix_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArryIndex(MiniDecafParser::ArryIndexContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncCall(MiniDecafParser::FuncCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimary_op(MiniDecafParser::Primary_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParen(MiniDecafParser::ParenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdentifier(MiniDecafParser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInteger(MiniDecafParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitType(MiniDecafParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }


};

