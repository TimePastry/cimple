#ifndef IF_NODE_H
#define IF_NODE_H
#include "ASTNodeDefines.h"

class IfNode : public StatementNode
{
private:
    ASTNode* condition;
    ASTNode* body;
    ASTNode* elseBody;
public:
    IfNode(ASTNode* c, ASTNode* b, ASTNode* e = NULL) : condition(c), body(b), elseBody(e) {}
    ASTNode* getCondition() { return condition; }
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void setElse(ASTNode* e) { elseBody = e; }
    ASTNode* getElse() { return elseBody; }
    void accept(Visitor* v);
    string toString();
};
#endif
