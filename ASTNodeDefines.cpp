#include "ASTNodeDefines.h"
#include "Visitor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

string opToString(operators o)
{
    switch(o)
    {
    case Add:
        return "+";
    case Sub:
        return "-";
    case Or:
        return "||";
    case And:
        return "&&";
    case Lt:
        return "<";
    case Gt:
        return ">";
    case Lte:
        return "<=";
    case Gte:
        return ">=";
    case Eq:
        return "==";
    case Neq:
        return "!=";
    case Mult:
        return "*";
    case Not:
        return "!";
    default:
        return "Not a legal operator";
    }
}

void ListNode::append(ASTNode* last)
{
    next ? ((ListNode*)next)->append(last) : setNext(last);
}

void ListNode::accept(Visitor* v)
{
    if (next) next->accept(v);
}

void ListifiedNode::accept(Visitor* v)
{
    if(v->previsit((ListifiedNode*)this))
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes.at(i)) nodes.at(i)->accept(v);
        }
    }
    v->visit((ListifiedNode*)this);
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes.at(i)) nodes.at(i)->accept(v);
    }
    v->postvisit((ListifiedNode*)this);
}

string ListifiedNode::toString()
{
    stringstream ss;
    for(int i = 0; i < nodes.size(); i++)
    {
        if (nodes.at(i))
        {
            ss << nodes.at(i)->toString();
            if (i != nodes.size() - 1) ss << ", ";
        }
    }
    return ss.str();
}

void ProgramNode::accept(Visitor* v)
{
    if(v->previsit((ProgramNode*)this))
    {
        if(globals) globals->accept(v);
        if(functions) functions->accept(v);
    }
    v->visit((ProgramNode*)this);
    if(globals) globals->accept(v);
    if(functions) functions->accept(v);
}

string ProgramNode::toString()
{
    return "";
}

void DeclarationNode::accept(Visitor* v)
{
    v->visit((DeclarationNode*)this);
    ListNode::accept(v);
}

string DeclarationNode::toString()
{
    return type + " " + id + ";\n";
}

void FunctionNode::accept(Visitor* v)
{
    if(v->previsit((FunctionNode*)this))
    {
        if(args) args->accept(v);
        if(body) body->accept(v);
    }
    v->visit((FunctionNode*)this);
    if(args) args->accept(v);
    if(body) body->accept(v);
    ListNode::accept(v);
}

string FunctionNode::toString()
{
    stringstream ss;
    ss << type << " " << id << "(";
    ss << ((args) ? args->toString() : "");
    ss << ")\n{";
    ss << ((body) ? body->toString() : "");
    ss << "\n}\n";
    return ss.str();
}

void ArgumentNode::accept(Visitor* v)
{
    v->visit((ArgumentNode*)this);
    ListNode::accept(v);
}

string ArgumentNode::toString()
{
    stringstream ss;
    ss << type << " " << id;
    if(getNext())
    {
        ss << ", " << getNext()->toString();
    }
    return ss.str();
}

void FunctionBodyNode::accept(Visitor* v)
{
    if(v->previsit((FunctionBodyNode*)this))
    {
        if(variables) variables->accept(v);
        if(statements) statements->accept(v);
    }
    v->visit((FunctionBodyNode*)this);
    if(variables) variables->accept(v);
    if(statements) statements->accept(v);
}

string FunctionBodyNode::toString()
{
    return "";
}

void FunctionCallNode::accept(Visitor* v)
{
    if(v->previsit((FunctionCallNode*)this))
    {
        if(values) values->accept(v);
    }
    v->visit((FunctionCallNode*)this);
    if(values) values->accept(v);
    ListNode::accept(v);
    v->postvisit((FunctionCallNode*)this);
}

string FunctionCallNode::toString()
{
    stringstream ss;
    ss << id << "(";
    ss << ((values) ? values->toString() : "");
    ss << ");\n";
    return ss.str();
}

string ValueNode::getType()
{
    switch (type)
    {
        case INTV:
            return "int";
        case STRINGV:
            return "string";
        case IDV:
            return "id";
        case CHARV:
            return "char";
    }
}

string ValueNode::getValue()
{
    stringstream ss;
    switch (type)
    {
        case INTV:
            ss << intval;
            break;
        case STRINGV:
            ss << "\"" << stringval << "\"";
            break;
        case IDV:
            ss << stringval;
            break;
        case CHARV:
            ss << "\'" << charval << "\'";
            break;
    }
    return ss.str();
}

