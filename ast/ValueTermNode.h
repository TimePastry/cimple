#ifndef VALUE_TERM_NODE_H
#define VALUE_TERM_NODE_H
#include "ASTNodeDefines.h"

class ValueTermNode : public ValueNode
{
public:
    ValueTermNode(int v) : ValueNode(v) {}
    ValueTermNode(char v) : ValueNode(v) {}
    ValueTermNode(string t, char* v) : ValueNode(t, v) {}
    virtual void accept(Visitor* v);
};
#endif
