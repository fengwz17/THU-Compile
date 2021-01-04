grammar MiniDecaf;

import CommonLex;

prog
    : func EOF
    ;

func
    : 'int' 'main' '(' ')' '{' stmt '}'
    ;

stmt    
    : 'return' expr ';' 
    # retStmt
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

    | Lparen expr Rparen    
    # paren

    | Integer   
    # integer                       
    ;