void ValueNode::accept(Visitor* v)
{
    v->visit((ValueNode*)this);
}

string ValueNode::toString()
{
    return getValue();
}

void ValueListNode::accept(Visitor* v)
{
    v->visit((ValueListNode*)this);
    if (next) next->accept(v);
}

void ValueListNode::append(ASTNode* last)
{
    next ? ((ListNode*)next)->append(last) : setNext(last);
}

string ValueListNode::toString()
{
    stringstream ss;
    ss << getValue();
    if (getNext())
    {
        ss << ", " << getNext()->toString();
    }
    return ss.str();
}

void ValueTermNode::accept(Visitor* v)
{
    v->visit((ValueTermNode*)this);
}

void AssignmentNode::accept(Visitor* v)
{
    if(v->previsit((AssignmentNode*)this))
    {
        if(term) term->accept(v);
    }
    v->visit((AssignmentNode*)this);
    if(term) term->accept(v);
    ListNode::accept(v);
}

string AssignmentNode::toString()
{
    stringstream ss;
    ss << id << " = " << term->toString();
    return ss.str();
}


void BinopNode::accept(Visitor* v)
{
    
    if(v->previsit((BinopNode*)this))
    {
        if(lhs) lhs->accept(v);
        if(rhs) rhs->accept(v);
    }
    
    v->visit((BinopNode*)this);
    if(lhs) lhs->accept(v);
    if(rhs) rhs->accept(v);
}

string BinopNode::toString()
{
    stringstream ss;
    ss << lhs->toString();
    ss << " " << opToString(op) << " ";
    ss << rhs->toString();
    return ss.str();
}

void UnopNode::accept(Visitor* v)
{
    
    if(v->previsit((UnopNode*)this))
    {
        if(rhs) rhs->accept(v);
    }
    
    v->visit((UnopNode*)this);
    if(rhs) rhs->accept(v);
}

string UnopNode::toString()
{
    stringstream ss;
    ss << opToString(op);
    ss << rhs->toString();
    return ss.str();
}

void WhileNode::accept(Visitor* v)
{
    if(v->previsit((WhileNode*)this))
    {
        if(condition) condition->accept(v);
        if(body) body->accept(v);
    }
    v->visit((WhileNode*)this);
    if(condition) if(condition) condition->accept(v);
    if(body) body->accept(v);
    v->postvisit((WhileNode*)this);
    ListNode::accept(v);
}

string WhileNode::toString()
{
    stringstream ss;
    ss << "while(";
    ss << condition->toString();
    ss << ")\n{";
    ss << body->toString();
    ss << "\n}\n";
    return ss.str();
}

void IfNode::accept(Visitor* v)
{
    if(v->previsit((IfNode*)this))
    {
        if(condition) condition->accept(v);
        if(body) body->accept(v);
        if(elseBody) elseBody->accept(v);
    }
    v->visit((IfNode*)this);
    if(condition) condition->accept(v);
    if(body) body->accept(v);
    v->postvisit((IfNode*)this);
    if(elseBody) elseBody->accept(v);
    ListNode::accept(v);
}

string IfNode::toString()
{
    stringstream ss;
    ss << "if(";
    ss << condition->toString();
    ss << ")\n{";
    ss << body->toString();
    ss << "\n}";
    if (elseBody) ss << elseBody->toString();
    return ss.str();
}

void ElseNode::accept(Visitor* v)
{
    if(v->previsit((ElseNode*)this))
    {
        if(body) body->accept(v);
    }
    v->visit((ElseNode*)this);
    if(body) body->accept(v);
    v->postvisit((ElseNode*)this);
}

string ElseNode::toString()
{
    stringstream ss;
    ss << " else {\n";
    ss << body->toString();
    ss << "\n}\n";
    return ss.str();
}

void ReturnNode::accept(Visitor* v)
{
    
    if(v->previsit((ReturnNode*)this))
    {
        if(value) value->accept(v);
    }
    
    v->visit((ReturnNode*)this);
    if(value) value->accept(v);
    ListNode::accept(v);
}

string ReturnNode::toString()
{
    return "return" + value->toString() + ";\n";
}