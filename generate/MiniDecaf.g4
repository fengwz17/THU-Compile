grammar MiniDecaf;

import CommonLex;

prog
    : (func | globalDecl ';')* EOF
    ;

func
    : type Identifier '(' (type Identifier ',')* (type Identifier)? ')' ('{' blockItem* '}' | ';')
    ;

blockItem
    : stmt
    | declaration ';'
    ;

globalDecl
    : type Identifier ('=' Integer)?
    # global
    ;


declaration
    : type Identifier ('=' expr)?
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

    | For '(' (declaration | expr)? ';' (expr)? ';' (expr)? ')' stmt     
    # forLoop
    
    | While '(' expr ')' stmt    
    # whileLoop

    | Do stmt While '(' expr ')' ';'                            
    # doWhile

   
    | Break ';'                                                   
    # break

    | Continue ';'                                                
    # continue        

    | ';'
    # semicolon
    ;

expr        
    : Identifier '(' (expr ',')* (expr)? ')'
    # funcCall

    | (Minus | Exclamation | Tilde) expr
    # unary

    | expr (Multiplication | Division | Modulo) expr    
    # mulDiv

    | expr (Addition | Minus) expr  
    # addSub

    | expr (LT | LE | GT | GE) expr
    # compare

    | expr (EQ | NEQ) expr
    # equal

    | expr (LAND) expr
    # land

    | expr (LOR) expr
    # lor
    
    | expr '?' expr ':' expr
    # condExpr

    | Lparen expr Rparen    
    # paren

    | Identifier '=' expr
    # assign

    | Identifier
    # identifier

    | Integer   
    # integer                       
    ;

type
    : 'int'
    ;