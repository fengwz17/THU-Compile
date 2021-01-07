
// Generated from MiniDecaf.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  MiniDecafLexer : public antlr4::Lexer {
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

  MiniDecafLexer(antlr4::CharStream *input);
  ~MiniDecafLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

