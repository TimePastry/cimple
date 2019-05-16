#ifndef ELSE_NODE_H
#define ELSE_NODE_H
#include "ASTNodeDefines.h"

class ElseNode : public StatementNode
{
private:
    ASTNode* body;
public:
    ElseNode(ASTNode* b) : body(b) {}
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void accept(Visitor* v);
    string toString();
};
#endif
