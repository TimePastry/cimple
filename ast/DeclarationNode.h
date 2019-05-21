#ifndef DECLARATION_NODE_H
#define DECLARATION_NODE_H
#include "ASTNodeDefines.h"

class DeclarationNode : public ListNode
{
private:
    string id;
    string type;
public:
    DeclarationNode(string t = "", string label = "") : type(t), id(label) {}
    string getID() { return id; }
    string getType() { return type; }
    void accept(Visitor* v);
    string toString();
};

#endif
