#include "TypeCheckVisitor.h"
#include "ASTNodeDefines.h"

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
        UnopNode* unop = (UnopNode*)n;
        if (getType((ValueNode*)(unop->getValue())) == INTV)
        {
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
        FunctionCallNode* func = (FunctionCallNode*)n;
        vtypes vtype = getTypeById(func->getID());
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
        if (lv == INTV && rv == INTV) return INTV;
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
}

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
    }
}

