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
    ;

expr
    : (Minus | Exclamation | Tilde) expr
    | num
    ;

num
    : Integer
    ;