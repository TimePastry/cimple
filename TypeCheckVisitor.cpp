#include "TypeCheckVisitor.h"
#include "ASTNodeDefines.h"

// this funciton only finds the type on the right hand side of an assignment
// it is kind of a misnomer
vtypes TypeCheckVisitor::getType(ASTNode* n)
{
    if (dynamic_cast<BinopNode*>(n))
    {
        BinopNode* binop = (BinopNode*)n;
        ValueNode* l = (ValueNode*)(binop->getLeft());
        ValueNode* r = (ValueNode*)(binop->getRight());
        operators o = binop->getOp();
        return binopReturn(l, r, o);
    }
    else if (dynamic_cast<UnopNode*>(n))
    {
        // both unops are only defined for ints
        // -i and !i
        UnopNode* unop = (UnopNode*)n;
        if (getType((ValueNode*)(unop->getValue())) == INTV)
        {
            if (verbose)
            {
                cout << unop->getValue()->toString() << " = int, ";
            }
            return INTV;
        }
        else throw "Expected an INTV in a unary operator";
    }
    else if (dynamic_cast<ValueNode*>(n))
    {
        ValueNode* val = (ValueNode*)n;
        vtypes vtype = val->getTypeIndex();
        if (vtype == IDV)
        {
            vtype = getTypeById(val->getValue());    
        }
        return vtype;
    }
    else if (dynamic_cast<FunctionCallNode*>(n))
    {
        // temporarily assuming that the function's arguments are valid, and that the
        // function will return the type as given in its definition
        // this is because the visitor pattern is going to visit the function next to check that,
        // and to check it here would be redundant
        FunctionCallNode* func = (FunctionCallNode*)n;
        vtypes vtype = getTypeById(func->getID());

        // there are seperate types for functions and values, so here we convert from
        // function type to value type
        switch(vtype)
        {
        case INTF:
            vtype = INTV;
            break;
        case CHARF:
            vtype = CHARV;
            break;
        case STRINGF:
            vtype = STRINGV;
            break;
        }
        return vtype;
    }
    throw "TypeCheckVisitor expected a valid typed node";
}

vtypes TypeCheckVisitor::binopReturn(ValueNode* l, ValueNode* r, operators o)
{
    vtypes lv = l->getTypeIndex();
    vtypes rv = r->getTypeIndex();
    if (lv == IDV) lv = getTypeById(l->getValue());
    if (rv == IDV) rv = getTypeById(r->getValue());
    
    switch(o)
    {
    case Add:
        // two ints sum to another int
        // adding two other things will create a string by concatenating them together in a string
        // i.e. 'a' + 4 == "a4"
        if (lv == INTV && rv == INTV)
        {
            if (verbose)
            {
                cout << l->getValue() << " = int & " << r->getValue() << " = int, ";
            }
            return INTV;
        }
        if (verbose)
        {
            cout << l->getValue() << " = " << vtypesToString(lv) << " & " << r->getValue() << " = " << vtypesToString(rv) << ", ";
        }
        return STRINGV;
    case Sub:
    case And:
    case Mult:
    case Or:
    case Lt:
    case Lte:
    case Gt:
    case Gte:
    case Eq:
    case Neq:
        // all other binops are only defined for ints
        // I could make subtraction work for strings, but I decided that was too complex for little gain
        if (lv == INTV && rv == INTV) return INTV;
        throw "Expected two integer values for operator: " + opToString(o);
    default:
        throw "I forgot to implement one of the binops";
    }
}

vtypes TypeCheckVisitor::getTypeById(string id)
{
    if (table.at(current).find(id) != table.at(current).end())
    {
        ASTNode* n = table.at(current)[id];
        if (dynamic_cast<DeclarationNode*>(n)){
            DeclarationNode* dec = (DeclarationNode*)(n);
            return stringToVtypes(dec->getType(), false);
        }
        if (dynamic_cast<ArgumentNode*>(n)){
            ArgumentNode* arg = (ArgumentNode*)n;
            return stringToVtypes(arg->getType(), false);
        }
    }
    else if (table.at(0).find(id) != table.at(0).end())
    {
        ASTNode* n = table.at(0)[id];
        if (dynamic_cast<DeclarationNode*>(n)){
            DeclarationNode* dec = (DeclarationNode*)(n);
            return stringToVtypes(dec->getType(), false);
        }
        else return stringToVtypes(((FunctionNode*)n)->getType(), true);
    }
    
    throw "Undefined id: " + id;
}

