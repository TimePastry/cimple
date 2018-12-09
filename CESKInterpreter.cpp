#include "CESKInterpreter.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

CESKInterpreter::CESKInterpreter(ASTNode* h)
{  
    // HALT continuation 
    store[0] = NULL;
    nextVar = 1;
    nextK = -1;

    fillStandard();
    srand(time(NULL));

    head = (ProgramNode*)h;
    ListifiedNode* globals = (ListifiedNode*)(head->getGlobals());
    for (int i = 0; i < globals->getLength(); i++)
    {
        DeclarationNode* g = (DeclarationNode*)globals->at(i);
        // touch every global variable declaration
        // leave them as default (0) which can be detected as "uninitialized" because it would conflict with the HALT kont
        globalEnv[g->getID()];
    }

    ListifiedNode* functions = (ListifiedNode*)(head->getFunctions());
    FunctionNode* mainNode;
    for (int i = 0; i < functions->getLength(); i++)
    {
        FunctionNode* f = (FunctionNode*)functions->at(i);
        if (!f) continue;
        if (f->getID() == "main")
        {
            mainNode = f;
            break;
        }
    }

    Control c = Control((ListifiedNode*)((FunctionBodyNode*)mainNode->getBody())->getStatements());

    workStack.push(State(c, map<string, int>(), store, 0));
}

void CESKInterpreter::fillStandard()
{
    standard.insert("print");
    standard.insert("printstring");
    standard.insert("printint");
    standard.insert("printchar");
    standard.insert("select");
}

void CESKInterpreter::performStandard(FunctionCallNode* fNode, string funid, map<string, int>& e, map<int, StoreEntry*>& s, string assignmentId = "")
{
	if (funid == "print" || funid == "printstring" || funid == "printint" || funid == "printchar")
	{
		ValueNode* arg = (ValueNode*)((ListifiedNode*)fNode->getValues())->at(0);
		arg = dereferenceID(arg, e, s);
		cout << arg->toString() << endl;

		if (assignmentId != "") writeValue(assignmentId, new ValueNode(1), e, s);
	}
	if (funid == "select")
	{
		ValueNode* arg1 = (ValueNode*)((ListifiedNode*)fNode->getValues())->at(0);
		arg1 = dereferenceID(arg1, e, s);

		ValueNode* arg2 = (ValueNode*)((ListifiedNode*)fNode->getValues())->at(1);
		arg2 = dereferenceID(arg2, e, s);

		int difference = arg2->getIntVal() - arg1->getIntVal();

		int r = (rand() % difference) + arg1->getIntVal();

		if (assignmentId != "") writeValue(assignmentId, new ValueNode(r), e, s);
	}
}

State CESKInterpreter::stepDeclaration(DeclarationNode* dNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k)
{
	c = c.next();

	int allocAddress = nextVar++;
	s[allocAddress];
	e[dNode->getID()] = allocAddress;

	return State(c, e, s, k);
}

State CESKInterpreter::stepFunctionCall(FunctionCallNode* fNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k)
{	// Control
	string funid = fNode->getID();

	// standard functions such as print and rand
	if (standard.find(funid) != standard.end())
	{
		performStandard(fNode, funid, e, s);
		return State(c.next(), e, s, k);
	}
	FunctionNode* func = (FunctionNode*)getFunction(funid);
	Control oldControl = c;
	c = Control((ListifiedNode*)((FunctionBodyNode*)func->getBody())->getStatements());

	// Environment
	map<string, int> oldEnv = e;
	e = map<string, int>();
	ListifiedNode* formals = (ListifiedNode*)func->getArgs();
	ListifiedNode* actuals = (ListifiedNode*)fNode->getValues();
	for (int i = 0; i < formals->getLength(); i++)
	{
		int allocAddress = nextVar++;
		ValueNode* val = (ValueNode*)actuals->at(i);
		val = dereferenceID(val, oldEnv, s);
		s[allocAddress] = new StoreEntry(val);
		e[((ArgumentNode*)formals->at(i))->getID()] = allocAddress;
	}

	// Kontinuation
	Control returnControl = oldControl.next();
	map<string, int> returnEnvironment = current.getEnvironment();
	int returnK = current.getKontinuation();
	Continuation* kont = new Continuation(returnControl, returnEnvironment, returnK, 0);
	s[nextK] = new StoreEntry(kont);

	k = nextK--;

	return State(c, e, s, k);
}

