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

# step5

这一步引入了对变量的声明和使用，使main函数中包含多条语句和声明。

修改词法和语法规范，使编译器能够读变量。

加入变量时需要确定变量的存储和访问，于是引入了栈帧和符号表，相对于前一步来说需要进行一个比较大的改进，符号表的维护参考了助教的示例代码，用一个数据结构map<funcName, map<varName, varOffset>>来存储符号表，在遍历AST的时候访问到函数节点、变量节点会更新符号表。

在代码生成中，访问到对变量声明赋值节点时，通过前面维护的符号表来得到偏移量，用于生成对应的汇编代码。

## 错误检查

前面步的错误检查使用antlr4的错误监听器（ANTLRErrorListener和ANTLRErrorStrategy），然后代码生成部分增加对int型变量返回值的范围限定。

这一步中在维护符号表的部分，遍历AST时通过检查维护的符号表中函数和变量符号是否存在来检测是否有重复声明与使用未声明变量的错误。

# step6

支持 if 语句和条件表达式;

# step7

增加块语句，多个语句组成一个块，每个块都是一个作用域。

语法规则上的改进比较容易，增加block类型表示作用域即可。

主要难点在于名称解析，即需要在符号表中区分不同作用域的变量，参考了助教的示例代码，在符号表中，通过增加两个下标index1和index2的方式来处理不同的作用域和同一作用域下不同的语句. 

另外，示例代码中通过记录声明语句行号的方式来处理声明覆盖的问题，而该方式当代码中多个声明在同一行时会出现问题，这里我改变了这一方式，增加了一个数据结构map<varName, varNum>，先在遍历AST时，每次遇到赋值和声明，令当前varName的varNum加1，记录每个varName最大的varNum，然后在生成汇编时，比较每个varName的当前varNum和最大varNum来处理声明覆盖。

# step8

增加对循环语句，以及 break/continue 的支持;

注意到for要自带一个作用域，需要进行名称解析；

break，continue的标号需要用一个数据结构维护，这里用一个vector记录。

