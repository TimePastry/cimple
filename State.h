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
	bool equals(State* other)
	{
		return (c.equals(other->c) && (e == other->e) && (s == other->s) && (k == other->k));
	}
};
