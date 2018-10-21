#pragma once
#include "ASTNodeDefines.h"
#include "State.h"
#include "StoreEntry.h"
#include <map>
#include <set>
#include <string>

using namespace std;

class CESKInterpreter
{
private:
    map<int, StoreEntry*> store;
    map<string, int> globalEnv;
	set<string> standard;
    ProgramNode* head;
    State current;
    int nextVar;
    int nextK;

    State step();
    ASTNode* getFunction(string id);
	ValueNode* dereferenceID(ValueNode* n);
	ValueNode* performUnop(UnopNode* n);
	ValueNode* performBinop(BinopNode* n);
	void fillStandard();
	void standardPrint(Control, map<string, int>&, int);
	void writeValue(string id, ValueNode* val, map<string, int> localEnv);
public:
    CESKInterpreter(ASTNode* h);
    void interpret()
    { 
        // yes, these statements are intentional
        cout << "--Execution Starting--" << endl;
        try
        {
			while (true)
            {
				current = step();
			}
        }
        catch(string s)
        {
			cout << s << endl;
            cout << "--Execution Finished--" << endl;
        }
		catch(char const* s)
        {
			cout << s << endl;
            cout << "--Execution Finished--" << endl;
        }
    }
};
