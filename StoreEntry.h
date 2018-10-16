#pragma once
#include "Continuation.h"
#include "ASTNodeDefines.h"

class StoreEntry
{
private:
    bool valueType;
    Continuation* k;
    ValueNode* v;
public:
    StoreEntry() { valueType = false; }
    StoreEntry(Continuation* kont) : valueType(false), k(kont), v(NULL) {}
    StoreEntry(ValueNode* val) : valueType(true), v(val), k(NULL) {}

    bool isValue() { return valueType; }
    Continuation* getKont() { return k; }
    ValueNode* getVal() { return v; }
};
