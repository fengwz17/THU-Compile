# tutorial

[tutorial](https://decaf-lang.github.io/minidecaf-tutorial/)

# step1

参考实例代码，使用ANTLR工具；

描述被分析的词法和语法：

1、修改词法规范，增加对‘main’字节的读入；

2、修改语法规范，使其能够输入step1中返回int类型的函数语法EBNF。

使用ANTLR工具生成AST，使用Visitor模式遍历AST，生成对应的汇编代码。

## 错误检查

遍历AST的时候对于expr类型，检查整型变量是否在规定范围内;

其他错误类型可以直接报错。

# step2

对step1进行扩展。

词法分析新增三个token：‘-’，‘~’和‘！’，类似语法指导书，在语法上在expr中增加对这三个token的处理：

```
expr
    : (Minus | Exclamation | Tilde) expr
    | Integer
    ;
```
和step1一样用Visitor模式来遍历AST，直接在遍历的时候对三种一元操作指令生成对应的汇编语句；

# step3

对step2进行扩展。

增加加减乘除模和括号。

语法上为了简单对step2扩展：
```

expr                
    : (Minus | Exclamation | Tilde) expr    # unary
    | expr (Multiplication | Division | Modulo) expr    # mulDiv
    | expr (Addition | Minus) expr  # addSub
    | Lparen expr Rparen    # paren
    | Integer   # integer                       
    ;
```
这里尝试了用语法指导书中表达式语法的写法，但是在遍历AST生成汇编语句时没有调通，故直接在step2基础上修改，这里一元操作符、加减运算、乘除运算等语法产生式的顺序保证了正确的运算优先级。

遍历AST,遍历时直接生成对应的汇编代码，没有显式生成IR，但是生成的汇编代码保持add,sub等IR的语义（即对栈顶操作数pop，操作然后push的过程）。

# step4

对step3进行扩展，直接增加比较和相等二元操作符，增加逻辑与，逻辑或。

考虑到运算优先级，语法规则中需要将新增运算符放在加减产生式之前。

遍历AST，产生对应的汇编代码，与step3的二元操作符方式类似，pop出栈顶元素，进行操作，然后将结果push入栈。
