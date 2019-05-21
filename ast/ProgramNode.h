#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H
#include "ASTNodeDefines.h"

class ProgramNode : public ASTNode
{
private:
    ASTNode* globals;
    ASTNode* functions;
public:
    ProgramNode(ASTNode* g, ASTNode* f) : globals(g), functions(f) {}
    ASTNode* getGlobals() { return globals; }
    void setGlobals(ASTNode* g) { globals = g; }
    ASTNode* getFunctions() { return functions; }
    void setFunctions(ASTNode* f) { functions = f; }
    void accept(Visitor* v);
    string toString();
};

#endif
