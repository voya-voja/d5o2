#include <QcellOps.h>
#include <Qbit.h>

using namespace dann5::ocean;

/*** Q cell operation ***/

QcellOp::QcellOp(const string& id, size_t size) 
	: Qcell(id), Qop(size)
{}

void QcellOp::inputs(const Qdefs& ins)
{
	Qop::inputs(ins);
	Qvalue v = value();
	if (v != gSuperposition)
	{
		Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
		Qvalue outV =pOut->value();
		if (outV == gSuperposition && v != outV && pOut->id()[0] == '_')
			pOut->value(v);
	}
}

void QcellOp::output(const Qdef::Sp& pOut, size_t forBit)
{
	Qvalue v = value();
	if (v != gSuperposition)
	{
		Qvalue outV = dynamic_pointer_cast<Qcell>(pOut)->value();
		if (outV != gSuperposition && v != outV)
			throw logic_error("Error: output '" + pOut->toString()
				+ "' is out of sync with operation '" + Qop::toString() + "'!");
	}
	Qop::output(pOut, forBit);
}

void QcellOp::value(Qvalue v)
{
	throw std::logic_error("Setting value of a Qcell operation!");
}

Qvalue QcellOp::value() const
{
	Qdefs ins = Qop::inputs();
	size_t iSize = ins.size();
	Qvalues values;
	for (int at = 0; at < iSize; at++)
	{
		Qcell::Sp pIn = dynamic_pointer_cast<Qcell>(ins[at]);
		if (pIn != nullptr)
		{
			Qvalue v = pIn->value();
			if (v == gSuperposition)
				return(gSuperposition);
			values.push_back(v);
		}
	}
	if(values.size() > 0)
		return(calculate(values));
	return(gSuperposition);
}

Qvalue QcellOp::value()
{
	return(as_const(*this).value());
}

void QcellOp::solutions(const Qsolver::Samples& samples)
{
	Qop::solutions(samples);
}

string QcellOp::solution(size_t sampleId) const
{
	return Qop::solution(sampleId);
}

/*** Q operator ***/

string Qoperator::toString(bool decomposed, size_t forBit) const
{
	string str("?"), rest("");
	Qdef::Sp pIn = Qop::inputs()[0];
	if (pIn != nullptr)
	{
		str = pIn->toString(decomposed, forBit);
		if (decomposed)
		{
			// if operand is a sub-operation
			Qop::Sp pOp = dynamic_pointer_cast<Qop>(pIn);
			if (pOp != nullptr)
			{
				rest += str;
				str = pOp->output(forBit)->toString(decomposed, forBit); // extract sub-operation output
			}
		}
	}
	string rStr = str;
	rStr += " " + identifier() + " ";
	Qdef::Sp pOut = Qop::output();
	if (pOut != nullptr)
	{
		str = pOut->toString(decomposed, forBit);
		if (decomposed)
		{
			// if operand is a sub-operation
			Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOut);
			if (pOp != nullptr)
			{
				rest += str;
				str = pOp->output(forBit)->toString(decomposed, forBit); // extract sub-operation output
			}
		}
		rStr += str;
	}
	if (decomposed)
		rStr = "\n" + rStr + rest;
	return rStr;
}

/*** Q eq operation ***/

Qvalue Qeq::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] == dynamic_pointer_cast<Qcell>(Qop::output())->value()));
}

/*** Q ne operation ***/

Qvalue Qneq::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] != dynamic_pointer_cast<Qcell>(Qop::output())->value()));
}

/*** Q lt operation ***/

Qvalue Qlt::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] < values[1]));
}

/*** Q le operation ***/

Qvalue Qle::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] <= dynamic_pointer_cast<Qcell>(Qop::output())->value()));
}

/*** Q gt operation ***/

Qvalue Qgt::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] > dynamic_pointer_cast<Qcell>(Qop::output())->value()));
}

/*** Q ge operation ***/

Qvalue Qge::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] >= dynamic_pointer_cast<Qcell>(Qop::output())->value()));
}

/*** Q and operation ***/

Qvalue Qand::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] & values[1]));
}

/*** Q cell 2 outputs operation ***/

Qcell2OutOp::Qcell2OutOp(const string& id, size_t size) 
	: QcellOp(id, size), mpAuxiliary(nullptr)
{}

void Qcell2OutOp::output(const Qdef::Sp& pOut, size_t forBit)
{
	QcellOp::output(pOut, forBit);
	// if aux output is not initialized, initialize it with same Q type as output
	if (mpAuxiliary == nullptr)
	{
		mpAuxiliary = dynamic_pointer_cast<QcellOp>(Qop::output()->clone());
		mpAuxiliary->id("?" + id() + Qop::Id("?" + id()));
	}
}

Qdefs Qcell2OutOp::outputs() const
{
	Qdefs outs = QcellOp::outputs();
	outs.push_back(mpAuxiliary);
	return outs;
}

/*** Q nAnd operation ***/

Qvalue QnAnd::calculate(const Qvalues& values) const
{
	return(Qvalue(~(values[0] & values[1])));
}

