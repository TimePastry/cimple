#pragma once
#include "Visitor.h"
#include "ASTNodeDefines.h"
#include <map>

using namespace std;

/*
 * Checks type validity when given an AST and a symbol table for it
 *
 */

class TypeCheckVisitor : public Visitor
{
private:
    vector<map<string, ASTNode*> > table;
    int current = 0;
    bool verbose;
    vtypes returnType;

    vtypes getType(ASTNode* n);
    vtypes binopReturn(ValueNode* l, ValueNode* r, operators o);
    vtypes getTypeById(string id);
    ASTNode* getNodeById(string id);
public:
    TypeCheckVisitor(vector<map<string, ASTNode*> > t, bool v = false) : table(t), verbose(v) {}
    
    bool previsit(FunctionNode* n) { current++; returnType = stringToVtypes(n->getType());  return false; }

    virtual void visit(FunctionCallNode* n);
    virtual void visit(AssignmentNode* n);
    virtual void visit(WhileNode* n);
    virtual void visit(IfNode* n);
    virtual void visit(ReturnNode* n);
};
