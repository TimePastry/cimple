#include "CESKInterpreter.h"

void CESKInterpreter::init(ASTNode* h)
{  
    // HALT continuation 
    store[0] = NULL;
    nextVar = 1;
    nextK = -1;

    head = (ProgramNode*)h;
    ListifiedNode* globals = (ListifiedNode*)(head->getGlobals());
    for (int i = 0; i < globals->getLength(); i++)
    {
        DeclarationNode* g = (DeclarationNode*)globals->at(i);
        // touch every global variable declaration
        // leave them as default (0) which can be deteced as "uninitialized" because it would conflict with the HALT kont
        globalEnv[g->getID()];
    }

    ListifiedNode* functions = (ListifiedNode*)(head->getFunctions());
    FunctionNode* mainNode;
    for (int i = 0; i < functions->getLength(); i++)
    {
        FunctionNode* f = (FunctionNode*)functions->at(i);
        if (f->getID() == "main") mainNode = f;
    }

    current = State((ListifiedNode*)mainNode->getBody(), map<string, int>(), 0);
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
        newControl = c.next();
    }
    else if (dynamic_cast<FunctionCallNode*>(statement))
    {
        string funid = ((FunctionCallNode*)statement)->getID();
        newControl = Control((ListifiedNode*)(getFunctionBody(funid)));
    }
    else if (dynamic_cast<AssignmentNode*>(statement))
    {
        newControl = c.next();
    }
    else if (dynamic_cast<WhileNode*>(statement))
    {
        WhileNode* wNode = (WhileNode*)statement;
        // check condition
        ValueNode* cond = (ValueNode*)(wNode->getCondition());
        int condVal;
        if (cond->getTypeIndex() == IDV)
        {
            // fetch value from environment and store
            int storeaddr;

            if (newEnvironment.find(cond->getStringVal()) != newEnvironment.end())
            {
                storeaddr = newEnvironment[cond->getStringVal()];
            }
            else 
            {
                storeaddr = globalEnv[cond->getStringVal()];
            }
            if (!storeaddr) { throw "Uninitialized Variable in While Loop Condition"; }
            cond = store[storeaddr]->getVal();
        }
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
        newControl = c.nextIf((ListifiedNode*)(iNode->getBody()));
    }
    else if (dynamic_cast<ElseNode*>(statement))
    {
        ElseNode* eNode = (ElseNode*)statement;
        // check condition
        newControl = c.nextElse((ListifiedNode*)(eNode->getBody()));
    }
    else if (dynamic_cast<ReturnNode*>(statement))
    {
        // get next control from continuation
    }
    
    return State(newControl, newEnvironment, newKont);
}

ASTNode* CESKInterpreter::getFunctionBody(string id)
{
    ListifiedNode* functions = (ListifiedNode*)(head->getFunctions());
    for (int i = 0; i < functions->getLength(); i++)
    {
        FunctionNode* f = (FunctionNode*)(functions->at(i));
        if (f->getID() == id)
        {
            return f->getBody();
        }
    }

    return NULL;
}

