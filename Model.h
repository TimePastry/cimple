#pragma once
#include "CESKSearch.h"
#include "Property.h"
#include "ModelState.h"
#include <vector>
#include <set>

using namespace std;

class Model
{
private:
	unsigned int times = 0;
    CESKSearch program;
    Property property;
    vector<ModelState*> states;

	bool find(ModelState*& state)
	{
		for(auto other : states)
		{
			if (state->equivalent(other))
			{
				delete state;
				state = other;
				return true;
			}
		}
		return false;
	}

public:
	Model(CESKSearch s, Property p) : program(s), property(p) {}

	void check()
	{
		ModelState* ms = new ModelState();
		ms->prop = property.firstState();
		try
		{
			dfs1(ms);
		}
		catch (string s)
		{
			cout << "Trace found: " << endl;
			cout << s << endl;
			return;
		}
		cout << "The program cannot satisfy the given property" << endl;
	}

	void dfs1(ModelState* q)
	{
		try
		{
			q->onStack = true;
			State* s = q->cesk;
			q->onStack = true;
			states.push_back(q);

			//what if s == NULL
			vector<State*> ceskSucc = program.step(s);
			for (auto sprime : ceskSucc)
			{
				vector<PropertyState*> propSucc = q->prop->getSuccessors(*sprime, program.getGlobalEnv());
				for (auto pprime : propSucc)
				{
					ModelState* qprime = new ModelState();
					qprime->cesk = sprime;
					qprime->prop = pprime;
					bool found = find(qprime);
					q->successors.push_back(qprime);
					if (!found)
					{
						dfs1(qprime);
					}
				}
			}
			if (q->isAccept())
			{
				try
				{
					dfs2(q);
				}
				catch (bool b)
				{
					throw "line " + to_string(s->getControl().getLineNumber());
				}
			}
			q->onStack = false;
		}
		catch (string str)
		{
			//cout << q->prop->getId() << endl;
			if (q->cesk) throw "line " + to_string(q->cesk->getControl().getLineNumber()) + "\n" + str;
			throw str;
		}
	}

	void dfs2(ModelState* q)
	{
		q->flagged = true;
		for (auto qprime : q->successors)
		{
			if (qprime->onStack)
			{
				throw true;
			}
			else if (!(qprime->flagged))
			{
				dfs2(qprime);
			}
		}
	}
};

//pseudocode from slide:
/*
procedure dfs1(q)
   local q’;
   hash(q);
   for all successors q’ of q do
      if q’ not in the hash table then dfs1(q’);
   if accept(q) then dfs2(q);
end procedure


procedure dfs2(q)
   local q’;
   flag(q);
   for all successors q’ of q do
      if q’ on dfs1 stack then terminate(True);
      else if q’ not flagged then dfs2(q’)
      endif
end procedure
*/