ASTNode* TypeCheckVisitor::getNodeById(string id)
{
    if (table.at(current).find(id) != table.at(current).end())
    {
        return table.at(current)[id];
    }
    else if (table.at(0).find(id) != table.at(0).end())
    {
        return table.at(0)[id];
    }
    throw "Undefined id: " + id;
}

void TypeCheckVisitor::visit(AssignmentNode* n)
{
    vtypes rhs = getType(n->getTerm());
    vtypes lhs = getTypeById(n->getID());
    if (rhs != lhs)
    {
        throw "attempted to assign " + vtypesToString(rhs) + " to a " + vtypesToString(lhs);     
    }
    if (verbose)
    {
        cout << "Type(" << n->getTerm()->toString() << ") = Type(" << n->getID() << ")" << endl;
        cout << "_____________________________________________________" << endl;
        cout << '\t' << n->toString() << " is type safe" << endl << endl;
    }
}

// this is where the arguments for a function call are compared with the function signature
void TypeCheckVisitor::visit(FunctionCallNode* n)
{
    FunctionNode* func = (FunctionNode*)getNodeById(n->getID());
    ListifiedNode* args = (ListifiedNode*)n->getValues();
    int numArgs = listLength(args);
    int expectedArgs = listLength(func->getArgs());
    if (numArgs > expectedArgs) throw "too many arguments for function call: " + n->getID();
    if (numArgs < expectedArgs) throw "not enough arguments for function call: " + n->getID();
    for (int i = 0; i < numArgs; i++)
    {
        ArgumentNode* expected = (ArgumentNode*)func->getArg(i); 
        vtypes expectedType = stringToVtypes(expected->getType());
        ValueListNode* actual = (ValueListNode*)n->getVal(i);
        vtypes actualType = getType(actual);
        if (expectedType != actualType) throw "type mismatch in function call to: " + n->getID();
        if (verbose)
        {
            cout << "Type(" << actual->toString() << ") = " << expected->getType();
            if (i + 1 != numArgs) cout << ", ";
        }
    }
    if (verbose)
    {
        cout << endl << "_____________________________________________________" << endl;
        cout << '\t' << n->toString() << " is type safe" << endl << endl;
    }
}

// conditionals must be ints, 0 == false else true
void TypeCheckVisitor::visit(WhileNode* n)
{
    ValueNode* v = (ValueNode*)n->getCondition();
    vtypes type = getType(v);
    if (type != INTV) throw "value of condition for while loop must be int";
    if (verbose)
    {
        cout << '\t' << v->toString() << " = int" << endl;
        cout << "_____________________________________________________" << endl;
        cout << '\t' << "Conditional is type safe" << endl << endl;
    }
}

void TypeCheckVisitor::visit(IfNode* n)
{
    ValueNode* v = (ValueNode*)n->getCondition();
    vtypes type = getType(v);
    if (type != INTV) throw "value of condition for if block must be int";
    if (verbose)
    {
        cout << '\t' << v->toString() << " = int" << endl;
        cout << "_____________________________________________________" << endl;
        cout << '\t' << "Conditional is type safe" << endl << endl;
    }
}

// returns are only valid if they return the type for the current function
void TypeCheckVisitor::visit(ReturnNode* n)
{
    ValueNode* v = (ValueNode*)n->getValue();
    vtypes type = getType(v);
    if (type != returnType) throw "return type does not match that of function definition";
    if (verbose)
    {
        cout << '\t' << v->toString() << " = " << vtypesToString(type) << endl;
        cout << "_____________________________________________________" << endl;
        cout << '\t' << "Return is type safe" << endl << endl;
    }
}
