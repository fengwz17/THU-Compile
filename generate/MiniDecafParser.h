
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
    Multiplication = 25, AND = 26, Division = 27, Modulo = 28, LAND = 29, 
    LOR = 30, EQ = 31, NEQ = 32, LT = 33, LE = 34, GT = 35, GE = 36, Integer = 37, 
    Identifier = 38, WS = 39
  };

  enum {
    RuleProg = 0, RuleFunc = 1, RuleBlockItem = 2, RuleGlobalDecl = 3, RuleDeclaration = 4, 
    RuleStmt = 5, RuleExpr = 6, RuleConditional = 7, RuleLogical_or = 8, 
    RuleLogical_and = 9, RuleEquality = 10, RuleRelational = 11, RuleAdd = 12, 
    RuleMul = 13, RuleUnary = 14, RulePostfix = 15, RulePrimary = 16, RuleType = 17
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
  class BlockItemContext;
  class GlobalDeclContext;
  class DeclarationContext;
  class StmtContext;
  class ExprContext;
  class ConditionalContext;
  class Logical_orContext;
  class Logical_andContext;
  class EqualityContext;
  class RelationalContext;
  class AddContext;
  class MulContext;
  class UnaryContext;
  class PostfixContext;
  class PrimaryContext;
  class TypeContext; 

  class  ProgContext : public antlr4::ParserRuleContext {
  public:
    ProgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<FuncContext *> func();
    FuncContext* func(size_t i);
    std::vector<GlobalDeclContext *> globalDecl();
    GlobalDeclContext* globalDecl(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Semicolon();
    antlr4::tree::TerminalNode* Semicolon(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgContext* prog();

  class  FuncContext : public antlr4::ParserRuleContext {
  public:
    FuncContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TypeContext *> type();
    TypeContext* type(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);
    antlr4::tree::TerminalNode *Lparen();
    antlr4::tree::TerminalNode *Rparen();
    antlr4::tree::TerminalNode *Lbrace();
    antlr4::tree::TerminalNode *Rbrace();
    antlr4::tree::TerminalNode *Semicolon();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);
    std::vector<BlockItemContext *> blockItem();
    BlockItemContext* blockItem(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FuncContext* func();

  class  BlockItemContext : public antlr4::ParserRuleContext {
  public:
    BlockItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StmtContext *stmt();
    DeclarationContext *declaration();
    antlr4::tree::TerminalNode *Semicolon();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockItemContext* blockItem();

  class  GlobalDeclContext : public antlr4::ParserRuleContext {
  public:
    GlobalDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    GlobalDeclContext() = default;
    void copyFrom(GlobalDeclContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GlobalContext : public GlobalDeclContext {
  public:
    GlobalContext(GlobalDeclContext *ctx);

    TypeContext *type();
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Integer();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GlobalArryContext : public GlobalDeclContext {
  public:
    GlobalArryContext(GlobalDeclContext *ctx);

    TypeContext *type();
    antlr4::tree::TerminalNode *Identifier();
    std::vector<antlr4::tree::TerminalNode *> Lbrkt();
    antlr4::tree::TerminalNode* Lbrkt(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Integer();
    antlr4::tree::TerminalNode* Integer(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Rbrkt();
    antlr4::tree::TerminalNode* Rbrkt(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  GlobalDeclContext* globalDecl();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    DeclarationContext() = default;
    void copyFrom(DeclarationContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  LocalArryContext : public DeclarationContext {
  public:
    LocalArryContext(DeclarationContext *ctx);

    TypeContext *type();
    antlr4::tree::TerminalNode *Identifier();
    std::vector<antlr4::tree::TerminalNode *> Lbrkt();
    antlr4::tree::TerminalNode* Lbrkt(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Integer();
    antlr4::tree::TerminalNode* Integer(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Rbrkt();
    antlr4::tree::TerminalNode* Rbrkt(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VarDefineContext : public DeclarationContext {
  public:
    VarDefineContext(DeclarationContext *ctx);

    TypeContext *type();
    antlr4::tree::TerminalNode *Identifier();
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  DeclarationContext* declaration();

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

  class  ExprStmtContext : public StmtContext {
  public:
    ExprStmtContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *Semicolon();
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  WhileLoopContext : public StmtContext {
  public:
    WhileLoopContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *While();
    antlr4::tree::TerminalNode *Lparen();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Rparen();
    StmtContext *stmt();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DoWhileContext : public StmtContext {
  public:
    DoWhileContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *Do();
    StmtContext *stmt();
    antlr4::tree::TerminalNode *While();
    antlr4::tree::TerminalNode *Lparen();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Rparen();
    antlr4::tree::TerminalNode *Semicolon();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BreakContext : public StmtContext {
  public:
    BreakContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *Break();
    antlr4::tree::TerminalNode *Semicolon();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ContinueContext : public StmtContext {
  public:
    ContinueContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *Continue();
    antlr4::tree::TerminalNode *Semicolon();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BlockContext : public StmtContext {
  public:
    BlockContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *Lbrace();
    antlr4::tree::TerminalNode *Rbrace();
    std::vector<BlockItemContext *> blockItem();
    BlockItemContext* blockItem(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IfElseContext : public StmtContext {
  public:
    IfElseContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *If();
    antlr4::tree::TerminalNode *Lparen();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Rparen();
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);
    antlr4::tree::TerminalNode *Else();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ForLoopContext : public StmtContext {
  public:
    ForLoopContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *For();
    antlr4::tree::TerminalNode *Lparen();
    std::vector<antlr4::tree::TerminalNode *> Semicolon();
    antlr4::tree::TerminalNode* Semicolon(size_t i);
    antlr4::tree::TerminalNode *Rparen();
    StmtContext *stmt();
    DeclarationContext *declaration();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SemicolonContext : public StmtContext {
  public:
    SemicolonContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *Semicolon();

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

  class  CondContext : public ExprContext {
  public:
    CondContext(ExprContext *ctx);

    ConditionalContext *conditional();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AssignContext : public ExprContext {
  public:
    AssignContext(ExprContext *ctx);

    UnaryContext *unary();
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();

  class  ConditionalContext : public antlr4::ParserRuleContext {
  public:
    ConditionalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ConditionalContext() = default;
    void copyFrom(ConditionalContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CondExprContext : public ConditionalContext {
  public:
    CondExprContext(ConditionalContext *ctx);

    Logical_orContext *logical_or();
    antlr4::tree::TerminalNode *Question();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Colon();
    ConditionalContext *conditional();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Lor_opContext : public ConditionalContext {
  public:
    Lor_opContext(ConditionalContext *ctx);

    Logical_orContext *logical_or();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ConditionalContext* conditional();

  class  Logical_orContext : public antlr4::ParserRuleContext {
  public:
    Logical_orContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Logical_orContext() = default;
    void copyFrom(Logical_orContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Land_opContext : public Logical_orContext {
  public:
    Land_opContext(Logical_orContext *ctx);

    Logical_andContext *logical_and();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LorContext : public Logical_orContext {
  public:
    LorContext(Logical_orContext *ctx);

    std::vector<Logical_orContext *> logical_or();
    Logical_orContext* logical_or(size_t i);
    antlr4::tree::TerminalNode *LOR();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Logical_orContext* logical_or();
  Logical_orContext* logical_or(int precedence);
  class  Logical_andContext : public antlr4::ParserRuleContext {
  public:
    Logical_andContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Logical_andContext() = default;
    void copyFrom(Logical_andContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  LandContext : public Logical_andContext {
  public:
    LandContext(Logical_andContext *ctx);

    std::vector<Logical_andContext *> logical_and();
    Logical_andContext* logical_and(size_t i);
    antlr4::tree::TerminalNode *LAND();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equal_opContext : public Logical_andContext {
  public:
    Equal_opContext(Logical_andContext *ctx);

    EqualityContext *equality();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Logical_andContext* logical_and();
  Logical_andContext* logical_and(int precedence);
  class  EqualityContext : public antlr4::ParserRuleContext {
  public:
    EqualityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    EqualityContext() = default;
    void copyFrom(EqualityContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  EqualContext : public EqualityContext {
  public:
    EqualContext(EqualityContext *ctx);

    std::vector<EqualityContext *> equality();
    EqualityContext* equality(size_t i);
    antlr4::tree::TerminalNode *EQ();
    antlr4::tree::TerminalNode *NEQ();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Rela_opContext : public EqualityContext {
  public:
    Rela_opContext(EqualityContext *ctx);

    RelationalContext *relational();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  EqualityContext* equality();
  EqualityContext* equality(int precedence);
  class  RelationalContext : public antlr4::ParserRuleContext {
  public:
    RelationalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    RelationalContext() = default;
    void copyFrom(RelationalContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CompareContext : public RelationalContext {
  public:
    CompareContext(RelationalContext *ctx);

    std::vector<RelationalContext *> relational();
    RelationalContext* relational(size_t i);
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *LE();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *GE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Add_opContext : public RelationalContext {
  public:
    Add_opContext(RelationalContext *ctx);

    AddContext *add();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  RelationalContext* relational();
  RelationalContext* relational(int precedence);
  class  AddContext : public antlr4::ParserRuleContext {
  public:
    AddContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AddContext() = default;
    void copyFrom(AddContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  AddSubContext : public AddContext {
  public:
    AddSubContext(AddContext *ctx);

    std::vector<AddContext *> add();
    AddContext* add(size_t i);
    antlr4::tree::TerminalNode *Addition();
    antlr4::tree::TerminalNode *Minus();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Mul_opContext : public AddContext {
  public:
    Mul_opContext(AddContext *ctx);

    MulContext *mul();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AddContext* add();
  AddContext* add(int precedence);
  class  MulContext : public antlr4::ParserRuleContext {
  public:
    MulContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    MulContext() = default;
    void copyFrom(MulContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Unary_opContext : public MulContext {
  public:
    Unary_opContext(MulContext *ctx);

    UnaryContext *unary();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MulDivContext : public MulContext {
  public:
    MulDivContext(MulContext *ctx);

    std::vector<MulContext *> mul();
    MulContext* mul(size_t i);
    antlr4::tree::TerminalNode *Multiplication();
    antlr4::tree::TerminalNode *Division();
    antlr4::tree::TerminalNode *Modulo();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  MulContext* mul();
  MulContext* mul(int precedence);
  class  UnaryContext : public antlr4::ParserRuleContext {
  public:
    UnaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    UnaryContext() = default;
    void copyFrom(UnaryContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CastContext : public UnaryContext {
  public:
    CastContext(UnaryContext *ctx);

    antlr4::tree::TerminalNode *Lparen();
    TypeContext *type();
    antlr4::tree::TerminalNode *Rparen();
    UnaryContext *unary();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UnaryOpContext : public UnaryContext {
  public:
    UnaryOpContext(UnaryContext *ctx);

    UnaryContext *unary();
    antlr4::tree::TerminalNode *Exclamation();
    antlr4::tree::TerminalNode *Tilde();
    antlr4::tree::TerminalNode *Minus();
    antlr4::tree::TerminalNode *Multiplication();
    antlr4::tree::TerminalNode *AND();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Postfix_opContext : public UnaryContext {
  public:
    Postfix_opContext(UnaryContext *ctx);

    PostfixContext *postfix();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  UnaryContext* unary();

  class  PostfixContext : public antlr4::ParserRuleContext {
  public:
    PostfixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PostfixContext() = default;
    void copyFrom(PostfixContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ArryIndexContext : public PostfixContext {
  public:
    ArryIndexContext(PostfixContext *ctx);

    PostfixContext *postfix();
    antlr4::tree::TerminalNode *Lbrkt();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Rbrkt();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FuncCallContext : public PostfixContext {
  public:
    FuncCallContext(PostfixContext *ctx);

    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Lparen();
    antlr4::tree::TerminalNode *Rparen();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_opContext : public PostfixContext {
  public:
    Primary_opContext(PostfixContext *ctx);

    PrimaryContext *primary();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PostfixContext* postfix();
  PostfixContext* postfix(int precedence);
  class  PrimaryContext : public antlr4::ParserRuleContext {
  public:
    PrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PrimaryContext() = default;
    void copyFrom(PrimaryContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  IdentifierContext : public PrimaryContext {
  public:
    IdentifierContext(PrimaryContext *ctx);

    antlr4::tree::TerminalNode *Identifier();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParenContext : public PrimaryContext {
  public:
    ParenContext(PrimaryContext *ctx);

    antlr4::tree::TerminalNode *Lparen();
    ExprContext *expr();
    antlr4::tree::TerminalNode *Rparen();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IntegerContext : public PrimaryContext {
  public:
    IntegerContext(PrimaryContext *ctx);

    antlr4::tree::TerminalNode *Integer();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PrimaryContext* primary();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Int();
    std::vector<antlr4::tree::TerminalNode *> Multiplication();
    antlr4::tree::TerminalNode* Multiplication(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeContext* type();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool logical_orSempred(Logical_orContext *_localctx, size_t predicateIndex);
  bool logical_andSempred(Logical_andContext *_localctx, size_t predicateIndex);
  bool equalitySempred(EqualityContext *_localctx, size_t predicateIndex);
  bool relationalSempred(RelationalContext *_localctx, size_t predicateIndex);
  bool addSempred(AddContext *_localctx, size_t predicateIndex);
  bool mulSempred(MulContext *_localctx, size_t predicateIndex);
  bool postfixSempred(PostfixContext *_localctx, size_t predicateIndex);

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

