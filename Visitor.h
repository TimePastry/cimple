#pragma once
#include "ASTNodeDefines.h"
#include <string>
#include <vector>

using namespace std;

/*
 * Base class for visitor pattern
 */

class Visitor
{
public:
    virtual bool previsit(ASTNode* n) { return false; };
    virtual bool previsit(ProgramNode* n) { return false; };
    virtual bool previsit(DeclarationNode* n) { return false; };
    virtual bool previsit(FunctionNode* n) { return false; };
    virtual bool previsit(ArgumentNode* n) { return false; };
    virtual bool previsit(FunctionBodyNode* n) { return false; };
    virtual bool previsit(FunctionCallNode* n) { return false; };
    virtual bool previsit(ValueNode* n) { return false; };
    virtual bool previsit(ValueListNode* n) { return false; };
    virtual bool previsit(ValueTermNode* n) { return false; };
    virtual bool previsit(AssignmentNode* n) { return false; };
    virtual bool previsit(BinopNode* n) { return false; };
    virtual bool previsit(UnopNode* n) { return false; };
    virtual bool previsit(WhileNode* n) { return false; };
    virtual bool previsit(IfNode* n) { return false; };
    virtual bool previsit(ElseNode* n) { return false; };
    virtual bool previsit(ReturnNode* n) { return false; };
    virtual bool previsit(ListifiedNode* n) { return false; };

    virtual void visit(ASTNode* n) {};
    virtual void visit(ProgramNode* n) {};
    virtual void visit(DeclarationNode* n) {};
    virtual void visit(FunctionNode* n) {};
    virtual void visit(ArgumentNode* n) {};
    virtual void visit(FunctionBodyNode* n) {};
    virtual void visit(FunctionCallNode* n) {};
    virtual void visit(ValueNode* n) {};
    virtual void visit(ValueListNode* n) {}
    virtual void visit(ValueTermNode* n) {}
    virtual void visit(AssignmentNode* n) {};
    virtual void visit(BinopNode* n) {};
    virtual void visit(UnopNode* n) {};
    virtual void visit(WhileNode* n) {};
    virtual void visit(IfNode* n) {};
    virtual void visit(ElseNode* n) {};
    virtual void visit(ReturnNode* n) {};
    virtual void visit(ListifiedNode* n) {};

    virtual void postvisit(ASTNode* n) {};
    virtual void postvisit(ProgramNode* n) {};
    virtual void postvisit(DeclarationNode* n) {};
    virtual void postvisit(FunctionNode* n) {};
    virtual void postvisit(ArgumentNode* n) {};
    virtual void postvisit(FunctionBodyNode* n) {};
    virtual void postvisit(FunctionCallNode* n) {};
    virtual void postvisit(ValueNode* n) {};
    virtual void postvisit(ValueListNode* n) {}
    virtual void postvisit(ValueTermNode* n) {}
    virtual void postvisit(AssignmentNode* n) {};
    virtual void postvisit(BinopNode* n) {};
    virtual void postvisit(UnopNode* n) {};
    virtual void postvisit(WhileNode* n) {};
    virtual void postvisit(IfNode* n) {};
    virtual void postvisit(ElseNode* n) {};
    virtual void postvisit(ReturnNode* n) {};
    virtual void postvisit(ListifiedNode* n) {};
};

/*
 * Identity Visitor for testing visitor pattern
 * Prints the name of each node as it is encountered
 */

class IdentityVisitor : public Visitor
{
public:
    void visit(ASTNode* n) { cout << "not implemented" << endl; }
    void visit(ProgramNode* n) { cout << "ProgramNode" << endl; }
    void visit(DeclarationNode* n) { cout << "DeclarationNode" << endl; };
    void visit(FunctionNode* n) { cout << "FunctionNode" << endl; };
    void visit(ArgumentNode* n) { cout << "ArgumentNode" << endl; };
    void visit(FunctionBodyNode* n) { cout << "FunctionBodyNode" << endl; };
    void visit(FunctionCallNode* n) { cout << "FunctionCallNode" << endl; };
    void visit(ValueNode* n) { cout << "ValueNode" << endl; };
    void visit(ValueListNode* n) { cout << "ValueListNode" << endl; }
    void visit(ValueTermNode* n) { cout << "ValueTermNode" << endl; }
    void visit(AssignmentNode* n) { cout << "AssignmentNode" << endl; };
    void visit(BinopNode* n) { cout << "BinopNode" << endl; };
    void visit(UnopNode* n) { cout << "UnopNode" << endl; };
    void visit(WhileNode* n) { cout << "WhileNode" << endl; };
    void visit(IfNode* n) { cout << "IfNode" << endl; };
    void visit(ElseNode* n) { cout << "ElseNode" << endl; };
    void visit(ReturnNode* n) { cout << "ReturnNode" << endl; };
    void visit(ListifiedNode* n) { cout << "ListifiedNode" << endl; };
};

/*
 * Visitor for printing an AST in syntactically accurate format
 * Only has visit methods for top level nodes, allowing for precise formatting
 */

class PrettyPrintVisitor : public Visitor
{
private:
    string contents;
    bool decs;
public:
    PrettyPrintVisitor() { contents = ""; decs = true; }
    string getContents() { return contents; }

    void visit(ASTNode* n);
    void visit(ProgramNode* n);
    void visit(DeclarationNode* n);
    void visit(FunctionNode* n);

    void postvisit(ListifiedNode* n);
};

/*
 * Helper to the PrettyPrintVisitor
 * Prints the body of functions and the list of arguments in a function definition 
 */

class PrettyPrintVisitorHelper : public Visitor
{
private:
    string contents;
    int tablevel;
    int templevel;
public:
    PrettyPrintVisitorHelper() { contents = ""; tablevel = 0; templevel = 0; }
    string getContents() { return contents; }
    void trim();

    bool previsit(IfNode* n);

    void visit(DeclarationNode* n);
    void visit(ArgumentNode* n);
    void visit(FunctionBodyNode* n);
    void visit(FunctionCallNode* n);
    void visit(ValueNode* n) {}
    void visit(ValueListNode* n) {}
    void visit(ValueTermNode* n);
    void visit(AssignmentNode* n);
    void visit(BinopNode* n);
    void visit(UnopNode* n);
    void visit(WhileNode* n);
    void visit(IfNode* n);
    void visit(ElseNode* n);
    void visit(ReturnNode* n);

    void postvisit(WhileNode* n);
    void postvisit(IfNode* n);
    void postvisit(ElseNode* n);
    void postvisit(ListifiedNode* n);
};

/*
 * Visitor that flattens linked list structures into vectors
 */

class ListifyVisitor : public Visitor
{
public:
    void visit(ProgramNode* n);
    void visit(FunctionNode* n);
    void visit(FunctionBodyNode* n);
    void visit(FunctionCallNode* n);
    void visit(WhileNode* n);
    void visit(IfNode* n);
    void visit(ElseNode* n);
    vector<ASTNode*> buildList(ListNode* n); 
    vector<ASTNode*> buildList(ValueListNode* n);
};
