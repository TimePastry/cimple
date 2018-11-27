#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;

enum vtypes { INTV, STRINGV, CHARV, IDV, INTF, STRINGF, CHARF };
enum operators { Add, Sub, Or, And, Lt, Gt, Lte, Gte, Eq, Neq, Mult, Not };

class Visitor;

string opToString(operators o);
string vtypesToString(vtypes v);
vtypes stringToVtypes(string s, bool isFunc = false);

class ASTNode
{
private:
	int linenum;
public:
	void setLineNum(int num) { linenum = num; }
	int getLineNum() { return linenum; }
    virtual string toString() = 0;
    virtual void accept(Visitor* v) = 0;
};

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

class StatementNode : public ListNode
{
protected:

};

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

class FunctionNode : public ListNode
{
private:
    string id;
    string type;
    ASTNode* args;
    ASTNode* body;
public:
    FunctionNode(string t = "", string label = "", ASTNode* a = NULL, ASTNode* b = NULL) : type(t), id(label), args(a), body(b) {}
    string getType() { return type; }
    string getID() { return id; }
    void setArgs(ASTNode* a) { args = a; }
    ASTNode* getArgs() { return args; }
    ASTNode* getArg(int i);
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void accept(Visitor* v);
    string toString();
};

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

class FunctionBodyNode : public ASTNode
{
private:
    ASTNode* variables;
    ASTNode* statements;
public:
    FunctionBodyNode(ASTNode* var, ASTNode* state) : variables(var), statements(state) {}
    void setVariables(ASTNode* v) { variables = v; }
    ASTNode* getVariables() { return variables; }
    void setStatements(ASTNode* s) { statements = s; }
    ASTNode* getStatements() { return statements; }
    void accept(Visitor* v);
    string toString();
};

class FunctionCallNode : public StatementNode
{
private:
    string id;
    ASTNode* values;
public:
    FunctionCallNode(string label, ASTNode* v) : id(label), values(v) {}
    string getID() { return id; }
    void setValues(ASTNode* v) { values = v; }
    ASTNode* getValues() { return values; }
    ASTNode* getVal(int i);
    void accept(Visitor* v);
    string toString();
};

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

class ValueTermNode : public ValueNode
{
public:
    ValueTermNode(int v) : ValueNode(v) {}
    ValueTermNode(char v) : ValueNode(v) {}
    ValueTermNode(string t, char* v) : ValueNode(t, v) {}
    virtual void accept(Visitor* v);
};

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

class BinopNode : public ASTNode
{
private:
    ASTNode* lhs;
    ASTNode* rhs;
    operators op;
public:
    BinopNode(ASTNode* l, ASTNode* r, int opindex) : lhs(l), rhs(r), op(operators(opindex)) {}
    operators getOp() { return op; }
    ASTNode* getLeft() { return lhs; }
    ASTNode* getRight() { return rhs; }
    void accept(Visitor* v);
    string toString();
};


class UnopNode : public ASTNode
{
private:
    ASTNode* rhs;
    operators op;
public:
    UnopNode(ASTNode* r, int opindex) : rhs(r), op(operators(opindex)) {}
    ASTNode* getValue() { return rhs; }
    operators getOp() { return op; }
    void accept(Visitor* v);
    string toString();
};

class WhileNode : public StatementNode
{
private:
    ASTNode* condition;
    ASTNode* body;
public:
    WhileNode(ASTNode* c, ASTNode* b) : condition(c), body(b) {}
    ASTNode* getCondition() { return condition; }
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void accept(Visitor* v);
    string toString();
};

class IfNode : public StatementNode
{
private:
    ASTNode* condition;
    ASTNode* body;
    ASTNode* elseBody;
public:
    IfNode(ASTNode* c, ASTNode* b, ASTNode* e = NULL) : condition(c), body(b), elseBody(e) {}
    ASTNode* getCondition() { return condition; }
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void setElse(ASTNode* e) { elseBody = e; }
    ASTNode* getElse() { return elseBody; }
    void accept(Visitor* v);
    string toString();
};

class ElseNode : public StatementNode
{
private:
    ASTNode* body;
public:
    ElseNode(ASTNode* b) : body(b) {}
    void setBody(ASTNode* b) { body = b; }
    ASTNode* getBody() { return body; }
    void accept(Visitor* v);
    string toString();
};

class ReturnNode : public StatementNode
{
private:
    ASTNode* value;
public:
    ReturnNode(ASTNode* v) : value(v) {}
    ASTNode* getValue() { return value; }
    void accept(Visitor* v);
    string toString();
};

int listLength(ASTNode* n);
