#pragma once
#include "State.h"
#include <vector>
#include <map>

using namespace std;

class PropertyState
{
private:
	int id;
	bool accept;
	vector<pair<bool (*)(State, map<string, int>), PropertyState*>> edges;
public:
	PropertyState(int i) : id(i) {}
	int getId() { return id; }
	bool isAccept() { return accept; }
	void setAccept(bool a) { accept = a; }
	vector<pair<bool (*)(State, map<string, int>), PropertyState*>> getEdges() { return edges; }
	void setEdges(vector<pair<bool (*)(State, map<string, int>), PropertyState*>> vec) { edges = vec; }
	bool equals(PropertyState* other)
	{
		return id == other->id;
	}
	vector<PropertyState*> getSuccessors(State s, map<string, int> e)
	{
		vector<PropertyState*> vec;
		for(auto edge : edges)
		{
			if((edge.first)(s, e))
			{
				vec.push_back(edge.second);
			}
		}
		return vec;
	}
};