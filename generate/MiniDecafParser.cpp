
// Generated from MiniDecaf.g4 by ANTLR 4.8


#include "MiniDecafVisitor.h"

#include "MiniDecafParser.h"


using namespace antlrcpp;
using namespace antlr4;

MiniDecafParser::MiniDecafParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

MiniDecafParser::~MiniDecafParser() {
  delete _interpreter;
}

std::string MiniDecafParser::getGrammarFileName() const {
  return "MiniDecaf.g4";
}

const std::vector<std::string>& MiniDecafParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& MiniDecafParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgContext ------------------------------------------------------------------

MiniDecafParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniDecafParser::FuncContext* MiniDecafParser::ProgContext::func() {
  return getRuleContext<MiniDecafParser::FuncContext>(0);
}

tree::TerminalNode* MiniDecafParser::ProgContext::EOF() {
  return getToken(MiniDecafParser::EOF, 0);
}


size_t MiniDecafParser::ProgContext::getRuleIndex() const {
  return MiniDecafParser::RuleProg;
}


antlrcpp::Any MiniDecafParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitProg(this);
  else
    return visitor->visitChildren(this);
}

MiniDecafParser::ProgContext* MiniDecafParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 0, MiniDecafParser::RuleProg);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(10);
    func();
    setState(11);
    match(MiniDecafParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncContext ------------------------------------------------------------------

MiniDecafParser::FuncContext::FuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniDecafParser::TypeContext* MiniDecafParser::FuncContext::type() {
  return getRuleContext<MiniDecafParser::TypeContext>(0);
}

tree::TerminalNode* MiniDecafParser::FuncContext::Identifier() {
  return getToken(MiniDecafParser::Identifier, 0);
}

tree::TerminalNode* MiniDecafParser::FuncContext::Lparen() {
  return getToken(MiniDecafParser::Lparen, 0);
}

tree::TerminalNode* MiniDecafParser::FuncContext::Rparen() {
  return getToken(MiniDecafParser::Rparen, 0);
}

tree::TerminalNode* MiniDecafParser::FuncContext::Lbrace() {
  return getToken(MiniDecafParser::Lbrace, 0);
}

tree::TerminalNode* MiniDecafParser::FuncContext::Rbrace() {
  return getToken(MiniDecafParser::Rbrace, 0);
}

std::vector<MiniDecafParser::StmtContext *> MiniDecafParser::FuncContext::stmt() {
  return getRuleContexts<MiniDecafParser::StmtContext>();
}

MiniDecafParser::StmtContext* MiniDecafParser::FuncContext::stmt(size_t i) {
  return getRuleContext<MiniDecafParser::StmtContext>(i);
}


size_t MiniDecafParser::FuncContext::getRuleIndex() const {
  return MiniDecafParser::RuleFunc;
}


antlrcpp::Any MiniDecafParser::FuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitFunc(this);
  else
    return visitor->visitChildren(this);
}

