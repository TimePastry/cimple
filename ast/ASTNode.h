#ifndef AST_NODE_H
#define AST_NODE_H
#include "ASTNodeDefines.h"
#include <string>

class Visitor;

class ASTNode
{
private:
	int linenum;
public:
	void setLineNum(int num) { linenum = num; }
	int getLineNum() { return linenum; }
    virtual std::string toString() = 0;
    virtual void accept(Visitor* v) = 0;
};
#endif