/*** Q or operation ***/

Qvalue Qor::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] | values[1]));
}

/*** Q nOr operation ***/

Qvalue QnOr::calculate(const Qvalues& values) const
{
	return(Qvalue(~(values[0] | values[1])));
}


/*** Q addition operation ***/

Qaddition::Qaddition(const string& id, size_t size, const Qdef::Sp pOut, const Qaddition::Carry::Sp pCarry)
	: QcellOp(id, size), mpCarry(pCarry)
{
	mpCarry->addition(this);
	output(pOut);
	pOut->id(outId());
}


void Qaddition::inputs(const Qdefs& ins)
{
	QcellOp::inputs(ins);
	mpCarry->inputs(ins);
}

void Qaddition::output(const Qdef::Sp& pOut, size_t forBit)
{
	QcellOp::output(pOut, forBit);
	// if carry output is not initialized, initialize it with same Q type as output
	if(mpCarry->Qop::output() == nullptr)
	{
		Qdef::Sp pCarryOut = Qop::output()->clone();
		pCarryOut->id(mpCarry->id() + Qop::Id(mpCarry->id()));
		mpCarry->output(pCarryOut);
	}
}

Qdefs Qaddition::outputs() const
{
	Qdefs outs = QcellOp::outputs();
	outs.push_back(mpCarry->Qop::output());
	return outs;
}


/*** Carry Operation ***/

Qaddition::Carry::Carry(Qaddition* pAddition)
	: QcellOp(AdderQT::Carry::cMark, pAddition->noInputs()), mpAddition(pAddition)
{}

Qaddition::Carry::Carry(const Carry& right)
	: QcellOp(right), mpAddition(right.mpAddition)
{}

Qaddition::Carry::~Carry()
{}

Qvalue Qaddition::Carry::calculate(const Qvalues& values) const
{
	const Qdefs& ins = as_const(*mpAddition).Qop::inputs();
	size_t iSize = ins.size();
	Qvalue carry = dynamic_pointer_cast<Qcell>(ins[0])->value() & dynamic_pointer_cast<Qcell>(ins[1])->value();
	if (iSize == 3)
		carry &= dynamic_pointer_cast<Qcell>(ins[2])->value();
	return carry;
}

string Qaddition::Carry::toString(bool decomposed, size_t forBit) const
{
	if (decomposed)
	{
		Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(mpAddition->Qop::output(forBit));	
		string cStr = "";
		Qvalue v = pOut->value();
		if (v != gSuperposition) cStr = to_string(v);
		else cStr.append(1, gSuperposition);
		cStr = "\n" + Qop::output()->toString(decomposed, forBit) + " = "
				+ AdderQT::Carry::Symbol(pOut->id() + "/" + cStr + "/");
		return cStr;
	}
	return AdderQT::Carry::Symbol(mpAddition->Qop::toString(decomposed, forBit));
}

void Qaddition::Carry::addition(Qaddition* pAddition)
{
	mpAddition = pAddition;
}

/*** Q xor operation ***/

Qvalue Qxor::calculate(const Qvalues& values) const
{
	// returns one Qbit value without carry bit
	return(Qvalue((values[0] ^ values[1]) & 1));
}

/*** Q adder operation ***/

Qvalue Qadder::calculate(const Qvalues& values) const
{
	// returns one Qbit value without carry bit
	return(Qvalue(values[0] + values[1] + values[2]) & 1);
}

/*** Q xor-adder operation ***/

QxorAdder::Sp QxorAdder::process(const Qdefs& ins)
{
	if (ins.size() == 3)
	{
		noInputs(3);
		Qaddition::inputs(ins);
	}
	else if (ins.size() == 2)
	{
		QxorAdder::Sp pLxor(dynamic_pointer_cast<QxorAdder>(ins[0])),
			pRxor(dynamic_pointer_cast<QxorAdder>(ins[1]));
		if (pLxor == nullptr && pRxor == nullptr)
		{
			Qaddition::inputs(ins);
		}
		else
		{
			QxorAdder::Sp pAdder;
			size_t at = 1;
			if (pLxor != nullptr) 
				pAdder = pLxor;
			else
			{
				pAdder = pRxor;
				at = 0;
			}
			if(pAdder->isAdder())
			{
				Qaddition::inputs(ins);
			}
			else
			{
				pAdder->id(AdderQT::cMark);
				pAdder->noInputs(3);
				pAdder->append(ins[at]);
				pAdder->extended();
				return pAdder;
			}
		}
	}
	else
		throw invalid_argument("Arguments number is " + to_string(ins.size())
			+ " instead of 2 or 3, in case of xor or adder, respectfully ");
	return(QxorAdder::Sp(new QxorAdder(*this)));
}

Qvalue QxorAdder::calculate(const Qvalues& values) const
{
	size_t size = values.size();
	// returns one Qbit value without carry bit
	if(size == 2)
		return(Qvalue((values[0] ^ values[1]) & 1));

	return(Qvalue(values[0] + values[1] + values[2]) & 1);
}
