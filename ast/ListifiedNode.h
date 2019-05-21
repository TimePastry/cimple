#ifndef LISTIFIED_NODE_H
#define LISTIFIED_NODE_H
#include "ASTNodeDefines.h"

class ListifiedNode : public ASTNode
{
private:
    vector<ASTNode*> nodes;
public:
    ListifiedNode(vector<ASTNode*> v) : nodes(v) {}
    void accept(Visitor* v);
    int getLength() { return nodes.size(); }
    ASTNode* at(int i) { return nodes.at(i); }
    string toString();
};

#endif