State CESKInterpreter::stepAssignment(AssignmentNode* aNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k)
{
	if (dynamic_cast<FunctionCallNode*>(aNode->getTerm()))
	{
		FunctionCallNode* fNode = (FunctionCallNode*)aNode->getTerm();

		// Control
		string funid = fNode->getID();
		// standard functions such as print and rand
		if (standard.find(funid) != standard.end())
		{
			performStandard(fNode, funid, e, s, aNode->getID());
			return State(c.next(), e, s, k);
		}

		FunctionNode* func = (FunctionNode*)getFunction(funid);
		Control oldControl = c;
		c = Control((ListifiedNode*)((FunctionBodyNode*)func->getBody())->getStatements());

		// Environment
		map<string, int> oldEnv = e;
		e = map<string, int>();
		ListifiedNode* formals = (ListifiedNode*)func->getArgs();
		ListifiedNode* actuals = (ListifiedNode*)fNode->getValues();
		for (int i = 0; i < formals->getLength(); i++)
		{
			int allocAddress = nextVar++;
			ValueNode* val = (ValueNode*)actuals->at(i);
			val = dereferenceID(val, oldEnv, s);
			s[allocAddress] = new StoreEntry(val);

			e[((ArgumentNode*)formals->at(i))->getID()] = allocAddress;
		}

		// Kontinuation
		Control returnControl = oldControl.next();
		map<string, int> returnEnvironment = current.getEnvironment();
		int returnK = current.getKontinuation();

		Continuation* kont = new Continuation(returnControl, returnEnvironment, returnK, returnEnvironment[aNode->getID()]);
		s[nextK] = new StoreEntry(kont);

		k = nextK--;
	} 
	else if (dynamic_cast<BinopNode*>(aNode->getTerm()))
	{
		BinopNode* bNode = (BinopNode*)aNode->getTerm();
		ValueNode* vNode = performBinop(bNode, e, s);
		writeValue(aNode->getID(), vNode, e, s);
		c = c.next();
	}
	else if (dynamic_cast<UnopNode*>(aNode->getTerm()))
	{
		UnopNode* uNode = (UnopNode*)aNode->getTerm();
		ValueNode* vNode = performUnop(uNode, e, s);
		writeValue(aNode->getID(), vNode, e, s);
		c = c.next();
	}
	else if (dynamic_cast<ValueNode*>(aNode->getTerm()))
	{
		ValueNode* vNode = (ValueNode*)aNode->getTerm();
		vNode = dereferenceID(vNode, e, s);
		writeValue(aNode->getID(), vNode, e, s);
		c = c.next();
	}

	return State(c, e, s, k);
}

State CESKInterpreter::stepWhile(WhileNode* wNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k)
{
	ValueNode* cond = (ValueNode*)(wNode->getCondition());
	int condVal;
	cond = dereferenceID(cond, e, s);
	condVal = cond->getIntVal();

	if (condVal) c = c.nextWhile((ListifiedNode*)(wNode->getBody()));
	else c = c.next();

	return State(c, e, s, k);
}

State CESKInterpreter::stepIf(IfNode* iNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k)
{
	ValueNode* cond = (ValueNode*)(iNode->getCondition());
	int condVal;
	cond = dereferenceID(cond, e, s);
	condVal = cond->getIntVal();

	if (condVal)
	{
		c = c.nextIf((ListifiedNode*)(iNode->getBody()));
	}
	else
	{
		c = c.next();
	}

	return State(c, e, s, k);
}

State CESKInterpreter::stepElse(ElseNode* eNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k)
{
        c = c.nextElse((ListifiedNode*)(eNode->getBody()));
		return State(c, e, s, k);
}

State CESKInterpreter::stepReturn(ReturnNode* rNode, Control c, map<string, int> e, map<int, StoreEntry*> s, int k)
{
	// get next control from continuation
	if (!current.getKontinuation()) throw "HALT";
	StoreEntry* kontEntry = s[k];
	Continuation* oldKont = kontEntry->getKont();
	c = oldKont->getControl();
	k = oldKont->getAddress();
	int returnAddress = oldKont->getReturnAddress();
	if (returnAddress)
	{
		ValueNode* val = (ValueNode*)rNode->getValue();
		val = dereferenceID(val, e, s);
		s[returnAddress] = new StoreEntry(val);
	}
	e = oldKont->getEnvironment();

	return State(c, e, s, k);
}

State CESKInterpreter::step()
{
	//cout << current.getControl().toString() << endl;
    //State(Control cp, map<string, int> ep, int kp) : c(cp), e(ep), k(kp) {}
    Control c = current.getControl();
    ASTNode* statement = c.getStatement();

    Control newControl = c;
    map<string, int> newEnvironment = current.getEnvironment();
	map<int, StoreEntry*> newStore = current.getStore();
    int newKont = current.getKontinuation();
    // find the next control based on the current statement being executed
    if (dynamic_cast<DeclarationNode*>(statement))
    {
		return stepDeclaration((DeclarationNode*)statement, newControl, newEnvironment, newStore, newKont);
    }
    else if (dynamic_cast<FunctionCallNode*>(statement))
    {
		return stepFunctionCall((FunctionCallNode*)statement, newControl, newEnvironment, newStore, newKont);
    }
    else if (dynamic_cast<AssignmentNode*>(statement))
    {
		return stepAssignment((AssignmentNode*)statement, newControl, newEnvironment, newStore, newKont);
    }
    else if (dynamic_cast<WhileNode*>(statement))
    {
		return stepWhile((WhileNode*)statement, newControl, newEnvironment, newStore, newKont);
    }
    else if (dynamic_cast<IfNode*>(statement))
    {
		return stepIf((IfNode*)statement, newControl, newEnvironment, newStore, newKont);
    }
    else if (dynamic_cast<ElseNode*>(statement))
    {
		return stepElse((ElseNode*)statement, newControl, newEnvironment, newStore, newKont);
    }
    else if (dynamic_cast<ReturnNode*>(statement))
    {
		return stepReturn((ReturnNode*)statement, newControl, newEnvironment, newStore, newKont);
    }
    else throw "Unidentifiable statement: " + statement->toString();

    return State(newControl, newEnvironment, newKont);
}

