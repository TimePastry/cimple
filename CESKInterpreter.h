#pragma once
#include "ASTNodeDefines.h"
#include "State.h"
#include "StoreEntry.h"
#include <map>
#include <string>

using namespace std;

class CESKInterpreter
{
private:
    map<int, StoreEntry*> store;
    map<string, int> globalEnv;
    ProgramNode* head;
    State current;
    int nextVar;
    int nextK;

    State step();
    ASTNode* getFunctionBody(string id);
public:
    void init(ASTNode* h);
    void interpret()
    { 
        // yes, these statements are intentional
        cout << "--Execution Starting--" << endl;
        try
        {
            current = step();
        }
        catch(...)
        {
            cout << "--Execution Finished--" << endl;
        }
    }
};
