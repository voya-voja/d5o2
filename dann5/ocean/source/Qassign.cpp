#include <math.h> 
#include <Qassign.h>
#include <Qnary.h>
#include <Utility.h>
#include <Logger.h>

using namespace dann5::ocean;

/*** Quantum Assignment ***/

Qassignment::Qassignment(const Qdef::Sp& pAssignee, const Qexpression::Sp& pExpr)
	: mpAssignee(pAssignee), mpExpr(pExpr) 
{
	bind();
}

void Qassignment::assign(const Qdef::Sp& pAssignee, const Qexpression::Sp& pExpr)
{
	mpAssignee = pAssignee;
	expression(pExpr);
}

void Qassignment::expression(const Qexpression::Sp& pExpr)
{ 
	mpExpr = pExpr; 
	if (mpAssignee != nullptr && mpExpr != nullptr)
		bind();
}

void Qassignment::bind()
{
	size_t exprSize = mpExpr->noqbs(), asgnSize = mpAssignee->noqbs();
	// ensure result Q variable and equation Q expression have same number-of-bits
	if (exprSize > asgnSize)
	{
		Qnary::Sp pAssignee = dynamic_pointer_cast<Qnary>(mpAssignee);
		if (pAssignee != nullptr)
			// if Qnary result, size result to have same # of Q bits as expression
			pAssignee->resize(exprSize);
		else
			throw std::logic_error("The Q equation result and expression are of different type!");
	}
	else if(exprSize < asgnSize)
	{ // if expression has less Q bits, remaining Q bits or result will result 0 bitwise logic
		QnaryOp::Sp pNaryOp = dynamic_pointer_cast<QnaryOp>(as_const(*mpExpr).root());
		pNaryOp->resize(asgnSize);
	}

	mpExpr->root().output(mpAssignee);
}

Qubo Qassignment::qubo(bool finalized, size_t forBit) const
{
	if (mpExpr == nullptr)
		return Qubo();
	return(mpExpr->qubo(finalized, forBit));
}

string Qassignment::toString(bool decomposed, size_t forBit) const
{
	string sAssignment = "";
	if (!decomposed)
	{
		sAssignment = mpAssignee->toString(decomposed, forBit) + " = ";
	}
	sAssignment += mpExpr->toString(decomposed, forBit);
	return sAssignment;
}

void Qassignment::add(const Qsolver::Samples& samples) 
{ 
	mpExpr->add(samples);
}

string Qassignment::solutions() const
{
	return mpExpr->solutions();
}

string Qassignment::solve()
{
	return mpExpr->solve();
}

void Qassignment::reset()
{
	mpExpr->reset();
}

ostream& dann5::ocean::operator<<(std::ostream& out, const Qassignment& statement)
{
	out << statement.toString();
	return out;
}