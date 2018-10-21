#include "CESKInterpreter.h"
#include <sstream>

CESKInterpreter::CESKInterpreter(ASTNode* h)
{  
    // HALT continuation 
    store[0] = NULL;
    nextVar = 1;
    nextK = -1;

	fillStandard();

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

    current = State(c, map<string, int>(), 0);
}

void CESKInterpreter::fillStandard()
{
	standard.insert("print");
	standard.insert("printstring");
	standard.insert("printint");
	standard.insert("printchar");
}

State CESKInterpreter::step()
{
    //State(Control cp, map<string, int> ep, int kp) : c(cp), e(ep), k(kp) {}
    Control c = current.getControl();
    ASTNode* statement = c.getStatement();

    Control newControl;
    map<string, int> newEnvironment = current.getEnvironment();
    int newKont = current.getKontinuation();

    // find the next control based on the current statement being executed
    if (dynamic_cast<DeclarationNode*>(statement))
    {
        DeclarationNode* dNode = (DeclarationNode*)statement;
        
        newControl = c.next();

        int allocAddress = nextVar++;
        store[allocAddress];
        newEnvironment[dNode->getID()] = allocAddress;
    }
    else if (dynamic_cast<FunctionCallNode*>(statement))
    {
		cout << "##starting call" << endl;
        FunctionCallNode* fNode = (FunctionCallNode*)statement;
        
        // Control
        string funid = fNode->getID();
		// standard functions such as print and rand
		if (standard.find(funid) != standard.end())
		{
			cout << "##found a standard function" << endl;
			if (funid == "print" || funid == "printstring" || funid == "printint" || funid == "printchar")
			{
				ValueNode* arg = (ValueNode*)((ListifiedNode*)fNode->getValues())->at(0);
				arg = dereferenceID(arg);
				cout << arg->toString() << endl;
			}
			return State(c.next(), newEnvironment, newKont);
		}
		cout << "##not a standard function" << endl;
        FunctionNode* func = (FunctionNode*)getFunction(funid);
        newControl = Control((ListifiedNode*)func->getBody());

        // Environment
		cout << "##starting env" << endl;
        newEnvironment = map<string, int>();
        ListifiedNode* formals = (ListifiedNode*)func->getArgs();
        ListifiedNode* actuals = (ListifiedNode*)fNode->getValues();
        for (int i = 0; i < formals->getLength(); i++)
        {
            int allocAddress = nextVar++;
            ValueNode* val = (ValueNode*)actuals->at(i);
			val = dereferenceID(val);
            store[allocAddress] = new StoreEntry(val);
            newEnvironment[((ArgumentNode*)formals->at(i))->getID()] = allocAddress;
        }

        // Kontinuation
		cout << "##start kont" << endl;
        Control returnControl = c.next();
        map<string, int> returnEnvironment = current.getEnvironment();
        int returnK = current.getKontinuation();
        Continuation* kont = new Continuation(returnControl, returnEnvironment, returnK, 0);
        store[nextK] = new StoreEntry(kont);
        newKont = nextK--;
		cout << "##done with call" << endl;
    }
    else if (dynamic_cast<AssignmentNode*>(statement))
    {
        AssignmentNode* aNode = (AssignmentNode*)statement;
		if (dynamic_cast<FunctionCallNode*>(aNode->getTerm()))
		{
			FunctionCallNode* fNode = (FunctionCallNode*)aNode->getTerm();
        
			// Control
			string funid = fNode->getID();
			// standard functions such as print and rand
			if (standard.find(funid) != standard.end())
			{
				if (funid == "print" || funid == "printstring" || funid == "printint" || funid == "printchar")
				{
					ValueNode* arg = (ValueNode*)((ListifiedNode*)fNode->getValues())->at(0);
					arg = dereferenceID(arg);
					cout << arg->toString() << endl;

					writeValue(aNode->getID(), new ValueNode(1), newEnvironment);
				}
				return State(c.next(), newEnvironment, newKont);
			}
			FunctionNode* func = (FunctionNode*)getFunction(funid);
			newControl = Control((ListifiedNode*)func->getBody());

			// Environment
			newEnvironment = map<string, int>();
			ListifiedNode* formals = (ListifiedNode*)func->getArgs();
			ListifiedNode* actuals = (ListifiedNode*)fNode->getValues();
			for (int i = 0; i < formals->getLength(); i++)
			{
				int allocAddress = nextVar++;
				ValueNode* val = (ValueNode*)actuals->at(i);
				val = dereferenceID(val);
				store[allocAddress] = new StoreEntry(val);
				newEnvironment[((ArgumentNode*)formals->at(i))->getID()] = allocAddress;
			}

			// Kontinuation
			Control returnControl = c.next();
			map<string, int> returnEnvironment = current.getEnvironment();
			int returnK = current.getKontinuation();
			
			Continuation* kont = new Continuation(returnControl, returnEnvironment, returnK, returnEnvironment[aNode->getID()]);
			store[nextK] = new StoreEntry(kont);
			newKont = nextK--;
		} 
		else if (dynamic_cast<BinopNode*>(aNode->getTerm()))
		{
			BinopNode* bNode = (BinopNode*)aNode->getTerm();
			ValueNode* vNode = performBinop(bNode);
			writeValue(aNode->getID(), vNode, current.getEnvironment());
			/*
			int storeaddr;
			if (current.getEnvironment().find(aNode->getID()) != current.getEnvironment().end())
			{
				storeaddr = newEnvironment[aNode->getID()];
			}
			else
			{
				// we can assume that the variable being written to is defined, as the type checking handles that already
				storeaddr = globalEnv[aNode->getID()];
			}
			store[storeaddr] = new StoreEntry(vNode);
			*/
			newControl = c.next();
		}
		else if (dynamic_cast<UnopNode*>(aNode->getTerm()))
		{
			UnopNode* uNode = (UnopNode*)aNode->getTerm();
			ValueNode* vNode = performUnop(uNode);
			writeValue(aNode->getID(), vNode, current.getEnvironment());
			/*
			int storeaddr;
			if (current.getEnvironment().find(aNode->getID()) != current.getEnvironment().end())
			{
				storeaddr = newEnvironment[aNode->getID()];
			}
			else
			{
				storeaddr = globalEnv[aNode->getID()];
			}
			store[storeaddr] = new StoreEntry(vNode);
			*/
			newControl = c.next();
		}
		else if (dynamic_cast<ValueNode*>(aNode->getTerm()))
		{
			ValueNode* vNode = (ValueNode*)aNode->getTerm();
			vNode = dereferenceID(vNode);
			writeValue(aNode->getID(), vNode, current.getEnvironment());
			/*
			int storeaddr;
			if (current.getEnvironment().find(aNode->getID()) != current.getEnvironment().end())
			{
				storeaddr = newEnvironment[aNode->getID()];
			}
			else
			{
				// we can assume that the variable being written to is defined, as the type checking handles that already
				storeaddr = globalEnv[aNode->getID()];
			}
			store[storeaddr] = new StoreEntry(vNode);
			*/
			newControl = c.next();
		}
    }
    else if (dynamic_cast<WhileNode*>(statement))
    {
        WhileNode* wNode = (WhileNode*)statement;
        // check condition
        ValueNode* cond = (ValueNode*)(wNode->getCondition());
        int condVal;
        cond = dereferenceID(cond);
        // we can now assume that cond is a ValueNode (either straight from the AST or through the store)
        condVal = cond->getIntVal();

        if (condVal)
        {
            newControl = c.nextWhile((ListifiedNode*)(wNode->getBody()));
        }
        else
        {
            newControl = c.next();
        }
    }
    else if (dynamic_cast<IfNode*>(statement))
    {
        IfNode* iNode = (IfNode*)statement;
        // check condition
        ValueNode* cond = (ValueNode*)(iNode->getCondition());
        int condVal;
        cond = dereferenceID(cond);
        // we can now assume that cond is a ValueNode (either straight from the AST or through the store)
        condVal = cond->getIntVal();

        if (condVal)
        {
            newControl = c.nextIf((ListifiedNode*)(iNode->getBody()));
        }
        else
        {
            newControl = c.next();
        }
    }
    else if (dynamic_cast<ElseNode*>(statement))
    {
        ElseNode* eNode = (ElseNode*)statement;
        newControl = c.nextElse((ListifiedNode*)(eNode->getBody()));
    }
    else if (dynamic_cast<ReturnNode*>(statement))
    {
        ReturnNode* rNode = (ReturnNode*)statement;
        // get next control from continuation
		if (!current.getKontinuation()) throw "HALT";
        StoreEntry* kontEntry = store[current.getKontinuation()];
        Continuation* oldKont = kontEntry->getKont();
        newControl = oldKont->getControl();
		newEnvironment = oldKont->getEnvironment();
		newKont = oldKont->getAddress();
		int returnAddress = oldKont->getReturnAddress();
		if (returnAddress)
		{
			ValueNode* val = (ValueNode*)rNode->getValue();
			val = dereferenceID(val);
			store[returnAddress] = new StoreEntry(val);
		}
    }
	else throw "No idea what this is";

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

ValueNode* CESKInterpreter::dereferenceID(ValueNode* n)
{
	if (n->getTypeIndex() == IDV)
	{
		// fetch value from environment and store
		int storeaddr;

		if (current.getEnvironment().find(n->getStringVal()) != current.getEnvironment().end())
		{
			storeaddr = current.getEnvironment()[n->getStringVal()];
		}
		else 
		{
			storeaddr = globalEnv[n->getStringVal()];
		}
		if (!storeaddr) { throw "Uninitialized Variable being referenced"; }
		n = store[storeaddr]->getVal();
	}
	return n;
}

void CESKInterpreter::writeValue(string id, ValueNode* val, map<string, int> localEnv)
{
	int storeaddr;
	if (localEnv.find(id) != localEnv.end()) storeaddr = localEnv[id];
	else storeaddr = globalEnv[id];
	store[storeaddr] = new StoreEntry(val);
}

ValueNode* CESKInterpreter::performUnop(UnopNode* n)
{
	ValueNode* rhs = (ValueNode*)n->getValue();
	rhs = dereferenceID(rhs);
	operators op = n->getOp();
	if (op == Sub) return new ValueNode(rhs->getIntVal() * -1);
	else return new ValueNode(!(rhs->getIntVal()));
}

ValueNode* CESKInterpreter::performBinop(BinopNode* n)
{
	ValueNode* rhs = (ValueNode*)n->getRight();
	ValueNode* lhs = (ValueNode*)n->getLeft();
	rhs = dereferenceID(rhs);
	lhs = dereferenceID(lhs);
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