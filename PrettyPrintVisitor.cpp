#include "Visitor.h"
#include <string>
#include <sstream>

using namespace std;

void PrettyPrintVisitor::visit(ASTNode* n)
{
    cout << "not implemented" << endl;
}

// do nothing, is abstract
void PrettyPrintVisitor::visit(ProgramNode* n) {}

void PrettyPrintVisitor::visit(DeclarationNode* n)
{
    if (!decs) return;
    stringstream ss;
    ss << contents;
    ss << "global " << n->toString();

    contents = ss.str();
}

void PrettyPrintVisitor::postvisit(ListifiedNode* n)
{
    stringstream ss;
    ss << contents;
    //ss << endl;
    contents = ss.str();
}

void PrettyPrintVisitorHelper::postvisit(ListifiedNode* n)
{
    stringstream ss;
    ss << contents;
    contents = ss.str();
}

void PrettyPrintVisitor::visit(FunctionNode* n)
{
    stringstream ss;
    ss << contents;
    decs = false;

    ss << n->getType() << " " << n->getID() << "(";
    if (n->getArgs())
    {
        PrettyPrintVisitorHelper* helper = new PrettyPrintVisitorHelper();
        n->getArgs()->accept(helper);
        helper->trim();
        ss << helper->getContents();
        delete helper;
    }
    ss << ")\n{\n";
    if (n->getBody())
    {
        PrettyPrintVisitorHelper* helper = new PrettyPrintVisitorHelper();
        n->getBody()->accept(helper);
        ss << helper->getContents();
        delete helper;
    }
    ss << "}\n";

    contents = ss.str();
}

void PrettyPrintVisitorHelper::visit(ArgumentNode* n)
{
    stringstream ss;
    ss << contents;

    ss << n->getType() << " " << n->getID();
    ss << ", ";

    contents = ss.str();
}

void PrettyPrintVisitorHelper::trim()
{
    if (contents == "" || contents[contents.size() - 1] != ' ') return;
    contents = contents.substr(0, contents.size() - 2);
}

void PrettyPrintVisitorHelper::visit(FunctionBodyNode* n)
{
    tablevel = 1;
}


void PrettyPrintVisitorHelper::visit(DeclarationNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << n->toString();

    if (!n->getNext()) ss << "\n";

    contents = ss.str();
}

void PrettyPrintVisitorHelper::visit(FunctionCallNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << n->getID() << "(";
    if (n->getValues())
    {
        ss << n->getValues()->toString();
    }
    ss << ");\n";

    contents = ss.str();
    if (templevel)
    {
        tablevel = templevel;
        templevel = 0;
    }
}

void PrettyPrintVisitorHelper::visit(ValueTermNode* n)
{
    stringstream ss;
    ss << contents;

    ss << n->toString() << ";\n";

    contents = ss.str();
    if (templevel)
    {
        tablevel = templevel;
        templevel = 0;
    }
}

void PrettyPrintVisitorHelper::visit(AssignmentNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    templevel = tablevel;
    tablevel = 0;

    ss << n->getID() << " = ";

    contents = ss.str();
}

void PrettyPrintVisitorHelper::visit(BinopNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << n->toString() << ";\n";

    contents = ss.str();
    if (templevel)
    {
        tablevel = templevel;
        templevel = 0;
    }
}

void PrettyPrintVisitorHelper::visit(UnopNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << n->toString() << ";\n";

    contents = ss.str();
    if (templevel)
    {
        tablevel = templevel;
        templevel = 0;
    }
}

void PrettyPrintVisitorHelper::visit(WhileNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "while (" << n->getCondition()->toString() << ")\n";

    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }
    ss << "{\n";

    tablevel++;

    contents = ss.str();
}

bool PrettyPrintVisitorHelper::previsit(IfNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "if (" << n->getCondition()->toString() << ")\n";

    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }
    ss << "{\n";

    return false;
}

void PrettyPrintVisitorHelper::visit(IfNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "if (" << n->getCondition()->toString() << ")\n";

    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }
    ss << "{\n";

    tablevel++;

    contents = ss.str();
}

void PrettyPrintVisitorHelper::visit(ElseNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "else\n";

    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }
    ss << "{\n";

    tablevel++;

    contents = ss.str();
}

void PrettyPrintVisitorHelper::visit(ReturnNode* n)
{
    stringstream ss;
    ss << contents;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "return " << n->getValue()->toString() << ";\n";

    contents = ss.str();
}

void PrettyPrintVisitorHelper::postvisit(WhileNode* n)
{
    stringstream ss;
    ss << contents;
    tablevel--;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "}\n";

    contents = ss.str();
}

void PrettyPrintVisitorHelper::postvisit(IfNode* n)
{
    stringstream ss;
    ss << contents;
    tablevel--;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "}\n";

    contents = ss.str();
}

void PrettyPrintVisitorHelper::postvisit(ElseNode* n)
{
    stringstream ss;
    ss << contents;
    tablevel--;
    for (int i = 0; i < tablevel; i++)
    {
        ss << "\t";
    }

    ss << "}\n";

    contents = ss.str();
}
