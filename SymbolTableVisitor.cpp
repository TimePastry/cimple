#include "SymbolTableVisitor.h"

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
