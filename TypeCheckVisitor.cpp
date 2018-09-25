#include "TypeCheckVisitor.h"
#include "ASTNodeDefines.h"

vtypes TypeCheckVisitor::getType(ASTNode* n)
{
    if (dynamic_cast<BinopNode*>(n))
    {
        BinopNode* binop = (BinopNode*)n;
        ValueNode* l = (ValueNode*)(n->getLeft());
        ValueNode* r = (ValueNode*)(n->getRight());
        operators o = n->getOp();
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
    else if (dynamic_cast<FunctionNode*>(n))
    {
        return INTV;
    }
    throw "TypeCheckVisitor expected a valid typed node";
}

vtypes TypeCheckVisitor::binopRetrun(ValueNode* l, ValueNode* r, operators o)
{
    //dummy
    return INTV;
}

vtypes TypeCheckVisitor::getTypeById(string id)
{
    if (table.at(0).find(id) != table.at(0).end())
    {
        cout << "found id: " << id << " in global table" << endl;
        ASTNode* n = table.at(0)[id];
        if (dynamic_cast<DeclarationNode*>(n)){
            DeclarationNode* dec = (DeclarationNode*)(n);
            return stringToVtypes(dec->getType());
        }
        else return stringToVtypes(((FunctionNode*)n)->getType(), true);
    }
    else if (table.at(current).find(id) != table.at(current).end())
    {
        cout << "found id: " << id << " in table " << current << endl;
        ASTNode* n = table.at(current)[id];
        if (dynamic_cast<DeclarationNode*>(n)){
            DeclarationNode* dec = (DeclarationNode*)(n);
            return stringToVtypes(dec->getType());
        }
    }
    throw "Undefined id: " + id;
}

void TypeCheckVisitor::visit(AssignmentNode* n)
{
    vtypes rhs = getType(n->getTerm());
}


