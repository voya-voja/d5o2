//
//  UTestQwhole.cpp
//  d5o_ta
//
//  Created by Nebojsa Vojinovic on 2023-01-07.
//
#include "UTestQwhole.hpp"

#include <Qwhole.h>
#include <Qblock.h>
#include <Qroutine.h>
#include <Qbinder.h>
#include <Qubo.h>
#include <QuboCompiler.h>
#include <QuboAnalyzer.h>

using namespace dann5test;

using namespace dann5;
using namespace dann5::ocean;

void UTestQwhole::runAll(ostream& out)
{
    out << endl << " --- Initializations ----" << endl;
    initialization(out);
    out << endl << " --- Bitwise Expressions ----" << endl;
    bitwise(out);
    out << endl << " --- Arithmetic Expressions ----" << endl;
    arithmetic(out);
    out << endl << " --- Comparison Expressions ----" << endl;
    comparisonLogic(out);
    comparison(out);
    out << endl << " --- Assignments ----" << endl;
    assignment(out);
    out << endl << " --- Factorial ----" << endl;
    factorial(out);
    out << endl << " --- Prime numbers ----" << endl;
    prime(out);
    out << endl << " --- Prime numbers (6 * Qwhole +/- 1) algorithm ----"
        << endl;
    prime6(out);
}

void UTestQwhole::initialization(ostream& out)
{
    // Default constructor initializes Q whole with an empty id and
    // supperposition value, plus initialize Q whole with a given id and
    // supperposition value
    Qwhole dcQwhole, idQwhole("id");
    out << "Qwhole default constructor: " << dcQwhole.toString()
        << ", creates an object with empty id, i.e. '',"
        << " 0 qbits, and value 0." << endl << "Qwhole with an id only: "
        << idQwhole.toString() << " has 0 qbits and value 0, too." << endl;

    Qwhole size3(3, "size3");
    out << "Qwhole 'size3' is unknown and has 3 Qbits:" << endl;
    out << " toString() => " << size3.toString() << endl
        << " toString(false, 0) => " << size3.toString(false, 0) << endl
        << " toString(true) => " << size3.toString(true) << endl
        << " toString(true, 0) => " << size3.toString(true, 0) << endl;

    // Initialize Q whole with a given id and value.
    // If value is different from 0 or 1, Qwhole into superposition state
    Qwhole oct10("octal10", 010), dec5("dec5", 5), bin6("bin6", 0b110),
           hexA("hexA", 0xA);
    out << "For 'octal10' set to value 010:" << endl;
    out << " toString() => " << oct10.toString() << endl
        << " toString(true) => " << oct10.toString(true) << endl
        << " toString(true, 3) => " << oct10.toString(true, 3) << endl;
    out << "For 'dec5' set to value 5:" << endl;
    out << " toString() => " << dec5.toString() << endl
        << " toString(true) => " << dec5.toString(true) << endl
        << " toString(true, 1) => " << dec5.toString(true, 1) << endl;
    out << "For 'bin6' set to value 0b110:" << endl;
    out << " toString() => " << bin6.toString() << endl
        << " toString(true) => " << bin6.toString(true) << endl
        << " toString(true, 2) => " << bin6.toString(true, 2) << endl;
    out << "For 'hexA' set to value 0xA:" << endl;
    out << " toString() => " << hexA.toString() << endl
        << " toString(true) => " << hexA.toString(true) << endl
        << " toString(true, 3) => " << hexA.toString(true, 3) << endl;

    Qwhole qBits("bin6qBits", Qbits(Qbin(bin6)));
    out << "Qbits of 'bin6' have been used to construct '"
        << qBits.toString() << endl;

    Qwhole cQwhole(hexA);
    out << "A copy of 'hexA' has id '" << cQwhole.id() << "' and value "
        << Bits(Qbin(cQwhole)) << endl;
}

