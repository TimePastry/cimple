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
    // address into the store that the return value will be written to
    // 0 means the function does not write the result to a position in the store
    int returnAddr;
public:
    Continuation(Control control, map<string, int> environment, int address, int ret = 0) : c(control), env(environment), kontAddr(address), returnAddr(ret) {}
    
    Control getControl() { return c; }
    map<string, int> getEnvironment() { return env; }
    int getAddress() { return kontAddr; }
    int getReturnAddress() { return returnAddr; }
};
