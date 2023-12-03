//
//  Qfunc.cpp
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-12-01.
//

#include <Qfunc.h>

using namespace dann5;

/*** Qatomicity ***/

string Qatomicity::toString(bool decomposed, size_t forBit) const
{
    if(!decomposed) return QcellOp::toString(decomposed);
    string opStr = "{ ";
    for(auto pOp : mOps)
        opStr += pOp->toString(decomposed) + "; ";
    opStr += "}";
    return(opStr);
}

void Qatomicity::compile(Qcompiler& compiler) const
{
    for(auto pOp : mOps)
        pOp->compile(compiler);
}

Qvalue Qatomicity::calculate(const Qvalues& values) const
{
    Qvalue value = cSuperposition;
    Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
    string outId = pOut->id();
    for(auto pOp : mOps)
    {
        Qcell::Sp pOpOut = dynamic_pointer_cast<Qcell>(pOp->Qop::output());
        if(pOpOut->id() == outId)
            value = pOp->value();
    }
    return value;
}

/*** Qfunction ***/

Qfunction::Qfunction(const string& name, const Qblock& block,
                                                size_t noArguments)
: QnaryOp(name, noArguments)
{
    Compiler compiler(this);
    block.compile(compiler);
}

Qfunction::Qfunction(const string& name, const Qstatements& statements,
                                                size_t noArguments)
: QnaryOp(name, noArguments)
{
    for(auto pStatement : statements)
        (*this) << (*pStatement);
}

string Qfunction::declaration() const
{
    Qdef::Sp pOut = Qop::output();
    string dclrtn = pOut->toString() + " " + id() + "(";
    
    Qdefs ins(Qop::inputs());
    size_t size = ins.size();
    if(size > 1) dclrtn += " ";
    for(size_t at = 0; at < size; at++)
    {
        dclrtn += ins[at]->toString();
        if(at != size - 1) dclrtn += ", ";
    }
    dclrtn += ")";
    return dclrtn;
}

void Qfunction::resize(size_t size, Qvalue value)
{
}

Qfunction& Qfunction::operator<<(const Qstatement& right)
{
    Compiler compiler(this);
    right.compile(compiler);
    return(*this);
}

Qfunction Qfunction::operator<<(const Qstatement& right) const
{
    Qfunction aCopy(*this);
    aCopy << right;
    return aCopy;
}

Qfunction::CommaOp Qfunction::operator=(const Qstatement& statement)
{
    (*this) << statement;
    return(CommaOp(this));
}

void Qfunction::refreshOnInputs()
{
    Qdefs ins(Qop::inputs());
    for(auto pIn: ins)
        mBinder << *pIn;
}

void Qfunction::refreshOnOutput()
{
    mBinder << *Qop::output();
}

ostream& dann5::operator << (std::ostream& out, const Qfunction& r)
{
    out << r.toString();
    return out;
}

/*** Qfunction::AtomicityCompiler ***/

void Qfunction::Compiler::compile(const Qop& op)
{
    size_t opNoQbs = op.noqbs();
    Qdefs ins = mpFunction->Qop::inputs();
    Qdef::Sp pOut = mpFunction->Qop::output();
    for(size_t at = 0; at < opNoQbs; at++)
    {
        QcellOp::Sp pOp = nullptr;
        try{
            const QnaryOp& naryOp = dynamic_cast<const QnaryOp&>(op);
            pOp = static_pointer_cast<QcellOp>(naryOp[at]->clone());
        }
        catch(...)
        {
            const QcellOp& cellOp = static_cast<const QcellOp&>(op);
            pOp = static_pointer_cast<QcellOp>(cellOp.clone());
            if(at != 0)
                throw logic_error("Error @Qfunction::Compiler: QcellOp has "
                                  + to_string(opNoQbs) + " qbits.");
        }
        add(pOp, at);
    }
}

void Qfunction::Compiler::add(QcellOp::Sp pOp, size_t at)
{
    size_t noQbs = mpFunction->noqbs();
    Qdefs ins = mpFunction->Qop::inputs();
    Qdef::Sp pOut = mpFunction->Qop::output();
    Qatomicity::Sp pAtomicity = nullptr;
    Qcells& logic = mpFunction->cells();
    if(noQbs > at)
        pAtomicity = static_pointer_cast<Qatomicity>(logic[at]);
    else
    {
        string name = mpFunction->id() + to_string(at);
        pAtomicity = Qatomicity::Sp(new Qatomicity(name, mpFunction->noInputs()));
        logic.push_back(pAtomicity);
        Qdefs atomicIns;
        for(auto pIn : ins)
        {
            Qnary::Sp pNaryIn = dynamic_pointer_cast<Qnary>(pIn);
            if(pNaryIn == nullptr && at == 0)
                atomicIns.push_back(pIn);
            else
            {
                size_t inNoQbs = pNaryIn->noqbs();
                if(at == inNoQbs)
                    pNaryIn->resize(inNoQbs + 1);
                atomicIns.push_back(as_const(*pNaryIn)[at]);
            }
        }
        pAtomicity->noInputs(atomicIns.size());
        Qnary::Sp pNaryOut = dynamic_pointer_cast<Qnary>(pOut);
        Qdef::Sp pAtomicOut = nullptr;
        if(pNaryOut == nullptr && at == 0)
            pAtomicOut = pOut;
        else
        {
            size_t outNoQbs = pNaryOut->noqbs();
            if(at == outNoQbs)
                pNaryOut->resize(outNoQbs + 1);
            pAtomicOut = as_const(*pNaryOut)[at];
        }
        pAtomicity->operands(pAtomicOut, atomicIns);
    }
    pAtomicity->add(pOp);
}

