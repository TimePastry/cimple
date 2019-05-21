#ifndef LIST_NODE_H
#define LIST_NODE_H
#include "ASTNodeDefines.h"

class ListNode : public ASTNode
{
protected:
    ASTNode* next = NULL;
public:
    ASTNode* getNext() { return next; }
    void setNext(ASTNode* n) { next = n; }
    void append(ASTNode* last);
    virtual void accept(Visitor* v);
};

#endif
