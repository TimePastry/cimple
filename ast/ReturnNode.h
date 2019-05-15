#ifndef RETURN_NODE_H
#define RETURN_NODE_H
#include "ASTNodeDefines.h"

class ReturnNode : public StatementNode
{
private:
    ASTNode* value;
public:
    ReturnNode(ASTNode* v) : value(v) {}
    ASTNode* getValue() { return value; }
    void accept(Visitor* v);
    string toString();
};
#endif
