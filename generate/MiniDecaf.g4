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

    | type Identifier ('[' Integer ']')+
    # globalArry
    ;


declaration
    : type Identifier ('=' expr)?
    # varDefine

    | type Identifier ('[' Integer ']')+
    # localArry
    ;

stmt    
    : 'return' expr ';' 
    # retStmt

    | expr? ';'
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
    : unary '=' expr
    # assign

    | conditional
    # cond
    ;

conditional
    : logical_or '?' expr ':' conditional
    # condExpr

    | logical_or
    # lor_op
    ;

logical_or
    : logical_or '||' logical_or
    # lor

    | logical_and                                                       
    # land_op
    ;

logical_and
    : logical_and '&&' logical_and                                         
    # land

    | equality                                                           
    # equal_op
    ;

equality
    : equality ('==' | '!=') equality                                         
    # equal

    | relational                                                           
    # rela_op
    ;

relational
    : relational ('<' | '<=' | '>' | '>=') relational                             
    # compare

    | add                                                           
    # add_op
    ;

add
    : add ('+' | '-') add                                           
    # addSub

    | mul                                                           
    # mul_op
    ;

mul
    : mul ('*' | '/' | '%') mul                                     
    # mulDiv

    | unary                                                        
    # unary_op
    ;

unary
    : ('!' | '~' | '-' | '*' | '&') unary                          
    # unaryOp

    | '(' type ')' unary                                           
    # cast

    | postfix
    # postfix_op
    ;

postfix
    : Identifier '(' (expr ',')* (expr)? ')'                        
    # funcCall
    
    | postfix '[' expr ']'
    # arryIndex

    | primary
    # primary_op
    ;

primary
    : '(' expr ')'                                                  
    # paren

    | Identifier                                                    
    # identifier

    | Integer                                                      
    # integer
    ;

type
    : 'int' '*'*                                                    
    ;