#pragma once
#include "ASTNodeDefines.h"
#include "Control.h"
#include "StoreEntry.h"
#include <string>
#include <map>

using namespace std;

class State
{
private:
    Control c;
    map<string, int> e;
    // store is NO LONGER global
    map<int, StoreEntry*> s;
    // k is a continuation address
    int k;
public:
	State() {}
    State(Control cp, map<string, int> ep, map<int, StoreEntry*> sp, int kp) : c(cp), e(ep), s(sp), k(kp) {}
	State(Control cp, map<string, int> ep, int kp) : c(cp), e(ep), k(kp) {}

    Control getControl() { return c; }
    map<string, int> getEnvironment() { return e; }
    map<int, StoreEntry*> getStore() { return s; }
    int getKontinuation() { return k; }
	bool storeMatch(State* first, State* second)
	{
		if (first->e != second->e) return false;
		// we're going to assume global environments are already equal
		// that doesn't work super nice but okay
		for (auto entry : first->e)
		{
			if (first->s[entry.second] != second->s[entry.second]) return false;
		}
		// I don't think I need recursion because the continuation addresses being the same should imply that they are going back to the same spot
		return true;
	}
	bool equals(State* other)
	{
		/*
		string blank;

		if ((storeMatch(this, other)))
		{
			cout << "s match:" << endl;
			if (c.equals(other->c))
			{
				cout << "c match:" << endl;
				if ((e == other->e))
				{
					cout << "e match:" << endl;
					
				}
				else
				{
					cout << "e not match" << endl;
				}
			}
			else
			{
				cout << "c not match" << endl;
			}
			cin >> blank;
		}
		else
		{
			cout << "s not match" << endl;
		}
		
		return false;
		*/
/*
		if (c.equals(other->c) && (e == other->e) && (storeMatch(this, other)) && (k == other->k))
		{
			cout << "MATCH" << endl;
			return true;
		}
		return false;
*/
		return (c.equals(other->c) && (e == other->e) && (storeMatch(this, other)) && (k == other->k));
	}
};
