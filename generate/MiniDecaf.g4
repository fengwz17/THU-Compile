grammar MiniDecaf;

import CommonLex;

prog
    : func EOF
    ;

func
    : type Identifier '(' ')' '{' blockItem* '}'
    ;

blockItem
    : stmt
    | declaration
    ;

declaration
    : type Identifier ('=' expr)? ';'
    # varDefine
    ;

stmt    
    : 'return' expr ';' 
    # retStmt

    | expr ';'
    # exprStmt

    | If '(' expr ')' stmt (Else stmt)?
    # ifElse 
    | '{' blockItem* '}' 
    # block
    ;

expr                
    : (Minus | Exclamation | Tilde) expr
    # unary

    | expr (Multiplication | Division | Modulo) expr    
    # mulDiv

    | expr (LT | LE | GT | GE) expr
    # compare

    | expr (EQ | NEQ) expr
    # equal

    | expr (LAND) expr
    # land

    | expr (LOR) expr
    # lor

    | expr (Addition | Minus) expr  
    # addSub
    
    | expr '?' expr ':' expr
    # condExpr

    | Lparen expr Rparen    
    # paren

    | Identifier '=' expr
    # assign

    | Identifier
    # Identifier

    | Integer   
    # integer                       
    ;

type
    : 'int'
    ;