void UTestQwhole::bitwise(ostream& out)
{
    Qwhole x(3, "x"), y("y", 5), _0("_0", 0), _1("_1", 1);
    Qexpr<Qbin> xI = ~x;
 
    QuboCompiler compiler; xI.compile(compiler);
    out << "Expression '~x', INVERT (not) x is: " << xI << endl
        << " decomposed logic: " << xI.toString(true) << endl;
    out << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << xI.solve() << endl;

    Qexpr<Qbin> qbExpr = x & y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression AND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nand(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NAND: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(4);
    out << "**** x has been resized to 4 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x | y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression OR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nor(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression NOR: " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    x[2].value(0);
    out << "**** x's 2nd qbit is set to 0 ****" << endl
        << " decomposed: " << x.toString(true) << endl << endl;

    qbExpr = x ^ y;
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression UNLIKE (XOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    qbExpr = x.nxor(y);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Expression ALIKE (NXOR): " << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;

    x.resize(2); y.resize(2);
    out << "**** x & y have been resized to 2 qbits ****" << endl
        << " decomposed: " << x.toString(true) << endl
        << " decomposed: " << y.toString(true) << endl << endl;

    qbExpr = (_1 & (x.nand(y))) ^ (Qbin(_0) *= (y | x));
    QuboCompiler noFnlCmplr(false); qbExpr.compile(noFnlCmplr);
    compiler.reset(); qbExpr.compile(compiler);
    out << "Complex Expression" << endl << qbExpr << endl
        << " decomposed logic: " << qbExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qbExpr.solve() << endl;
}

void UTestQwhole::arithmetic(ostream& out)
{
    Qwhole x(2, "x"), y("y", 5), z(1, "z"), _3("_3", 3);
    Qexpr<Qwhole> qwExpr = x + y;
    QuboCompiler noFnlCmplr(false); qwExpr.compile(noFnlCmplr);
    QuboCompiler compiler; qwExpr.compile(compiler);
    out << "Addition Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y + x;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Addition Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    Qexpr<Qwhole> qwwExpr = x + y + z + _3;// +qwExpr;
    noFnlCmplr.reset(); qwwExpr.compile(noFnlCmplr);
    compiler.reset(); qwwExpr.compile(compiler);
    out << "Addition Expression" << endl << qwwExpr << endl
        << " decomposed logic: " << qwwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwwExpr.solve() << endl;

    Qexpr<Qwhole> qxwExpr = qwExpr + z + _3;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Addition Expression" << endl << qxwExpr << endl
        << " decomposed logic: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << "'" << endl << " resulting in :" << endl << qxwExpr.solve() << endl;

    qwExpr = x + y;
    qxwExpr = z + qwExpr + _3;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Addition Expression" << endl << qxwExpr << endl
        << " decomposed logic: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxwExpr.solve() << endl;

    Qexpr<Qwhole> z_3Expr = z + _3;
    Qexpr<Qwhole> qwxExpr = x + y + z_3Expr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Addition Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qwExpr = x + y;
    z_3Expr = z + _3;
    Qexpr<Qwhole> qxxExpr = qwExpr + z_3Expr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Addition Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    qwExpr.reset(); z_3Expr.reset(); qxxExpr.reset();
    qwExpr = x + y;
    z_3Expr = z + _3;
    qxxExpr = z_3Expr + qwExpr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Addition Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    qwExpr.reset();
    qwExpr = y - x;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;
/* Issue #6 */
    qwxExpr = x - qwExpr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qxwExpr = qwExpr - x;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qxwExpr << endl
        << " decomposed logic: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxwExpr.solve() << endl;
/* Issue #6  */
    qxxExpr = z_3Expr - qwExpr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Subtraction Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    qwExpr = x * y;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwxExpr = z * qwExpr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << "'" << endl << " resulting in :" << endl << qwxExpr.solve() << endl;

    qxwExpr = qwExpr * z;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qxwExpr << endl
        << " decomposed logic: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << "'" << endl << " resulting in :" << endl << qxwExpr.solve() << endl;

    qwExpr = x * y;
    z_3Expr = _3 * z;
    qxxExpr = qwExpr * z_3Expr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Multiplication Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyze(compiler.qubo());
    out << endl << " # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo() << endl << endl;

    qwExpr = y / x;
    noFnlCmplr.reset(); qwExpr.compile(noFnlCmplr);
    compiler.reset(); qwExpr.compile(compiler);
    out << "Division Expression" << endl << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyzeD(compiler.qubo());
    out << endl << " # of nodes: " << anlyzeD.nodesNo()
        << " # of branches: " << anlyzeD.branchesNo() << endl << endl;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qxwExpr = qwExpr / z;
    noFnlCmplr.reset(); qxwExpr.compile(noFnlCmplr);
    compiler.reset(); qxwExpr.compile(compiler);
    out << "Division Expression" << endl << qxwExpr << endl
        << " decomposed logic: " << qxwExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << "'" << endl << " resulting in :" << endl << qxwExpr.solve() << endl;

    qwExpr = x + z;
    qwxExpr = y / qwExpr;
    noFnlCmplr.reset(); qwxExpr.compile(noFnlCmplr);
    compiler.reset(); qwxExpr.compile(compiler);
    out << "Division Expression" << endl << qwxExpr << endl
        << " decomposed logic: " << qwxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << qwxExpr.solve() << endl;

    qxxExpr = z_3Expr / qwExpr;
    noFnlCmplr.reset(); qxxExpr.compile(noFnlCmplr);
    compiler.reset(); qxxExpr.compile(compiler);
    out << "Division Expression" << endl << qxxExpr << endl
        << " decomposed logic: " << qxxExpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    QuboAnalyzer anlyzeD2(compiler.qubo());
    out << endl << " # of nodes: " << anlyzeD2.nodesNo()
        << " # of branches: " << anlyzeD2.branchesNo() << endl << endl;
    out << " resulting in :" << endl << qxxExpr.solve() << endl;

    Qwhole  r("r", 6), k(2, "k");
    Qexpr<Qwhole> e1 = r - x, e2 = x / k;
    Qblock blck;
    blck = e1, e2;
    noFnlCmplr.reset(); blck.compile(noFnlCmplr);
    compiler.reset(); blck.compile(compiler);
    out << "Subtraction Expression" << endl << blck << endl
        << " decomposed logic: " << blck.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << blck.solve() << endl;
}

void UTestQwhole::comparisonLogic(ostream& out)
{
    Qbit _1("_1", 1), _0("_0", 0);
    Qwhole  y(4, "y"), r(4, "r"), x(5, "x");
    Qblock neBlck;
    {
        Qexpr<Qbit> expr;
        expr = r[1] | r[0];
        for (size_t at = 2; at < r.noqbs(); at++)
            expr = r[at] | expr;
        neBlck = y = r + x,
            _1 = expr;
    }
    Qroutine notEqual("ne", neBlck);

    QuboCompiler noFnlCmplr(false); neBlck.compile(noFnlCmplr);
    QuboCompiler compiler; neBlck.compile(compiler);
    out << "Qwhole x != y " << endl << neBlck << endl
        << " decomposed logic: " << neBlck.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << neBlck.solve() << endl;

    Qevaluations solutions = neBlck.compute();
    Qbinder xyBinder(solutions);
    xyBinder = x, y;
    out << endl << xyBinder << endl;

    Qwhole  y2(4, "y"), r2(4, "r"), x2(5, "x");
    Qassign<Qwhole> a5 = x2 = r2 + y2;
    Qexpr<Qbit> a6 = _0 != x2[4];
    Qblock blck2;
    blck2 = a5, a6;
    noFnlCmplr.reset(); blck2.compile(noFnlCmplr);
    compiler.reset(); blck2.compile(compiler);
    out << "Qwhole x < y " << endl << blck2 << endl
        << " decomposed logic: " << blck2.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << blck2.solve() << endl;

    Qevaluations solutions2 = blck2.compute();
    Qbinder xyBinder2(solutions2);
    xyBinder2 = x2, y2;
    out << endl << xyBinder2 << endl;

    Qwhole  y3(3, "y"), r3(3, "_>=0"), x3(3, "x");
    Qassign<Qwhole> a7 = x3 = r3 + y3;
    Qexpr<Qbit> a8 = _0 == x3[3];
    Qblock blck3;
    blck3 = a7, a8;
    noFnlCmplr.reset(); blck3.compile(noFnlCmplr);
    compiler.reset(); blck3.compile(compiler);
    out << "Qwhole x >= y " << endl << blck3 << endl
        << " decomposed logic: " << blck3.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << blck3.solve() << endl;

    Qevaluations solutions3 = blck3.compute();
    Qbinder xyBinder3(solutions3);
    xyBinder3 = x3, y3;
    out << endl << xyBinder3 << endl;

    Qwhole  y1(5, "y"), r1(4, "r"), x1(4, "x");
    Qassign<Qwhole> a3 = y1 = r1 + x1;
    Qexpr<Qbit> a4 = _0 == y1[4];
    Qblock blck1;
    blck1 = a3, a4;
    noFnlCmplr.reset(); blck1.compile(noFnlCmplr);
    compiler.reset(); blck1.compile(compiler);
    out << "Qwhole x <= y " << endl << blck1 << endl
        << " decomposed logic: " << blck1.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << blck1.solve() << endl;

    Qevaluations solutions1 = blck1.compute();
    Qbinder xyBinder1(solutions1);
    xyBinder1 = x1, y1;
    out << endl << xyBinder1 << endl;

    Qwhole  y4(5, "y"), r4(4, "r"), x4(4, "x");
    Qassign<Qwhole> a9 = y4 = r4 + x4;
    Qexpr<Qbit> a10 = _0 != y4[4];
    Qblock blck4;
    blck4 = a9, a10;
    noFnlCmplr.reset(); blck4.compile(noFnlCmplr);
    compiler.reset(); blck4.compile(compiler);
    out << "Qwhole x > y " << endl << blck4 << endl
        << " decomposed logic: " << blck4.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    out << " resulting in :" << endl << blck4.solve() << endl;

    Qevaluations solutions4 = blck4.compute();
    Qbinder xyBinder4(solutions4);
    xyBinder4 = x4, y4;
    out << endl << xyBinder4 << endl;
}

void UTestQwhole::comparison(ostream& out)
{
    Qwhole x(2, "x"), y("y", 5), z(3, "z");

    Qexpr<Qwhole> xEq = (x == y), xNe = (x != z), xXnYeZ0 = x != y == z,
        xXnYeZ1 = (x != y) == z, xXnYeZ2 = x != (y == z);

    QuboCompiler compiler; xEq.compile(compiler);
    out << xEq.toString() << " decomposed: " << xEq.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xEq.solve() << endl;
    out << xNe.toString() << " decomposed: " << xNe.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xNe.solve() << endl;
// ERROR ->
    out << "x != y == z decomposed: " << xXnYeZ0.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xXnYeZ0.solve() << endl;
// ERROR ->
    out << xXnYeZ1 << " decomposed: " << xXnYeZ1.toString(true) << endl
        << "Qubo: " << compiler.qubo()  << endl << xXnYeZ1.solve() << endl;
    out << xXnYeZ2 << " decomposed: " << xXnYeZ2.toString(true) << endl
        << "Qubo: " << compiler.qubo() << endl << xXnYeZ2.solve() << endl;

    Qwhole x1(2, "x"), y1(2, "y"), z1(2, "z"), w1(2, "w");
    Qexpr<Qwhole> comp = x1 + y1 == z1 * w1;
    QuboCompiler noFnlCmplr(false); comp.compile(noFnlCmplr);
    compiler.reset(); comp.compile(compiler);
    out << endl << comp << endl << endl << comp.toString(true) << endl;
    out << endl << "*** Qubo ***" << endl
        << endl << compiler.qubo() << endl;
    QuboAnalyzer analyzeM(compiler.qubo());
    out << endl << "# of nodes: " << analyzeM.nodesNo()
        << "\t# of branches: " << analyzeM.branchesNo() << endl;
    comp.solve();
    out << endl << comp.solutions();

    Qwhole x2(2, "x"), y2(2, "y"), z2(2, "z"), w2(2, "w");
    Qexpr<Qwhole> comp2 = x2 + y2 != z2 + w2;
    noFnlCmplr.reset(); comp2.compile(noFnlCmplr);
    compiler.reset(); comp2.compile(compiler);
    out << endl << comp2 << endl << endl << comp2.toString(true) << endl;
    out << endl << "*** Qubo ***" << endl
    << endl << compiler.qubo() << endl;
    QuboAnalyzer analyze2(compiler.qubo());
    out << endl << "# of nodes: " << analyze2.nodesNo()
        << "\t# of branches: " << analyze2.branchesNo() << endl;
// Error: ->
    out << endl << comp2.solve() << endl << "ERROR: [QwholeExpr] != [right] issue needs to be fixed!";

    Qexpr<Qwhole> qwExpr = x > y;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Expression > (GT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y < x;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Reversed expression > (GT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out  << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = x >= y;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Expression >= (GE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out  << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y <= x;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Reversed expression >= (GE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out  << " resulting in :" << endl << qwExpr.solve() << endl;

    Qwhole  x3(3, "x"), y3(2, "y"), z3(1, "z");
    Qexpr<Qwhole> xpr = x3 <= y3 + z3;
    noFnlCmplr.reset(); xpr.compile(noFnlCmplr);
    compiler.reset(); xpr.compile(compiler);
    out << "Qwhole x <= y + z " << endl << xpr << endl
        << " decomposed logic: " << xpr.toString(true) << endl
        << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl;
    out << "'" << endl << " & finalized Qubo is '"
        << compiler.qubo()<< "'" << endl;
    //    out << " resulting in :" << endl << xpr.solve() << endl;
    QuboAnalyzer anlyze(compiler.qubo());
    out << endl << " # of nodes: " << anlyze.nodesNo()
        << " # of branches: " << anlyze.branchesNo() << endl << endl;
    Qevaluations solutions = xpr.compute();
    Qbinder xyBinder(solutions);
    xyBinder = x1, y1, z1;
    out << endl << xyBinder << endl;

    qwExpr = x < y;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Expression < (LT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out  << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y > x;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Reversed Expression < (LT): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out  << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = x <= y;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Expression <= (LE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out  << " resulting in :" << endl << qwExpr.solve() << endl;

    qwExpr = y >= x;
    compiler.reset(); qwExpr.compile(compiler);
    out << "Reversed expression <= (LE): " << qwExpr << endl
        << " decomposed logic: " << qwExpr.toString(true) << endl
        << " It's Qubo is '" << compiler.qubo() << "'" << endl ;
    out  << " resulting in :" << endl << qwExpr.solve() << endl;
}

void UTestQwhole::assignment(ostream& out)
{
    Qwhole r(2, "r"), x(3, "x");
    Qassign<Qwhole> qwholeAssign = r = x;

    QuboCompiler compiler; qwholeAssign.compile(compiler);
    out << "Assignment 'r = x' creats logic => " << qwholeAssign << endl
        << " It's finalized (execution ready) Qubo is '"
        << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    qwholeAssign.solve();
    out << qwholeAssign.solutions() << endl;

    qwholeAssign = r += x;
    compiler.reset(); qwholeAssign.compile(compiler);
    out << endl << "Assignment 'r += x' creats logic => " << qwholeAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl;
    qwholeAssign.solve();
    out << qwholeAssign.solutions() << endl;

    qwholeAssign = r -= x;
    compiler.reset(); qwholeAssign.compile(compiler);
    out << endl << "Assignment 'r -= x' creats logic => " << qwholeAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl << qwholeAssign.solve();

    qwholeAssign = r *= x;
    compiler.reset(); qwholeAssign.compile(compiler);
    out << endl << "Assignment 'r *= x' creats logic => " << qwholeAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    out << "resulting in solutions:" << endl << qwholeAssign.solve();

    qwholeAssign = r /= x;
    compiler.reset(); qwholeAssign.compile(compiler);
    out << endl << "Assignment 'r /= x' creats logic => " << qwholeAssign
        << endl << " It's Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyzeAd(compiler.qubo());
    out << endl << "# of nodes: " << analyzeAd.nodesNo()
        << "\t# of branches: " << analyzeAd.branchesNo() << endl;
    out << "resulting in solutions:" << endl << qwholeAssign.solve();

    Qwhole y("y", 6), z(2, "z"), k(2, "k");
    Qassign<Qwhole> a1 = y = r - x, a2 = k = y - z;
    Qblock blck;
    blck = a1, a2;
    QuboCompiler noFnlCmplr(false); blck.compile(noFnlCmplr);
    compiler.reset(); blck.compile(compiler);
    out << "Subtraction Expression" << endl << blck << endl
        << " decomposed logic: " << blck.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    out  << " resulting in :" << endl << blck.solve() << endl;

    out << "Dann5.ocean Tests Qwhole addition!\n";
    Qwhole a(4, "a"), b(2, "b"), c(2, "c"), d(1, "d"), A("A", 21), _1("1_", 1);
    Qassign<Qwhole> aA = A = a + b + c + d + _1;
    noFnlCmplr.reset(); aA.compile(noFnlCmplr);
    compiler.reset(); aA.compile(compiler);
    out << endl << aA << endl << endl << aA.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyzeA(compiler.qubo());
    out << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: "
        << analyzeA.branchesNo() << endl;
    out << " resulting in :" << endl << aA.solve();

    out << endl << "--- Quantum assignment vs. equal expression";
    Qwhole ax(3, "a"), b1(2, "b"), A1("A", 7), c1(3, "c"), d1(2, "d"),
            Ae1("Ae", 7);
    Qassign<Qwhole> aA1 = A1 = ax + b1;
    compiler.reset(); aA1.compile(compiler);
    out << endl << aA1.toString() << endl << aA1.solve() << endl;
    QuboAnalyzer analyzeA1(compiler.qubo());
    out << "Assignment # of nodes: " << analyzeA1.nodesNo()
        << "\t# of branches: " << analyzeA1.branchesNo() << endl;
    
    Qexpr<Qwhole> eA1 = Ae1 == c1 + d1;
    compiler.reset(); eA1.compile(compiler);
    out << endl << eA1.toString() << endl << eA1.solve() << endl;
    QuboAnalyzer analyzeE1(compiler.qubo());
    out << "Equal expression # of nodes: " << analyzeE1.nodesNo()
        << "\t# of branches: " << analyzeE1.branchesNo() << endl;

}

void UTestQwhole::factorial(ostream& out)
{
    Qwhole M("M", 42), p(3, "p"), q(2, "q"), r(2, "r");
    Qassign<Qwhole> mM = M = p * q * r;
    QuboCompiler noFnlCmplr(false); mM.compile(noFnlCmplr);
    QuboCompiler compiler; mM.compile(compiler);
    out << endl << mM << endl << endl << mM.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyzeM(compiler.qubo());
    out << endl << "# of nodes: " << analyzeM.nodesNo() << "\t# of branches: "
        << analyzeM.branchesNo() << endl;
    const clock_t begin_time = clock();
    mM.solve();
    clock_t end_time = clock();
    out << endl << "Running time: "
        << to_string(float(end_time - begin_time) / CLOCKS_PER_SEC) << "s";
    out << endl << mM.solutions();

    out << endl << "--- Large number factorial:";
    const clock_t begin_time2 = clock();
    Qwhole M1("M", 132560640), p1(8, "p"), q1(8, "q"), r1(8, "r");
    Qassign<Qwhole> mM1 = M1 = p1 * q1 * r1;
    clock_t end_time2 = clock();
    out << endl << "Preparation time: "
        << to_string(float(end_time2 - begin_time2) / CLOCKS_PER_SEC) << "s";
    out << endl << mM1 << endl;
    compiler.reset(); mM1.compile(compiler);
    QuboAnalyzer analyzeM1(compiler.qubo());
    out << endl << "# of nodes: " << analyzeM1.nodesNo() << "\t# of branches: "
        << analyzeM1.branchesNo() << endl;

}

void UTestQwhole::prime(ostream& out)
{
    Qwhole prime(5, "p"), s(2, "s"), t(1, "t"), _1("1_", 1), _2("2_", 2);
    Qbin _3("3_", 3), _7("7_", 7);
    Qassign<Qwhole> p_s2_2t2 = prime = s * s + _2 * t * t;
    Qassign<Qbin> p3mod8 = _3 = prime & _7;
    Qassign<Qbin> s1mod2 = Qbin(_1) = s & _1;
    Qassign<Qbin> t1mod2 = Qbin(_1) = t & _1;
    Qblock gcd1;
    {
        Qwhole d(1, "d");
        gcd1 = s = s * d,
                t = t * d,
                d == _1;
    }
    Qblock primeNo;
    primeNo = p_s2_2t2, p3mod8, s1mod2, t1mod2, gcd1;
    QuboCompiler noFnlCmplr(false); primeNo.compile(noFnlCmplr);
    QuboCompiler compiler; primeNo.compile(compiler);
    out << "Prime Number\n Code" << endl << primeNo << endl
        << " Logic: " << primeNo.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyse(compiler.qubo());
    out << endl
        << "*** Prime number p = s2 + 2t2, p mod 8 = 3, gcd(s,t) = 1 has: "
        << analyse.nodesNo() << " # of node and # of branches: "
        << analyse.branchesNo() << endl;
    Qevaluations solution = primeNo.compute();
    Qbinder pst(solution);
    pst = prime, s, t;
    out << " resulting in :" << endl << pst << endl;
}

void UTestQwhole::prime6(ostream& out)
{
    Qwhole prime(4, "p"), s(2, "s"), t(1, "t"), mod(2, "m"), _1("1_", 1), _5("5_", 5), _3("3_", 3), _6("6_", 6), _8("8_", 8);
    Qblock prime6m1;
    {
        prime6m1 = prime = _6 * s - _1; // ,
//        prime6m1 = prime + _1 == _6 * s; // ,
//            prime = t * _8 + mod,
//                   mod != 5;
    }
    QuboCompiler noFnlCmplr(false); prime6m1.compile(noFnlCmplr);
    QuboCompiler compiler; prime6m1.compile(compiler);
    out << "Prime Number\n Code" << endl << prime6m1 << endl
        << " Logic: " << prime6m1.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyse(compiler.qubo());
    out << endl << "*** Prime number p = 6s - 1 has: " << analyse.nodesNo()
        << " # of node and # of branches: " << analyse.branchesNo() << endl;
    Qevaluations solution = prime6m1.compute();
    Qbinder pst(solution);
    pst = prime, s, t, mod;
    out << " resulting in :" << endl << pst << endl;

    Qblock prime6p1;
    {
        prime6p1 = prime = _6 * s + _1;
        //                    prime = t * _8 + mod,
        //                    mod != 5;
    }
    noFnlCmplr.reset(); prime6m1.compile(noFnlCmplr);
    compiler.reset(); prime6m1.compile(compiler);
    out << "Prime Number\n Code" << endl << prime6p1 << endl
        << " Logic: " << prime6p1.toString(true) << endl;
    out << " It's generic Qubo is '" << noFnlCmplr.qubo() << "'" << endl
        << " & finalized Qubo is '" << compiler.qubo() << "'" << endl;
    QuboAnalyzer analyse2(compiler.qubo());
    out << endl << "*** Prime number p = 6s + 1 has: "
        << analyse2.nodesNo() << " # of node and # of branches: " << analyse2.branchesNo() << endl;
    Qevaluations solution2 = prime6p1.compute();
    Qbinder pst2(solution2);
    pst2 = prime, s, t, mod;
    out << " resulting in :" << endl << pst2 << endl;
}
