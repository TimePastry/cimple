#pragma once
#include "ASTNodeDefines.h"
#include "Visitor.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

/*
 * Creates a symbol table for a given AST
 * The symbol table is set up as a vector of maps
 * index 0 of the table contains global symbols
 * each subsequent index contains symbols defined for each function, in 1-indexed fashion
 *
 * standard, built-in functions are defined globally before any other symbols are read,
 * making them reserved keywords
 * currently the only standard function is "print(string)"
 */

class SymbolTableVisitor : public Visitor
{
private:
    vector<map<string, ASTNode*> > root;
    int functionIndex;
    void fillStdFunctions();
public:
    SymbolTableVisitor() { functionIndex = 0; root.push_back(map<string, ASTNode*>()); fillStdFunctions(); }
    vector<map<string, ASTNode*> > getRootTable() { return root; }

    bool previsit(FunctionNode* n);

    void visit(DeclarationNode* n);
    void visit(ArgumentNode* n);
};
