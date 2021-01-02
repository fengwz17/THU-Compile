#include <iostream>

#include "MiniDecafLexer.h"
#include "MiniDecafParser.h"
#include "CodeEmission.h"

using namespace antlr4;
using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "[error] Input sourcefile required: ./MiniDecaf <sourcefile>\n";
        return 1;
    }
    ifstream sourceFile;
    sourceFile.open(argv[1]);

    // lexer & parser part of antlr4
    ANTLRInputStream input(sourceFile);
    MiniDecafLexer lexer(&input);
    lexer.addErrorListener(new BaseErrorListener());

    CommonTokenStream tokens(&lexer);
    MiniDecafParser parser(&tokens);

    shared_ptr<antlr4::ANTLRErrorStrategy> handler = make_shared<BailErrorStrategy>();
    parser.setErrorHandler(handler);

    // customized pass: allocator, typer, codegen and etc.

    MiniDecafParser::ProgContext* treeNode = parser.prog();
    CodeEmission codeEmission;
    string asmCode = codeEmission.visitProg(treeNode);
    
    cout << asmCode << endl;

    return 0;
}

