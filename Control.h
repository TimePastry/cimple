#include "ASTNodeDefines.h"
#include <stack>

using namespace std;

/*
 * Control class for the CESK states
 * next_____ functions defined for producing the next control depending on the current type of statement
 * stacks of Nodes and ints used to simulate "scope" (even though there is no actual scoping happening
 * when a compound block is entered (e.g. while, if, etc.), the new block is pushed on to the stack with
 * the previous block remaining underneath
 * the wide variation in how different statements treat compound blocks is the cause for so many different
 * "next" functions
 */
class Control
{
private:
    stack<ListifiedNode*> statements;
    stack<int> statementIndex;

    void pop()
    {
        statements.pop();
        statementIndex.pop();
    }
public:
    Control() {}
    Control(ListifiedNode* block) { statements.push(block); statementIndex.push(0); }
    Control(Control* other)
    {
        statements = other->statements;
        statementIndex = other->statementIndex;
    }

	bool equals(Control other)
	{
		return ((statements == other.statements) && (statementIndex == other.statementIndex));
	}

	string toString()
	{
		return getStatement()->toString();
	}

    ASTNode* getStatement()
    {
        return statements.top()->at(statementIndex.top());
    }

	int getLineNumber()
	{
		return getStatement()->getLineNum();
	}

    Control next()
    {
        Control next(this);
        
        int index = statementIndex.top() + 1;
        
        next.statementIndex.pop();
        next.statementIndex.push(index);
        
        if (index == statements.top()->getLength())
        {
			next.statementIndex.pop();
			next.statements.pop();
        }

        return next;
    }

    Control nextWhile(ListifiedNode* block)
    {
        Control next(this);

        next.statements.push(block);
        next.statementIndex.push(0);

        return next;
    }

    Control nextIf(ListifiedNode* block, bool followingElse = false)
    {
        Control next(this);
        
        int index = statementIndex.top() + 1;
        ASTNode* nextNode = statements.top()->at(index);
        if (dynamic_cast<ElseNode*>(nextNode)) index++;

        next.statementIndex.pop();
        next.statementIndex.push(index);

        if (index == statements.top()->getLength())
        {
			next.statementIndex.pop();
			next.statements.pop();
        }

        next.statements.push(block);
        next.statementIndex.push(0);

        return next;
    }

    Control nextElse(ListifiedNode* block)
    {
        Control next(this);
        
        int index = statementIndex.top() + 1;

        next.statementIndex.pop();
        next.statementIndex.push(index);

        if (index == statements.top()->getLength())
        {
			next.statementIndex.pop();
			next.statements.pop();
        }

        next.statements.push(block);
        next.statementIndex.push(0);

        return next;
    }

};
