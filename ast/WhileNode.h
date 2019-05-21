#ifndef WHILE_NODE_H
#define WHILE_NODE_H
#include "ASTNodeDefines.h"

class WhileNode : public StatementNode
{
private:
    ASTNode* condition;
    ASTNode* body;
public:
    WhileNode(ASTNode* c, ASTNode* b) : condition(c), body(b) {}
    ASTNode* getCondition() { return condition; }
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void accept(Visitor* v);
    string toString();
};

#endif
