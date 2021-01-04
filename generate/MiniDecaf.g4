grammar MiniDecaf;

import CommonLex;

prog
    : func EOF
    ;

func
    : 'int' 'main' '(' ')' '{' stmt '}'
    ;

stmt    
    : 'return' expr ';' # retStmt
    ;

expr                
    : (Minus | Exclamation | Tilde) expr    # unary
    | expr (Multiplication | Division | Modulo) expr    # mulDiv
    | expr (Addition | Minus) expr  # addSub
    | Lparen expr Rparen    # paren
    | Integer   # integer                       
    ;