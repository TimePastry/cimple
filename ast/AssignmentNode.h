#ifndef ASSIGNMENT_NODE_H
#define ASSIGNMENT_NODE_H
#include "ASTNodeDefines.h"

class AssignmentNode : public StatementNode
{
private:
    string id;
    ASTNode* term;
public:
    AssignmentNode(string label, ASTNode* t) : id(label), term(t) {}
    string getID() { return id; }
    ASTNode* getTerm() { return term; }
    void accept(Visitor* v);
    string toString();
};
#endif
