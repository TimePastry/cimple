#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H
#include "ASTNodeDefines.h"

class FunctionNode : public ListNode
{
private:
    string id;
    string type;
    ASTNode* args;
    ASTNode* body;
public:
    FunctionNode(string t = "", string label = "", ASTNode* a = NULL, ASTNode* b = NULL) : type(t), id(label), args(a), body(b) {}
    string getType() { return type; }
    string getID() { return id; }
    void setArgs(ASTNode* a) { args = a; }
    ASTNode* getArgs() { return args; }
    ASTNode* getArg(int i);
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void accept(Visitor* v);
    string toString();
};

#endif
