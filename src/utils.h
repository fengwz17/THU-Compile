#pragma once

#include <string>
#include <memory>
#include <map>
#include <tuple>
#include <iostream>
#include <queue>
#include <climits>

template<typename T>
using symTab = std::map<std::string, std::map<std::string, T> >;

class Type {
public:
    virtual std::string getType() = 0;
    virtual std::shared_ptr<Type> typeCast(int _valueType) = 0;
    
    virtual int getStarNum() { return starNum; }
    virtual int getSize() { return size; }
    virtual std::shared_ptr<Type> getBaseType() { return baseType; }
    void setValueType(int _valueType) { valueType = _valueType; }
    int getValueType() { return valueType; }
   
    bool typeCheck(std::shared_ptr<Type> srcType) 
    {
        if (srcType.get()->getType() == typeLiteral && srcType.get()->getStarNum() == starNum) 
        {
            return true;
        }
        return false;
    }
    bool typeCheckLiteral(std::string srcType) 
    { 
        if (srcType == typeLiteral) 
        {
            return true;
        }
        return false;
    }
    virtual ~Type() {}
protected:
    int valueType;
    int starNum;
    int size;
    std::string typeLiteral;
    std::shared_ptr<Type> baseType;
};


// Interger type
class IntType : public Type {
public:
    IntType() 
    {
        valueType = 0; typeLiteral = "Int"; starNum = 0; size = 4;
    }
    IntType(int _valueType) 
    {
        valueType = _valueType; typeLiteral = "Int", starNum = 0; size = 4;
    }
    std::shared_ptr<Type> typeCast(int _valueType) 
    {
        std::shared_ptr<Type> type = std::make_shared<IntType>(_valueType);
        return type;
    }
    std::string getType() { return typeLiteral; }
};


// Type: Pointer of integer type
class IntptrType : public Type {
public:
    IntptrType(int _starNum) 
    {
        starNum = _starNum; typeLiteral = "Intptr"; valueType = 0; size = 4;
    }
    IntptrType(int _starNum, int _valueType) 
    {
        starNum = _starNum; typeLiteral = "Intptr"; valueType = _valueType; size = 4;
    }
    std::shared_ptr<Type> typeCast(int _valueType) 
    {
        std::shared_ptr<Type> type = std::make_shared<IntptrType>(starNum, _valueType);
        return type;
    }
    std::shared_ptr<Type> getBaseType() 
    {
        std::shared_ptr<Type> returnType;
        if (starNum == 1) 
        {
            returnType = std::make_shared<IntType>(1);
        } 
        else if (starNum > 1) 
        {
            returnType = std::make_shared<IntptrType>(starNum - 1, 1);
        }
        return returnType;
    }
    std::string getType() { return typeLiteral; }
};


// None type. eg. The return type of for loop, if statement.
class NoneType : public Type {
public:
    NoneType() 
    {
        valueType = 0; typeLiteral = "None";
    }
    std::shared_ptr<Type> typeCast(int _valueType) 
    {
        std::cerr << "[Warning] Casting Nonetype symbol " << typeLiteral << "\n";
        return std::shared_ptr<Type>(this);
    }
    std::string getType() { return typeLiteral; }
};

class ArrayType : public Type {
public:
    ArrayType(std::shared_ptr<Type> _baseType, long _arrLength) 
    {
        baseType = _baseType; size = baseType.get()->getSize() * _arrLength; 
        typeLiteral = "Array";
        valueType = 0;
    }
    std::string getType() { return typeLiteral; }
    std::shared_ptr<Type> typeCast(int _valueType=0) 
    {
        std::shared_ptr<Type> type = std::make_shared<ArrayType>(baseType, size);
        return type;
    }
};


// variable's name, offset, defined line and typeinfo
class Symbol {
public:
    Symbol(std::string _literal, int _offset, std::shared_ptr<Type> _type, int _line=-1, int _volume=1) 
    {
        literal = _literal; 
        offset = _offset; 
        type = _type;
        line = _line; 
        volume = _volume;
    }
    std::string getLiteral() { return literal; }
    int getOffset() { return offset; }
    std::shared_ptr<Type> getType() { return type; }
    int getLineNum() { return line; }
    int getVolume() { return volume; }

    void setLineNum(int _line) { line = _line; }

protected:
    std::string literal;
    int offset, line, volume;
    std::shared_ptr<Type> type;
};


// stores information of defined functions;
// Function name, parameter's type, return type, initial state
class FuncSymbol {
public:
    FuncSymbol(std::string _literal, std::shared_ptr<Type> _retType, 
    std::vector<std::shared_ptr<Type> > _argType, bool _init=false) 
    {
        literal = _literal; 
        retType = _retType; 
        argType = _argType; 
        init = _init;
    }
    std::string getLiteral() { return literal; }
    std::shared_ptr<Type> getRetType() { return retType; }
    std::shared_ptr<Type> getArgType(int i) { return argType[i]; }
    int getArgSize() { return argType.size(); }
    bool initialized() { return init; }
    void initialize() { init = true; }

protected:
    bool init;
    std::string literal;
    std::vector<std::shared_ptr<Type> > argType;
    std::shared_ptr<Type> retType;
};


// global map structure to store the symbol table
// symbol table:
// funcName, variables, offset, data type 
class GlobStruct {
public:
    symTab<std::shared_ptr<Symbol> > symbolTable;
    std::unordered_map<std::string, std::shared_ptr<FuncSymbol> > funcTable;
    std::queue<std::shared_ptr<Type> > typeQueue;
    static GlobStruct& getInstance() {
        static GlobStruct pInstance;
        return pInstance;
    }
private:
    GlobStruct() {}
    GlobStruct(const GlobStruct& other) {};
};