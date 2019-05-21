#ifndef FUNCTION_CALL_NODE_H
#define FUNCTION_CALL_NODE_H
#include "ASTNodeDefines.h"

class FunctionCallNode : public StatementNode
{
private:
    string id;
    ASTNode* values;
public:
    FunctionCallNode(string label, ASTNode* v) : id(label), values(v) {}
    string getID() { return id; }
    void setValues(ASTNode* v) { values = v; }
    ASTNode* getValues() { return values; }
    ASTNode* getVal(int i);
    void accept(Visitor* v);
    string toString();
};
#endif
