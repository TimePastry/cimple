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
		PropertyState* initial = new PropertyState(0);
		PropertyState* other = new PropertyState(1);

		initial->setAccept(false);
		other->setAccept(true);

		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> initialEdges;
		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> otherEdges;

		initialEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(constantTrue, initial));
		initialEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(lineEighteen, other));

		otherEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(constantTrue, other));

		initial->setEdges(initialEdges);
		other->setEdges(otherEdges);

		states.push_back(initial);
		states.push_back(other);
	}

	void constructFarmer()
	{
		PropertyState* zero = new PropertyState(0);
		PropertyState* one = new PropertyState(1);
		PropertyState* two = new PropertyState(2);

		zero->setAccept(false);
		one->setAccept(false);
		two->setAccept(true);

		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> zeroEdges;
		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> oneEdges;
		vector<pair<bool (*)(State, map<string, int>), PropertyState*>> twoEdges;

		zeroEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(nonTerminalFarmer, zero));
		zeroEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(loseFarmer, one));
		zeroEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(winFarmer, two));

		oneEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(constantTrue, one));

		twoEdges.push_back(pair<bool (*)(State, map<string, int>), PropertyState*>(constantTrue, two));

		zero->setEdges(zeroEdges);
		one->setEdges(oneEdges);
		two->setEdges(twoEdges);

		states.push_back(zero);
		states.push_back(one);
		states.push_back(two);
	}

	PropertyState* firstState()
	{
		if (states.size() < 1) throw "empty property";
		return states.at(0);
	}
};