ASTNode* CESKInterpreter::getFunction(string id)
{
    ListifiedNode* functions = (ListifiedNode*)(head->getFunctions());
    for (int i = 0; i < functions->getLength(); i++)
    {
        FunctionNode* f = (FunctionNode*)(functions->at(i));
        if (!f) continue;
        if (f->getID() == id)
        {
            return f;
        }
    }

    return NULL;
}

ValueNode* CESKInterpreter::dereferenceID(ValueNode* n, map<string, int> e, map<int, StoreEntry*> s)
{
    if (n->getTypeIndex() == IDV)
    {
        // fetch value from environment and store
        int storeaddr = 0;

        if (e.find(n->getStringVal()) != e.end())
        {
            storeaddr = e[n->getStringVal()];
        }
        else 
        {
            storeaddr = globalEnv[n->getStringVal()];
        }
        if (!storeaddr) { throw "Uninitialized Variable being referenced"; }
		n = s[storeaddr]->getVal();
    }
    return n;
}

void CESKInterpreter::writeValue(string id, ValueNode* val, map<string, int>& localEnv,  map<int, StoreEntry*>& s)
{
    int storeaddr;
    if (localEnv.find(id) != localEnv.end()) storeaddr = localEnv[id];
    else storeaddr = globalEnv[id];
	s[storeaddr] = new StoreEntry(val);
}

ValueNode* CESKInterpreter::performUnop(UnopNode* n, map<string, int> e, map<int, StoreEntry*> s)
{
    ValueNode* rhs = (ValueNode*)n->getValue();
    rhs = dereferenceID(rhs, e, s);
    operators op = n->getOp();
    if (op == Sub) return new ValueNode(rhs->getIntVal() * -1);
    else return new ValueNode(!(rhs->getIntVal()));
}

ValueNode* CESKInterpreter::performBinop(BinopNode* n, map<string, int> e, map<int, StoreEntry*> s)
{
    ValueNode* rhs = (ValueNode*)n->getRight();
    ValueNode* lhs = (ValueNode*)n->getLeft();
    rhs = dereferenceID(rhs, e, s);
    lhs = dereferenceID(lhs, e, s);
    operators op = n->getOp();
    if (op == Add)
    {
        vtypes lhtype = lhs->getTypeIndex();
        vtypes rhtype = rhs->getTypeIndex();
        if (lhtype == INTV && rhtype == INTV) return new ValueNode(lhs->getIntVal() + rhs->getIntVal());
        stringstream ss;
        switch(lhtype)
        {
            case INTV:
                ss << lhs->getIntVal();
                break;
            case CHARV:
                ss << lhs->getCharVal();
                break;
            case STRINGV:
                ss << lhs->getStringVal();
                break;
            default:
                throw "error in performBinop code 1";
        }
        switch(rhtype)
        {
            case INTV:
                ss << rhs->getIntVal();
                break;
            case CHARV:
                ss << rhs->getCharVal();
                break;
            case STRINGV:
                ss << rhs->getStringVal();
                break;
            default:
                throw "error in performBinop code 2";
        }
        string result = ss.str();
        return new ValueNode("string", const_cast<char*>(result.c_str()));
    }
    else
    {
        // Sub, Or, And, Lt, Gt, Lte, Gte, Eq, Neq, Mult
        switch(op)
        {
            case Sub:
                return new ValueNode(lhs->getIntVal() - rhs->getIntVal());
            case Or:
                return new ValueNode(lhs->getIntVal() || rhs->getIntVal());
            case And:
                return new ValueNode(lhs->getIntVal() && rhs->getIntVal());
            case Lt:
                return new ValueNode(lhs->getIntVal() < rhs->getIntVal());
            case Gt:
                return new ValueNode(lhs->getIntVal() > rhs->getIntVal());
            case Lte:
                return new ValueNode(lhs->getIntVal() <= rhs->getIntVal());
            case Gte:
                return new ValueNode(lhs->getIntVal() >= rhs->getIntVal());
            case Eq:
                return new ValueNode(lhs->getIntVal() == rhs->getIntVal());
            case Neq:
                return new ValueNode(lhs->getIntVal() != rhs->getIntVal());
            case Mult:
                return new ValueNode(lhs->getIntVal() * rhs->getIntVal());
            default:
                throw "Error in performBinop code 3";
        }
    }
}
