#ifndef FUNCTION_BODY_NODE_H
#define FUNCTION_BODY_NODE_H
#include "ASTNodeDefines.h"

class FunctionBodyNode : public ASTNode
{
private:
    ASTNode* variables;
    ASTNode* statements;
public:
    FunctionBodyNode(ASTNode* var, ASTNode* state) : variables(var), statements(state) {}
    void setVariables(ASTNode* v) { variables = v; }
    ASTNode* getVariables() { return variables; }
    void setStatements(ASTNode* s) { statements = s; }
    ASTNode* getStatements() { return statements; }
    void accept(Visitor* v);
    string toString();
};
#endif
