#ifndef BINOP_NODE_H
#define BINOP_NODE_H
#include "ASTNodeDefines.h"

class BinopNode : public ASTNode
{
private:
    ASTNode* lhs;
    ASTNode* rhs;
    operators op;
public:
    BinopNode(ASTNode* l, ASTNode* r, int opindex) : lhs(l), rhs(r), op(operators(opindex)) {}
    operators getOp() { return op; }
    ASTNode* getLeft() { return lhs; }
    ASTNode* getRight() { return rhs; }
    void accept(Visitor* v);
    string toString();
};
#endif
