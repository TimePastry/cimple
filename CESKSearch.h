#pragma once
#include "ASTNodeDefines.h"
#include "State.h"
#include "StoreEntry.h"
#include <map>
#include <stack>
#include <set>
#include <string>
#include <vector>

using namespace std;

class CESKSearch
{
private:
    friend class Model;

	bool initialized;
    map<int, StoreEntry*> store;
    map<string, int> globalEnv;
	set<string> standard;
	ASTNode* abstractHead;
    ProgramNode* head;
    //State current;
    int nextVar;
    int nextK;

    ASTNode* getFunction(string id);
	ValueNode* dereferenceID(ValueNode* n, map<string, int> e,  map<int, StoreEntry*> s);
	ValueNode* performUnop(UnopNode* n, map<string, int> e, map<int, StoreEntry*> s);
	ValueNode* performBinop(BinopNode* n, map<string, int> e, map<int, StoreEntry*> s);
	void fillStandard();
	void standardPrint(Control, map<string, int>&, int);
	void writeValue(string id, ValueNode* val, map<string, int>& localEnv,  map<int, StoreEntry*>& s);
	vector<State*> performStandard(FunctionCallNode* fNode, string funid, Control c, map<string, int>& e, map<int, StoreEntry*>& s, int k, string assignmentId);
	vector<State*> stepDeclaration(DeclarationNode* dNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	vector<State*> stepFunctionCall(FunctionCallNode* fNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	vector<State*> stepAssignment(AssignmentNode* fNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	vector<State*> stepWhile(WhileNode* wNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	vector<State*> stepIf(IfNode* iNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	vector<State*> stepElse(ElseNode* eNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	vector<State*> stepReturn(ReturnNode* rNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k);
	vector<State*> init(ASTNode* h);

public:
    CESKSearch(ASTNode* h);

    vector<State*> step(State* currentPointer);
	map<string, int> getGlobalEnv() { return globalEnv; }
};
