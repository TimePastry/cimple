#pragma once
#include "Visitor.h"
#include "ASTNodeDefines.h"
#include <map>

using namespace std;

class TypeCheckVisitor : public Visitor
{
private:
    vector<map<string, ASTNode*> > table;
    int current = 0;

    vtypes getType(ASTNode* n);

    vtypes getTypeById(string id);
public:
    TypeCheckVisitor(vector<map<string, ASTNode*> > t) : table(t) {}
    
    bool previsit(FunctionNode*) { current++; return false; }

    virtual void visit(FunctionCallNode* n) {};
    virtual void visit(AssignmentNode* n);
    virtual void visit(WhileNode* n) {};
    virtual void visit(IfNode* n) {};
    virtual void visit(ReturnNode* n) {};
};
