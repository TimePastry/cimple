#ifndef VALUE_LIST_NODE_H
#define VALUE_LIST_NODE_H
#include "ASTNodeDefines.h"

class ValueListNode : public ValueNode
{
private:
    ASTNode* next = NULL;
public:
    ValueListNode(int v) : ValueNode(v) {}
    ValueListNode(char v) : ValueNode(v) {}
    ValueListNode(string t, char* v) : ValueNode(t, v) {}
    void accept(Visitor* v);
    string toString();

    ASTNode* getNext() { return next; }
    void setNext(ASTNode* n) { next = n; }
    void append(ASTNode* last);
};
#endif
