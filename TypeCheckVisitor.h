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
    vtypes returnType;

    vtypes getType(ASTNode* n);
    vtypes binopReturn(ValueNode* l, ValueNode* r, operators o);
    vtypes getTypeById(string id);
    ASTNode* getNodeById(string id);
public:
    TypeCheckVisitor(vector<map<string, ASTNode*> > t) : table(t) {}
    
    bool previsit(FunctionNode* n) { current++; returnType = stringToVtypes(n->getType());  return false; }

    virtual void visit(FunctionCallNode* n);
    virtual void visit(AssignmentNode* n);
    virtual void visit(WhileNode* n);
    virtual void visit(IfNode* n);
    virtual void visit(ReturnNode* n);
};
