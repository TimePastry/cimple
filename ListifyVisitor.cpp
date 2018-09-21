#include "Visitor.h"
#include "ASTNodeDefines.h"
#include <vector>

void ListifyVisitor::visit(ProgramNode* n)
{
    vector<ASTNode*> globals = buildList((ListNode*)(n->getGlobals()));
    n->setGlobals(new ListifiedNode(globals));
    vector<ASTNode*> functions = buildList((ListNode*)(n->getFunctions()));
    functions.insert(functions.begin(), NULL);
    n->setFunctions(new ListifiedNode(functions));
}

void ListifyVisitor::visit(FunctionNode* n)
{
    vector<ASTNode*> args = buildList((ListNode*)(n->getArgs()));
    n->setArgs(new ListifiedNode(args));
}

void ListifyVisitor::visit(FunctionBodyNode* n)
{
    vector<ASTNode*> statements = buildList((ListNode*)(n->getStatements()));
    n->setStatements(new ListifiedNode(statements));
}

void ListifyVisitor::visit(FunctionCallNode* n)
{
    vector<ASTNode*> values = buildList((ValueListNode*)(n->getValues()));
    n->setValues(new ListifiedNode(values));
}

void ListifyVisitor::visit(WhileNode* n)
{
    vector<ASTNode*> body = buildList((ListNode*)(n->getBody()));
    n->setBody(new ListifiedNode(body));
}

void ListifyVisitor::visit(IfNode* n)
{
    vector<ASTNode*> body = buildList((ListNode*)(n->getBody()));
    n->setBody(new ListifiedNode(body));
}

void ListifyVisitor::visit(ElseNode* n)
{
    vector<ASTNode*> body = buildList((ListNode*)(n->getBody()));
    n->setBody(new ListifiedNode(body));
}

vector<ASTNode*> ListifyVisitor::buildList(ListNode* n)
{
    if (!n) return vector<ASTNode*>();
    vector<ASTNode*> toReturn = buildList((ListNode*)(n->getNext()));
    n->setNext(NULL);
    toReturn.insert(toReturn.begin(), n);
    return toReturn;
}

vector<ASTNode*> ListifyVisitor::buildList(ValueListNode* n)
{
    if (!n) return vector<ASTNode*>();
    vector<ASTNode*> toReturn = buildList((ValueListNode*)(n->getNext()));
    n->setNext(NULL);
    toReturn.insert(toReturn.begin(), n);
    return toReturn;
}
