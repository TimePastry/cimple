#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "ASTNode.h"
#include "ListNode.h"
using namespace std;

enum vtypes { INTV, STRINGV, CHARV, IDV, INTF, STRINGF, CHARF };
enum operators { Add, Sub, Or, And, Lt, Gt, Lte, Gte, Eq, Neq, Mult, Not };

class Visitor;

string opToString(operators o);
string vtypesToString(vtypes v);
vtypes stringToVtypes(string s, bool isFunc = false);

class ASTNode;

class ListNode;

class StatementNode : public ListNode {};

int listLength(ASTNode* n);
