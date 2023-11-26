#include <Qbit.h>
#include <QcellOps.h>
#include <Factory.h>
#include <Qbool.h>

using namespace dann5;

Qassign<Qbit> Qbit::operator=(const Qbit& right)
{
	Qexpr<Qbit> expr = right == right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator=(const Qexpr<Qbit>& right)
{
	Qassign<Qbit> assign(*this, right);
	return assign;
}

Qassign<Qbit> Qbit::operator&=(const Qbit& right)
{
	Qexpr<Qbit> expr = *this & right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator&=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *this & right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator|=(const Qbit& right)
{
	Qexpr<Qbit> expr = *(this) | right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator|=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *(this) | right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator^=(const Qbit& right)
{
	Qexpr<Qbit> expr = *this ^ right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qassign<Qbit> Qbit::operator^=(const Qexpr<Qbit>& right)
{
	Qexpr<Qbit> expr = *this ^ right;
	Qassign<Qbit> assign(*this, expr);
	return assign;
}

Qexpr<Qbit> Qbit::operator~() const
{
	Qbit inverted("~" + id());
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
    pOp->operands(inverted.clone(), {clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qand::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator&(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qand::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nand(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnand::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nand(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnand::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qor::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator|(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qor::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nor(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnor::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::nor(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnor::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::unlike(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qxor::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::unlike(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qxor::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::alike(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnxor::cMark);
	Qbit out(pOp->outId());
    pOp->operands(out.clone(), {clone(), right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::alike(const Qexpr<Qbit>& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qnxor::cMark);
	Qbit out(pOp->outId());
	pOp->operands(out.clone(), {clone(), right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator==(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qeq::cMark);
    pOp->operands(clone(), {right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator==(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qeq::cMark);
    pOp->operands(clone(), {right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator!=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator!=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qneq::cMark);
    pOp->operands(clone(), {right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qgt::cMark);
    pOp->operands(clone(), {right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qgt::cMark);
    pOp->operands(clone(), {right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}
Qexpr<Qbit> Qbit::operator>=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qge::cMark);
    pOp->operands(clone(), {right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator>=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qge::cMark);
    pOp->operands(clone(), {right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qlt::cMark);
    pOp->operands(clone(), {right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qlt::cMark);
    pOp->operands(clone(), {right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<=(const Qbit& right) const
{
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qle::cMark);
    pOp->operands(clone(), {right.clone()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}

Qexpr<Qbit> Qbit::operator<=(const Qexpr<Qbit>& right) const {
	QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qle::cMark);
    pOp->operands(clone(), {right.rootDef()});
	Qexpr<Qbit> expr(pOp);
	return expr;
}