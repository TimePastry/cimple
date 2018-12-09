#pragma once
#include "CESKInterpreter.h"
#include "State.h"

using namespace std;

class ModelState
{
private:
    State* cesk = NULL;
    PropertyState* prop = NULL;
    bool onStack = false;
    bool flagged = false;
	vector<ModelState*> successors;
    friend class Model;
public:
	bool equivalent(ModelState* other)
	{
		if(!other) return false;
		if(!(other->cesk)) return !cesk;
		return (cesk->equals(other->cesk)) && (prop->equals(other->prop));
	}

	void addSuccessor(ModelState* succ)
	{
		successors.push_back(succ);
	}

	bool isAccept() { return prop->isAccept(); }
};
