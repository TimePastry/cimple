#ifndef ARGUMENT_NODE_H
#define ARGUMENT_NODE_H
#include "ASTNodeDefines.h"

class ArgumentNode : public ListNode
{
private:
    string id;
    string type;
public:
    ArgumentNode(string t = "", string label = "") : type(t), id(label) {}
    string getType() { return type; }
    string getID() { return id; }
    void accept(Visitor* v);
    string toString();
};
#endif
