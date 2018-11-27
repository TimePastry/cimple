#pragma once
#include "ASTNodeDefines.h"
#include "State.h"
#include "StoreEntry.h"
#include <map>
#include <stack>
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
	stack<State> workStack;
    State current;
    int nextVar;
    int nextK;

    State step();
    ASTNode* getFunction(string id);
	ValueNode* dereferenceID(ValueNode* n, map<string, int> e,  map<int, StoreEntry*> s);
	ValueNode* performUnop(UnopNode* n, map<string, int> e, map<int, StoreEntry*> s);
	ValueNode* performBinop(BinopNode* n, map<string, int> e, map<int, StoreEntry*> s);
	void fillStandard();
	void standardPrint(Control, map<string, int>&, int);
	void writeValue(string id, ValueNode* val, map<string, int>& localEnv,  map<int, StoreEntry*>& s);
	void performStandard(FunctionCallNode* fNode, string funid, map<string, int>& e, map<int, StoreEntry*>& s, string assignmentId);
	State stepDeclaration(DeclarationNode* dNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	State stepFunctionCall(FunctionCallNode* fNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	State stepAssignment(AssignmentNode* fNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	State stepWhile(WhileNode* wNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	State stepIf(IfNode* iNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	State stepElse(ElseNode* eNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	State stepReturn(ReturnNode* rNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);

public:
    CESKInterpreter(ASTNode* h);
    void interpret()
    { 
        // yes, these statements are intentional
        cout << "--Execution Starting--" << endl;
        try
        {
			while (!workStack.empty())
            {
				current = workStack.top();
				workStack.pop();
				workStack.push(step());
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
