#pragma once
#include <string>
#include "State.h"
#include <map>

using namespace std;

class Continuation
{
private:
    Control c;
    map<string, int> env;
    int kontAddr;
public:
    Continuation(Control control, map<string, int> environment, int address) : c(control), env(environment), kontAddr(address) {}
    
    Control getControl() { return c; }
    map<string, int> getEnvironment() { return env; }
    int getAddress() { return kontAddr; }
};
