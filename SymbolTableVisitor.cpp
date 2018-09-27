#include "SymbolTableVisitor.h"
//ArgumentNode(string t = "", string label = "") : type(t), id(label) {}
//FunctionNode(string t = "", string label = "", ASTNode* a = NULL, ASTNode* b = NULL) : type(t), id(label), args(a), body(b) {}
void SymbolTableVisitor::fillStdFunctions()
{
    vector<ASTNode*> v;
    v.push_back(new ArgumentNode("string", "s"));
    ListifiedNode* printList = new ListifiedNode(v);
    root.at(0)["print"] = new FunctionNode("int", "print", printList, NULL /*todo*/);
}

bool SymbolTableVisitor::previsit(FunctionNode* n)
{
    if (root.at(0).find(n->getID()) == root.at(0).end())
    {
        root.at(0)[n->getID()] = n;
    }
    else
    {
        throw n;
    }
    functionIndex++;
    root.push_back(map<string, ASTNode*>());

    return false;
}

void SymbolTableVisitor::visit(DeclarationNode* n)
{
    map<string, ASTNode*> table = root.at(functionIndex);
    if (table.find(n->getID()) == table.end())
    {
        root.at(functionIndex)[n->getID()] = n;
    }
    else
    {
        throw n;
    }
}

void SymbolTableVisitor::visit(ArgumentNode* n)
{
    map<string, ASTNode*> table = root.at(functionIndex);
    if (table.find(n->getID()) == table.end())
    {
        root.at(functionIndex)[n->getID()] = n;
    }
    else
    {
        throw n;
    }
}
