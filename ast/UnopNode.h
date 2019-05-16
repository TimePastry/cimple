#ifndef UNOP_NODE_H
#define UNOP_NODE_H
#include "ASTNodeDefines.h"

class UnopNode : public ASTNode
{
private:
    ASTNode* rhs;
    operators op;
public:
    UnopNode(ASTNode* r, int opindex) : rhs(r), op(operators(opindex)) {}
    ASTNode* getValue() { return rhs; }
    operators getOp() { return op; }
    void accept(Visitor* v);
    string toString();
};
#endif
