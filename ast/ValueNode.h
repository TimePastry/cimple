#ifndef VALUE_NODE_H
#define VALUE_NODE_H
#include "ASTNodeDefines.h"

class ValueNode : public ASTNode
{
protected:
    vtypes type;
    int intval;
    char charval;
    char* stringval;
public:
    ValueNode(int v) : type(INTV), intval(v) {}
    ValueNode(char v) : type(CHARV), charval(v) {}
    ValueNode(string t, char* v)
    {
            (t == "ID") ? (type = IDV) : (type = STRINGV);
            stringval = v;
    }
    string getType();
    vtypes getTypeIndex() { return type; }
    string getValue();
    int getIntVal() { return intval; }
    char getCharVal() { return charval; }
    string getStringVal() { return stringval; }
    virtual void accept(Visitor* v);
    virtual string toString();
};
#endif
