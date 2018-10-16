#pragma once
#include "ASTNodeDefines.h"
#include "Control.h"
#include <string>
#include <map>

using namespace std;

class State
{
private:
    Control c;
    map<string, int> e;
    // store is global
    // k is a continuation address
    int k;
public:
    State(Control cp, map<string, int> ep, int kp) : c(cp), e(ep), k(kp) {}

    Control getControl() { return c; }
    map<string, int> getEnvironment() { return e; }
    int getKontinuation() { return k; }
};