MiniDecafParser::FuncContext* MiniDecafParser::func() {
  FuncContext *_localctx = _tracker.createInstance<FuncContext>(_ctx, getState());
  enterRule(_localctx, 2, MiniDecafParser::RuleFunc);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(13);
    type();
    setState(14);
    match(MiniDecafParser::Identifier);
    setState(15);
    match(MiniDecafParser::Lparen);
    setState(16);
    match(MiniDecafParser::Rparen);
    setState(17);
    match(MiniDecafParser::Lbrace);
    setState(21);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MiniDecafParser::Int)
      | (1ULL << MiniDecafParser::Return)
      | (1ULL << MiniDecafParser::Lparen)
      | (1ULL << MiniDecafParser::Minus)
      | (1ULL << MiniDecafParser::Exclamation)
      | (1ULL << MiniDecafParser::Tilde)
      | (1ULL << MiniDecafParser::Integer)
      | (1ULL << MiniDecafParser::Identifier))) != 0)) {
      setState(18);
      stmt();
      setState(23);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(24);
    match(MiniDecafParser::Rbrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

MiniDecafParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniDecafParser::StmtContext::getRuleIndex() const {
  return MiniDecafParser::RuleStmt;
}

void MiniDecafParser::StmtContext::copyFrom(StmtContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- RetStmtContext ------------------------------------------------------------------

tree::TerminalNode* MiniDecafParser::RetStmtContext::Return() {
  return getToken(MiniDecafParser::Return, 0);
}

MiniDecafParser::ExprContext* MiniDecafParser::RetStmtContext::expr() {
  return getRuleContext<MiniDecafParser::ExprContext>(0);
}

tree::TerminalNode* MiniDecafParser::RetStmtContext::Semicolon() {
  return getToken(MiniDecafParser::Semicolon, 0);
}

MiniDecafParser::RetStmtContext::RetStmtContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::RetStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitRetStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MExprContext ------------------------------------------------------------------

MiniDecafParser::ExprContext* MiniDecafParser::MExprContext::expr() {
  return getRuleContext<MiniDecafParser::ExprContext>(0);
}

tree::TerminalNode* MiniDecafParser::MExprContext::Semicolon() {
  return getToken(MiniDecafParser::Semicolon, 0);
}

MiniDecafParser::MExprContext::MExprContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::MExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitMExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VarDefineContext ------------------------------------------------------------------

MiniDecafParser::TypeContext* MiniDecafParser::VarDefineContext::type() {
  return getRuleContext<MiniDecafParser::TypeContext>(0);
}

tree::TerminalNode* MiniDecafParser::VarDefineContext::Identifier() {
  return getToken(MiniDecafParser::Identifier, 0);
}

tree::TerminalNode* MiniDecafParser::VarDefineContext::Semicolon() {
  return getToken(MiniDecafParser::Semicolon, 0);
}

MiniDecafParser::ExprContext* MiniDecafParser::VarDefineContext::expr() {
  return getRuleContext<MiniDecafParser::ExprContext>(0);
}

MiniDecafParser::VarDefineContext::VarDefineContext(StmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::VarDefineContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitVarDefine(this);
  else
    return visitor->visitChildren(this);
}
MiniDecafParser::StmtContext* MiniDecafParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 4, MiniDecafParser::RuleStmt);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(41);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniDecafParser::Return: {
        _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<MiniDecafParser::RetStmtContext>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(26);
        match(MiniDecafParser::Return);
        setState(27);
        expr(0);
        setState(28);
        match(MiniDecafParser::Semicolon);
        break;
      }

      case MiniDecafParser::Lparen:
      case MiniDecafParser::Minus:
      case MiniDecafParser::Exclamation:
      case MiniDecafParser::Tilde:
      case MiniDecafParser::Integer:
      case MiniDecafParser::Identifier: {
        _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<MiniDecafParser::MExprContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(30);
        expr(0);
        setState(31);
        match(MiniDecafParser::Semicolon);
        break;
      }

      case MiniDecafParser::Int: {
        _localctx = dynamic_cast<StmtContext *>(_tracker.createInstance<MiniDecafParser::VarDefineContext>(_localctx));
        enterOuterAlt(_localctx, 3);
        setState(33);
        type();
        setState(34);
        match(MiniDecafParser::Identifier);
        setState(37);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == MiniDecafParser::T__0) {
          setState(35);
          match(MiniDecafParser::T__0);
          setState(36);
          expr(0);
        }
        setState(39);
        match(MiniDecafParser::Semicolon);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

MiniDecafParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniDecafParser::ExprContext::getRuleIndex() const {
  return MiniDecafParser::RuleExpr;
}

void MiniDecafParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- EqualContext ------------------------------------------------------------------

std::vector<MiniDecafParser::ExprContext *> MiniDecafParser::EqualContext::expr() {
  return getRuleContexts<MiniDecafParser::ExprContext>();
}

MiniDecafParser::ExprContext* MiniDecafParser::EqualContext::expr(size_t i) {
  return getRuleContext<MiniDecafParser::ExprContext>(i);
}

tree::TerminalNode* MiniDecafParser::EqualContext::EQ() {
  return getToken(MiniDecafParser::EQ, 0);
}

tree::TerminalNode* MiniDecafParser::EqualContext::NEQ() {
  return getToken(MiniDecafParser::NEQ, 0);
}

MiniDecafParser::EqualContext::EqualContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::EqualContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitEqual(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenContext ------------------------------------------------------------------

tree::TerminalNode* MiniDecafParser::ParenContext::Lparen() {
  return getToken(MiniDecafParser::Lparen, 0);
}

MiniDecafParser::ExprContext* MiniDecafParser::ParenContext::expr() {
  return getRuleContext<MiniDecafParser::ExprContext>(0);
}

tree::TerminalNode* MiniDecafParser::ParenContext::Rparen() {
  return getToken(MiniDecafParser::Rparen, 0);
}

MiniDecafParser::ParenContext::ParenContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::ParenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitParen(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CompareContext ------------------------------------------------------------------

std::vector<MiniDecafParser::ExprContext *> MiniDecafParser::CompareContext::expr() {
  return getRuleContexts<MiniDecafParser::ExprContext>();
}

MiniDecafParser::ExprContext* MiniDecafParser::CompareContext::expr(size_t i) {
  return getRuleContext<MiniDecafParser::ExprContext>(i);
}

tree::TerminalNode* MiniDecafParser::CompareContext::LT() {
  return getToken(MiniDecafParser::LT, 0);
}

tree::TerminalNode* MiniDecafParser::CompareContext::LE() {
  return getToken(MiniDecafParser::LE, 0);
}

tree::TerminalNode* MiniDecafParser::CompareContext::GT() {
  return getToken(MiniDecafParser::GT, 0);
}

tree::TerminalNode* MiniDecafParser::CompareContext::GE() {
  return getToken(MiniDecafParser::GE, 0);
}

MiniDecafParser::CompareContext::CompareContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::CompareContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitCompare(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IdentifierContext ------------------------------------------------------------------

tree::TerminalNode* MiniDecafParser::IdentifierContext::Identifier() {
  return getToken(MiniDecafParser::Identifier, 0);
}

MiniDecafParser::IdentifierContext::IdentifierContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::IdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitIdentifier(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LandContext ------------------------------------------------------------------

std::vector<MiniDecafParser::ExprContext *> MiniDecafParser::LandContext::expr() {
  return getRuleContexts<MiniDecafParser::ExprContext>();
}

MiniDecafParser::ExprContext* MiniDecafParser::LandContext::expr(size_t i) {
  return getRuleContext<MiniDecafParser::ExprContext>(i);
}

tree::TerminalNode* MiniDecafParser::LandContext::LAND() {
  return getToken(MiniDecafParser::LAND, 0);
}

MiniDecafParser::LandContext::LandContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::LandContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitLand(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AddSubContext ------------------------------------------------------------------

std::vector<MiniDecafParser::ExprContext *> MiniDecafParser::AddSubContext::expr() {
  return getRuleContexts<MiniDecafParser::ExprContext>();
}

MiniDecafParser::ExprContext* MiniDecafParser::AddSubContext::expr(size_t i) {
  return getRuleContext<MiniDecafParser::ExprContext>(i);
}

tree::TerminalNode* MiniDecafParser::AddSubContext::Addition() {
  return getToken(MiniDecafParser::Addition, 0);
}

tree::TerminalNode* MiniDecafParser::AddSubContext::Minus() {
  return getToken(MiniDecafParser::Minus, 0);
}

MiniDecafParser::AddSubContext::AddSubContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::AddSubContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitAddSub(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryContext ------------------------------------------------------------------

MiniDecafParser::ExprContext* MiniDecafParser::UnaryContext::expr() {
  return getRuleContext<MiniDecafParser::ExprContext>(0);
}

tree::TerminalNode* MiniDecafParser::UnaryContext::Minus() {
  return getToken(MiniDecafParser::Minus, 0);
}

tree::TerminalNode* MiniDecafParser::UnaryContext::Exclamation() {
  return getToken(MiniDecafParser::Exclamation, 0);
}

tree::TerminalNode* MiniDecafParser::UnaryContext::Tilde() {
  return getToken(MiniDecafParser::Tilde, 0);
}

MiniDecafParser::UnaryContext::UnaryContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::UnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitUnary(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntegerContext ------------------------------------------------------------------

tree::TerminalNode* MiniDecafParser::IntegerContext::Integer() {
  return getToken(MiniDecafParser::Integer, 0);
}

MiniDecafParser::IntegerContext::IntegerContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MulDivContext ------------------------------------------------------------------

std::vector<MiniDecafParser::ExprContext *> MiniDecafParser::MulDivContext::expr() {
  return getRuleContexts<MiniDecafParser::ExprContext>();
}

MiniDecafParser::ExprContext* MiniDecafParser::MulDivContext::expr(size_t i) {
  return getRuleContext<MiniDecafParser::ExprContext>(i);
}

tree::TerminalNode* MiniDecafParser::MulDivContext::Multiplication() {
  return getToken(MiniDecafParser::Multiplication, 0);
}

tree::TerminalNode* MiniDecafParser::MulDivContext::Division() {
  return getToken(MiniDecafParser::Division, 0);
}

tree::TerminalNode* MiniDecafParser::MulDivContext::Modulo() {
  return getToken(MiniDecafParser::Modulo, 0);
}

MiniDecafParser::MulDivContext::MulDivContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::MulDivContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitMulDiv(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LorContext ------------------------------------------------------------------

std::vector<MiniDecafParser::ExprContext *> MiniDecafParser::LorContext::expr() {
  return getRuleContexts<MiniDecafParser::ExprContext>();
}

MiniDecafParser::ExprContext* MiniDecafParser::LorContext::expr(size_t i) {
  return getRuleContext<MiniDecafParser::ExprContext>(i);
}

tree::TerminalNode* MiniDecafParser::LorContext::LOR() {
  return getToken(MiniDecafParser::LOR, 0);
}

MiniDecafParser::LorContext::LorContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::LorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitLor(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignContext ------------------------------------------------------------------

tree::TerminalNode* MiniDecafParser::AssignContext::Identifier() {
  return getToken(MiniDecafParser::Identifier, 0);
}

MiniDecafParser::ExprContext* MiniDecafParser::AssignContext::expr() {
  return getRuleContext<MiniDecafParser::ExprContext>(0);
}

MiniDecafParser::AssignContext::AssignContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any MiniDecafParser::AssignContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitAssign(this);
  else
    return visitor->visitChildren(this);
}

MiniDecafParser::ExprContext* MiniDecafParser::expr() {
   return expr(0);
}

MiniDecafParser::ExprContext* MiniDecafParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  MiniDecafParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  MiniDecafParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 6;
  enterRecursionRule(_localctx, 6, MiniDecafParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(55);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<UnaryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(44);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << MiniDecafParser::Minus)
        | (1ULL << MiniDecafParser::Exclamation)
        | (1ULL << MiniDecafParser::Tilde))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(45);
      expr(11);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ParenContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(46);
      match(MiniDecafParser::Lparen);
      setState(47);
      expr(0);
      setState(48);
      match(MiniDecafParser::Rparen);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<AssignContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(50);
      match(MiniDecafParser::Identifier);
      setState(51);
      match(MiniDecafParser::T__0);
      setState(52);
      expr(3);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<IdentifierContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(53);
      match(MiniDecafParser::Identifier);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<IntegerContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(54);
      match(MiniDecafParser::Integer);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(77);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(75);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MulDivContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(57);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(58);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << MiniDecafParser::Multiplication)
            | (1ULL << MiniDecafParser::Division)
            | (1ULL << MiniDecafParser::Modulo))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(59);
          expr(11);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<CompareContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(60);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(61);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << MiniDecafParser::LT)
            | (1ULL << MiniDecafParser::LE)
            | (1ULL << MiniDecafParser::GT)
            | (1ULL << MiniDecafParser::GE))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(62);
          expr(10);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<EqualContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(63);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(64);
          _la = _input->LA(1);
          if (!(_la == MiniDecafParser::EQ

          || _la == MiniDecafParser::NEQ)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(65);
          expr(9);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LandContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(66);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");

          setState(67);
          match(MiniDecafParser::LAND);
          setState(68);
          expr(8);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LorContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(69);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");

          setState(70);
          match(MiniDecafParser::LOR);
          setState(71);
          expr(7);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<AddSubContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(72);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(73);
          _la = _input->LA(1);
          if (!(_la == MiniDecafParser::Minus

          || _la == MiniDecafParser::Addition)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(74);
          expr(6);
          break;
        }

        } 
      }
      setState(79);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

MiniDecafParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniDecafParser::TypeContext::Int() {
  return getToken(MiniDecafParser::Int, 0);
}


size_t MiniDecafParser::TypeContext::getRuleIndex() const {
  return MiniDecafParser::RuleType;
}


antlrcpp::Any MiniDecafParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniDecafVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

MiniDecafParser::TypeContext* MiniDecafParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 8, MiniDecafParser::RuleType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    match(MiniDecafParser::Int);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool MiniDecafParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 3: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool MiniDecafParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 10);
    case 1: return precpred(_ctx, 9);
    case 2: return precpred(_ctx, 8);
    case 3: return precpred(_ctx, 7);
    case 4: return precpred(_ctx, 6);
    case 5: return precpred(_ctx, 5);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> MiniDecafParser::_decisionToDFA;
atn::PredictionContextCache MiniDecafParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN MiniDecafParser::_atn;
std::vector<uint16_t> MiniDecafParser::_serializedATN;

std::vector<std::string> MiniDecafParser::_ruleNames = {
  "prog", "func", "stmt", "expr", "type"
};

std::vector<std::string> MiniDecafParser::_literalNames = {
  "", "'='", "'int'", "'return'", "'if'", "'else'", "'for'", "'while'", 
  "'do'", "'break'", "'continue'", "'('", "')'", "'['", "']'", "'{'", "'}'", 
  "','", "';'", "'?'", "':'", "'-'", "'!'", "'~'", "'+'", "'*'", "'/'", 
  "'%'", "'&&'", "'||'", "'=='", "'!='", "'<'", "'<='", "'>'", "'>='"
};

std::vector<std::string> MiniDecafParser::_symbolicNames = {
  "", "", "Int", "Return", "If", "Else", "For", "While", "Do", "Break", 
  "Continue", "Lparen", "Rparen", "Lbrkt", "Rbrkt", "Lbrace", "Rbrace", 
  "Comma", "Semicolon", "Question", "Colon", "Minus", "Exclamation", "Tilde", 
  "Addition", "Multiplication", "Division", "Modulo", "LAND", "LOR", "EQ", 
  "NEQ", "LT", "LE", "GT", "GE", "Integer", "Identifier", "WS"
};

dfa::Vocabulary MiniDecafParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> MiniDecafParser::_tokenNames;

MiniDecafParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x28, 0x55, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 
    0x3, 0x16, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x19, 0xb, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x28, 0xa, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x2c, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x3a, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x4e, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x51, 
    0xb, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x2, 0x3, 0x8, 0x7, 0x2, 0x4, 
    0x6, 0x8, 0xa, 0x2, 0x7, 0x3, 0x2, 0x17, 0x19, 0x3, 0x2, 0x1b, 0x1d, 
    0x3, 0x2, 0x22, 0x25, 0x3, 0x2, 0x20, 0x21, 0x4, 0x2, 0x17, 0x17, 0x1a, 
    0x1a, 0x2, 0x5d, 0x2, 0xc, 0x3, 0x2, 0x2, 0x2, 0x4, 0xf, 0x3, 0x2, 0x2, 
    0x2, 0x6, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x8, 0x39, 0x3, 0x2, 0x2, 0x2, 0xa, 
    0x52, 0x3, 0x2, 0x2, 0x2, 0xc, 0xd, 0x5, 0x4, 0x3, 0x2, 0xd, 0xe, 0x7, 
    0x2, 0x2, 0x3, 0xe, 0x3, 0x3, 0x2, 0x2, 0x2, 0xf, 0x10, 0x5, 0xa, 0x6, 
    0x2, 0x10, 0x11, 0x7, 0x27, 0x2, 0x2, 0x11, 0x12, 0x7, 0xd, 0x2, 0x2, 
    0x12, 0x13, 0x7, 0xe, 0x2, 0x2, 0x13, 0x17, 0x7, 0x11, 0x2, 0x2, 0x14, 
    0x16, 0x5, 0x6, 0x4, 0x2, 0x15, 0x14, 0x3, 0x2, 0x2, 0x2, 0x16, 0x19, 
    0x3, 0x2, 0x2, 0x2, 0x17, 0x15, 0x3, 0x2, 0x2, 0x2, 0x17, 0x18, 0x3, 
    0x2, 0x2, 0x2, 0x18, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x19, 0x17, 0x3, 0x2, 
    0x2, 0x2, 0x1a, 0x1b, 0x7, 0x12, 0x2, 0x2, 0x1b, 0x5, 0x3, 0x2, 0x2, 
    0x2, 0x1c, 0x1d, 0x7, 0x5, 0x2, 0x2, 0x1d, 0x1e, 0x5, 0x8, 0x5, 0x2, 
    0x1e, 0x1f, 0x7, 0x14, 0x2, 0x2, 0x1f, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x20, 
    0x21, 0x5, 0x8, 0x5, 0x2, 0x21, 0x22, 0x7, 0x14, 0x2, 0x2, 0x22, 0x2c, 
    0x3, 0x2, 0x2, 0x2, 0x23, 0x24, 0x5, 0xa, 0x6, 0x2, 0x24, 0x27, 0x7, 
    0x27, 0x2, 0x2, 0x25, 0x26, 0x7, 0x3, 0x2, 0x2, 0x26, 0x28, 0x5, 0x8, 
    0x5, 0x2, 0x27, 0x25, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x3, 0x2, 0x2, 
    0x2, 0x28, 0x29, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x7, 0x14, 0x2, 0x2, 
    0x2a, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x2b, 
    0x20, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x23, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x7, 
    0x3, 0x2, 0x2, 0x2, 0x2d, 0x2e, 0x8, 0x5, 0x1, 0x2, 0x2e, 0x2f, 0x9, 
    0x2, 0x2, 0x2, 0x2f, 0x3a, 0x5, 0x8, 0x5, 0xd, 0x30, 0x31, 0x7, 0xd, 
    0x2, 0x2, 0x31, 0x32, 0x5, 0x8, 0x5, 0x2, 0x32, 0x33, 0x7, 0xe, 0x2, 
    0x2, 0x33, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x7, 0x27, 0x2, 0x2, 
    0x35, 0x36, 0x7, 0x3, 0x2, 0x2, 0x36, 0x3a, 0x5, 0x8, 0x5, 0x5, 0x37, 
    0x3a, 0x7, 0x27, 0x2, 0x2, 0x38, 0x3a, 0x7, 0x26, 0x2, 0x2, 0x39, 0x2d, 
    0x3, 0x2, 0x2, 0x2, 0x39, 0x30, 0x3, 0x2, 0x2, 0x2, 0x39, 0x34, 0x3, 
    0x2, 0x2, 0x2, 0x39, 0x37, 0x3, 0x2, 0x2, 0x2, 0x39, 0x38, 0x3, 0x2, 
    0x2, 0x2, 0x3a, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3c, 0xc, 0xc, 0x2, 
    0x2, 0x3c, 0x3d, 0x9, 0x3, 0x2, 0x2, 0x3d, 0x4e, 0x5, 0x8, 0x5, 0xd, 
    0x3e, 0x3f, 0xc, 0xb, 0x2, 0x2, 0x3f, 0x40, 0x9, 0x4, 0x2, 0x2, 0x40, 
    0x4e, 0x5, 0x8, 0x5, 0xc, 0x41, 0x42, 0xc, 0xa, 0x2, 0x2, 0x42, 0x43, 
    0x9, 0x5, 0x2, 0x2, 0x43, 0x4e, 0x5, 0x8, 0x5, 0xb, 0x44, 0x45, 0xc, 
    0x9, 0x2, 0x2, 0x45, 0x46, 0x7, 0x1e, 0x2, 0x2, 0x46, 0x4e, 0x5, 0x8, 
    0x5, 0xa, 0x47, 0x48, 0xc, 0x8, 0x2, 0x2, 0x48, 0x49, 0x7, 0x1f, 0x2, 
    0x2, 0x49, 0x4e, 0x5, 0x8, 0x5, 0x9, 0x4a, 0x4b, 0xc, 0x7, 0x2, 0x2, 
    0x4b, 0x4c, 0x9, 0x6, 0x2, 0x2, 0x4c, 0x4e, 0x5, 0x8, 0x5, 0x8, 0x4d, 
    0x3b, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x41, 
    0x3, 0x2, 0x2, 0x2, 0x4d, 0x44, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x47, 0x3, 
    0x2, 0x2, 0x2, 0x4d, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x51, 0x3, 0x2, 
    0x2, 0x2, 0x4f, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x50, 0x3, 0x2, 0x2, 
    0x2, 0x50, 0x9, 0x3, 0x2, 0x2, 0x2, 0x51, 0x4f, 0x3, 0x2, 0x2, 0x2, 
    0x52, 0x53, 0x7, 0x4, 0x2, 0x2, 0x53, 0xb, 0x3, 0x2, 0x2, 0x2, 0x8, 
    0x17, 0x27, 0x2b, 0x39, 0x4d, 0x4f, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

MiniDecafParser::Initializer MiniDecafParser::_init;
