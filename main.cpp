#include "ASTNodeDefines.h"
#include "Visitor.h"
#include <iostream>
#include "SymbolTableVisitor.h"
#include "TypeCheckVisitor.h"
#include "CESKInterpreter.h"
#include "Model.h"
#include <cstring>

using namespace std;

ASTNode* parse(char*);

string getSymbolType(ASTNode* n)
{
    if (dynamic_cast<DeclarationNode*>(n))
    {
        return ((DeclarationNode*)n)->getType();
    }
    if (dynamic_cast<FunctionNode*>(n))
    {
        return ((FunctionNode*)n)->getType();
    }
    if (dynamic_cast<ArgumentNode*>(n))
    {
        return ((ArgumentNode*)n)->getType();
    }
}

void printSymbolTable(vector<map<string, ASTNode*> > table)
{
    for (int i = 0; i < table.size(); i++)
    {
        cout << "Table " << i << ":\n";
        for(auto j : table.at(i))
        {
            cout << "\t" << j.first << "\t\t:\t" << getSymbolType(j.second) << endl;
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

    ASTNode* head = parse(argv[1]);

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
            TypeCheckVisitor* tcv;
            
            cout << "running SymbolTableVisitor, then TypeCheckVisitor..." << endl;
            ListifyVisitor* lv = new ListifyVisitor();
            ((ProgramNode*)head)->accept(lv);
            SymbolTableVisitor* stv = new SymbolTableVisitor();
            ((ProgramNode*)head)->accept(stv);
            if (i + 1 < argc && !strcmp(argv[i + 1], "-v"))
            {
                tcv = new TypeCheckVisitor(stv->getRootTable(), true);
            } else 
            {
                tcv = new TypeCheckVisitor(stv->getRootTable(), false);
            }
            try{
                ((ProgramNode*)head)->accept(tcv);
            } catch (string s)
            {
                cout << "caught exception:" << endl;
                cout << s << endl;
                cout << "exiting" << endl;
                exit(-1);
            }
            catch (const char* s)
            {
                cout << "caught exception:" << endl;
                cout << s << endl;
                cout << "exiting" << endl;
                exit(-1);
            }
            cout << "finished" << endl;
        }
		else if (!strcmp(argv[i], "-cesk"))
		{
			TypeCheckVisitor* tcv;
            
            cout << "Preparing for execution..." << endl;
            ListifyVisitor* lv = new ListifyVisitor();
            ((ProgramNode*)head)->accept(lv);
            SymbolTableVisitor* stv = new SymbolTableVisitor();
            ((ProgramNode*)head)->accept(stv);
            if (i + 1 < argc && !strcmp(argv[i + 1], "-v"))
            {
                tcv = new TypeCheckVisitor(stv->getRootTable(), true);
            } else 
            {
                tcv = new TypeCheckVisitor(stv->getRootTable(), false);
            }
            try{
                ((ProgramNode*)head)->accept(tcv);
            } catch (string s)
            {
                cout << "caught exception:" << endl;
                cout << s << endl;
                cout << "exiting" << endl;
                exit(-1);
            }
            catch (const char* s)
            {
                cout << "caught exception:" << endl;
                cout << s << endl;
                cout << "exiting" << endl;
                exit(-1);
            }

			cout << "Interpreting..." << endl;
			CESKInterpreter interp = CESKInterpreter(head);
			interp.interpret();
		}
		else if (!strcmp(argv[i], "-mc"))
		{
			TypeCheckVisitor* tcv;
            
            cout << "Preparing for execution..." << endl;
			cout << "This may take some time, please wait..." << endl;
            ListifyVisitor* lv = new ListifyVisitor();
            ((ProgramNode*)head)->accept(lv);
            SymbolTableVisitor* stv = new SymbolTableVisitor();
            ((ProgramNode*)head)->accept(stv);
            if (i + 1 < argc && !strcmp(argv[i + 1], "-v"))
            {
				i++;
                tcv = new TypeCheckVisitor(stv->getRootTable(), true);
            } else 
            {
                tcv = new TypeCheckVisitor(stv->getRootTable(), false);
            }
			// type checking
			try{
                ((ProgramNode*)head)->accept(tcv);
            } catch (string s)
            {
                cout << "caught exception:" << endl;
                cout << s << endl;
                cout << "exiting" << endl;
                exit(-1);
            }
            catch (const char* s)
            {
                cout << "caught exception:" << endl;
                cout << s << endl;
                cout << "exiting" << endl;
                exit(-1);
            }

			Property p;
			CESKSearch s = CESKSearch(head);
			if (i + 1 < argc && !strcmp(argv[i + 1], "-reach"))
			{
				i++;
				p.constructReachability();
			}
			else if (i + 1 < argc && !strcmp(argv[i + 1], "-farmer"))
			{
				i++;
				p.constructFarmer();
			}
			else
			{
				cout << "Unknown or unspecified property" << endl;
				return -1;
			}
			Model model(s, p);
			model.check();
		}
    }

    cout << "exiting" << endl;

    return 0;
}
