# tutorial

[tutorial](https://decaf-lang.github.io/minidecaf-tutorial/)

# step1

参考实例代码，使用ANTLR工具；

描述被分析的词法和语法：

1、修改词法规范，增加对‘main’字节的读入；

2、修改语法规范，使其能够输入step1中返回int类型的函数语法EBNF。

使用ANTLR工具生成AST，使用Visitor模式遍历AST，生成对应的汇编代码。

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

