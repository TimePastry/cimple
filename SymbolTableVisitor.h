#pragma once
#include "ASTNodeDefines.h"
#include "Visitor.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

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
