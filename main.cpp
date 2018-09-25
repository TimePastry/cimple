#include "ASTNodeDefines.h"
#include "Visitor.h"
#include <iostream>
#include "SymbolTableVisitor.h"
#include "TypeCheckVisitor.h"
#include <cstring>

using namespace std;

ASTNode* parse(char*);

void printSymbolTable(vector<map<string, ASTNode*> > table)
{
    for (int i = 0; i < table.size(); i++)
    {
        cout << "Table " << i << ":\n";
        for(auto j : table.at(i))
        {
            cout << "\t" << j.first << endl;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "missing file argument. Usage:\n";
        cout << "./exec <filename> <visitor tags...>" << endl;
        exit(-1);
    }

    cout << "starting parsing..." << endl;
    ASTNode* head = parse(argv[1]);
    cout << "finished" << endl;

    for (int i = 2; i < argc; i++)
    {
        if (!strcmp(argv[i], "-id"))
        {
            cout << "running IdentityVisitor..." << endl;
            IdentityVisitor* idv = new IdentityVisitor();
            ((ProgramNode*)head)->accept(idv);
            cout << "finished" << endl;
        } 
        else if (!strcmp(argv[i], "-list"))
        {
            cout << "running ListifyVisitor..." << endl;
            ListifyVisitor* lv = new ListifyVisitor();
            ((ProgramNode*)head)->accept(lv);
            cout << "finished" << endl;
        }
        else if (!strcmp(argv[i], "-pp"))
        {
            cout << "running PrettyPrintVisitor..." << endl;
            PrettyPrintVisitor* ppv = new PrettyPrintVisitor();
            ((ProgramNode*)head)->accept(ppv);
            cout << "program:" << endl;
            cout << ppv->getContents();
            cout << "finished" << endl;
        }
        else if (!strcmp(argv[i], "-st"))
        {
            cout << "running SymbolTableVisitor..." << endl;
            SymbolTableVisitor* stv = new SymbolTableVisitor();
            ((ProgramNode*)head)->accept(stv);
            printSymbolTable(stv->getRootTable());
            cout << "finished" << endl;
        }
        else if (!strcmp(argv[i], "-tc"))
        {
            cout << "running SymbolTableVisitor, then TypeCheckVisitor..." << endl;
            SymbolTableVisitor* stv = new SymbolTableVisitor();
            ((ProgramNode*)head)->accept(stv);
            try{
                TypeCheckVisitor* tcv = new TypeCheckVisitor(stv->getRootTable());
                ((ProgramNode*)head)->accept(tcv);
            } catch (string s)
            {
                cout << "caught exception:" << endl;
                cout << s << endl;
                cout << "exiting" << endl;
                exit(-1);
            }
            cout << "finished" << endl;
        }
    }

    cout << "exiting" << endl;

    return 0;
}
