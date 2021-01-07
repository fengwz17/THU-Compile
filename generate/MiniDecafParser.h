
// Generated from MiniDecaf.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  MiniDecafParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, Int = 2, Return = 3, If = 4, Else = 5, For = 6, While = 7, 
    Do = 8, Break = 9, Continue = 10, Lparen = 11, Rparen = 12, Lbrkt = 13, 
    Rbrkt = 14, Lbrace = 15, Rbrace = 16, Comma = 17, Semicolon = 18, Question = 19, 
    Colon = 20, Minus = 21, Exclamation = 22, Tilde = 23, Addition = 24, 
    Multiplication = 25, Division = 26, Modulo = 27, LAND = 28, LOR = 29, 
    EQ = 30, NEQ = 31, LT = 32, LE = 33, GT = 34, GE = 35, Integer = 36, 
    Identifier = 37, WS = 38
  };

  enum {
    RuleProg = 0, RuleFunc = 1, RuleStmt = 2, RuleExpr = 3, RuleType = 4
  };

  MiniDecafParser(antlr4::TokenStream *input);
  ~MiniDecafParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ProgContext;
  class FuncContext;
  class StmtContext;
  class ExprContext;
  class TypeContext; 

  class  ProgContext : public antlr4::ParserRuleContext {
  public:
    ProgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FuncContext *func();
    antlr4::tree::TerminalNode *EOF();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgContext* prog();

  class  FuncContext : public antlr4::ParserRuleContext {
  public:
    FuncContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Lparen();
    antlr4::tree::TerminalNode *Rparen();
    antlr4::tree::TerminalNode *Lbrace();
    antlr4::tree::TerminalNode *Rbrace();
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FuncContext* func();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    StmtContext() = default;
    void copyFrom(StmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  RetStmtContext : public StmtContext {
  public:
    RetStmtContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *Return();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Semicolon();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MExprContext : public StmtContext {
  public:
    MExprContext(StmtContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *Semicolon();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VarDefineContext : public StmtContext {
  public:
    VarDefineContext(StmtContext *ctx);

    TypeContext *type();
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Semicolon();
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  StmtContext* stmt();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExprContext() = default;
    void copyFrom(ExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  EqualContext : public ExprContext {
  public:
    EqualContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParenContext : public ExprContext {
  public:
    ParenContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *Lparen();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Rparen();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CompareContext : public ExprContext {
  public:
    CompareContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *GE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IdentifierContext : public ExprContext {
  public:
    IdentifierContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *Identifier();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LandContext : public ExprContext {
  public:
    LandContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LAND();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AddSubContext : public ExprContext {
  public:
    AddSubContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *Addition();
    antlr4::tree::TerminalNode *Minus();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UnaryContext : public ExprContext {
  public:
    UnaryContext(ExprContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *Minus();
    antlr4::tree::TerminalNode *Exclamation();
    antlr4::tree::TerminalNode *Tilde();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IntegerContext : public ExprContext {
  public:
    IntegerContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *Integer();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MulDivContext : public ExprContext {
  public:
    MulDivContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *Multiplication();
    antlr4::tree::TerminalNode *Division();
    antlr4::tree::TerminalNode *Modulo();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LorContext : public ExprContext {
  public:
    LorContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LOR();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AssignContext : public ExprContext {
  public:
    AssignContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *Identifier();
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Int();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeContext* type();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

