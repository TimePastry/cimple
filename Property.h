#pragma once
#include <vector>
#include "PropertyState.h"

using namespace std;
//bool (*)(State, map<string, int>)

bool allDefined(State s)
{
	map<string, int> e = s.getEnvironment();
	if(e.find("wolf") == e.end()) return false;
	if(e.find("chicken") == e.end()) return false;
	if(e.find("seed") == e.end()) return false;
	if(e.find("farmer") == e.end()) return false;
	return true;
}

bool constantTrue(State s, map<string, int> e)
{
	return true;
}

bool lineEighteen(State s, map<string, int> e)
{
	Control c = s.getControl();
	return c.getLineNumber() == 18;
}

bool winFarmer(State s, map<string, int> e)
{
	bool wolf, chicken, seed;
	int storeaddr;

	Control c = s.getControl();
	if (c.getLineNumber() != 132) return false;

	if(!allDefined(s)) return false;
	
	storeaddr = s.getEnvironment()["wolf"];
	if (!storeaddr) {  return false;} 
	StoreEntry* se = s.getStore()[storeaddr];
	if (!se) { return false;} 
	ValueNode* val = se->getVal();
	wolf = val->getIntVal();

	storeaddr = s.getEnvironment()["chicken"];
	if (!storeaddr) { return false;} 
	se = s.getStore()[storeaddr];
	if (!se) { return false; } 
	val = se->getVal();
	chicken = val->getIntVal();

	storeaddr = s.getEnvironment()["seed"];
	if (!storeaddr) { return false;} 
	se = s.getStore()[storeaddr];
	if (!se) { return false; } 
	val = se->getVal();
	seed = val->getIntVal();

	return wolf && chicken && seed;
}

bool loseFarmer(State s, map<string, int> e)
{
	bool wolf, chicken, seed, farmer;
	int storeaddr;

	Control c = s.getControl();
	if (c.getLineNumber() != 132) return false;

	if(!allDefined(s)) return false;
	
	storeaddr = s.getEnvironment()["wolf"];
	if (!storeaddr) return false;
	StoreEntry* se = s.getStore()[storeaddr];
	if (!se) { return false; } 
	ValueNode* val = se->getVal();
	wolf = val->getIntVal();

	storeaddr = s.getEnvironment()["chicken"];
	if (!storeaddr) return false;
	se = s.getStore()[storeaddr];
	if (!se) { return false; } 
	val = se->getVal();
	chicken = val->getIntVal();

	storeaddr = s.getEnvironment()["seed"];
	if (!storeaddr) return false;
	se = s.getStore()[storeaddr];
	if (!se) { return false; } 
	val = se->getVal();
	seed = val->getIntVal();

	storeaddr = s.getEnvironment()["farmer"];
	if (!storeaddr) return false;
	se = s.getStore()[storeaddr];
	if (!se) { return false; } 
	val = se->getVal();
	farmer = val->getIntVal();

	return (!wolf && !chicken && farmer) || (wolf && chicken && !farmer) || (!chicken && !seed && farmer) || (chicken && seed && !farmer);
}

bool nonTerminalFarmer(State s, map<string, int> e)
{
	
	return !winFarmer(s, e) && !loseFarmer(s, e);
}


//reaches line whatever

class Property
{
private:
    vector<PropertyState*> states;
public:
    void constructReachability() 
	{
		// define all states
		PropertyState* initial = new PropertyState(0);
		PropertyState* other = new PropertyState(1);

		// set accept is applicable for each state
		initial->setAccept(false);
		other->setAccept(true);

		// define vectors for edges for each state
		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> initialEdges;
		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> otherEdges;

		// for the first state, define the edge by giving a function and a destination state
		initialEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(constantTrue, initial));
		initialEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(lineEighteen, other));

		// continue for all states
		otherEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(constantTrue, other));

		// set the vectors into the states
		initial->setEdges(initialEdges);
		other->setEdges(otherEdges);

		// add the states to the property
		states.push_back(initial);
		states.push_back(other);
	}

	void constructFarmer()
	{
		// define all states
		PropertyState* zero = new PropertyState(0);
		PropertyState* two = new PropertyState(2);

		// set accept is applicable for each state
		zero->setAccept(false);
		two->setAccept(true);

		// define vectors for edges for each state
		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> zeroEdges;
		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> twoEdges;

		// for the first state, define the edge by giving a function and a destination state
		zeroEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(nonTerminalFarmer, zero));
		zeroEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(winFarmer, two));

		// continue for all states
		twoEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(constantTrue, two));

		// set the vectors into the states
		zero->setEdges(zeroEdges);
		two->setEdges(twoEdges);

		// add the states to the property
		states.push_back(zero);
		states.push_back(two);
	}

	PropertyState* firstState()
	{
		if (states.size() < 1) throw "empty property";
		return states.at(0);
	}
};
