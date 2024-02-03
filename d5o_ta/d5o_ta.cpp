// d5o_ta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <typeinfo>

#include <Qwhole.h>

#include <Qassign.h>
#include <Qblock.h>
#include <Qint.h>
#include <Qbinder.h>
#include <Qroutine.h>

#include <Qsolver.h>
#include <Utility.h>

#include "UTestQbit.h"
#include "UTestQbool.hpp"
#include "UTestQbin.hpp"
#include "UTestQwhole.hpp"
#include <ULint.h>

#include <algorithm>

using namespace std;
using namespace dann5::ocean;
using namespace dann5;


void qintNegativeAdd_test()
{
    std::cout << "Dann5.ocean Tests Qint Negative Add!\n";
    Qint a(1, "a"), b(1, "b"), c(2, "c"), d(1, "d"), A(1, "A", -2), _1_(2, "_1_", -1), _2_(2,"_2_", -2), _3_(2, "_3_", -3), _4_(2,"_4_",-4), _5_(3, "_5_", -5), _6_(3, "_6_", -6), _7_(3, "_7_", -7);
    cout << endl << "-1 = " << _1_.Qbin::toString();
    cout << endl << "-2 = " << _2_.Qbin::toString();
    cout << endl << "-3 = " << _3_.Qbin::toString();
    cout << endl << "-4 = " << _4_.Qbin::toString();
    cout << endl << "-5 = " << _5_.Qbin::toString();
    cout << endl << "-6 = " << _6_.Qbin::toString();
    cout << endl << "-7 = " << _7_.Qbin::toString();
    cout << endl << "A = " << A.Qbin::toString() << endl;

    Qassign<Qint> aA = A = a + b; // +c + d + _1;
    cout << endl << "aA.A = " << static_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

    cout << endl << aA << endl << endl << aA.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;

    Qanalyzer analyzeA(aA.qubo());
    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;

    aA.solve();
    Qubo q = aA.qubo();
    Qsolver s0(q, false);
    Qevaluations samples = s0.solution();
    cout << endl << aA.solutions() << endl << "min Energy : " << s0.minEnergy() << endl;
    s0.solution(cout);
}

void qintPositiveAdd_test()
{
    std::cout << "Dann5.ocean Tests Qint Positive Add!\n";
    Qint a(2, "a"), b(2, "b"), c(2, "c"), d(1, "d"), A(2, "A", 3), _1(2, "_1", 1), _2(2, "_2", 2);
    cout << endl << "1 = " << _1.Qbin::toString();
    cout << endl << "2 = " << _2.Qbin::toString();
    cout << endl << "A = " << A.Qbin::toString() << endl;

    Qassign<Qint> aA = A = a + b; // +c + d + _1;
    cout << endl << "aA.A = " << static_pointer_cast<Qbin>(as_const(aA).assignee())->Qbin::toString() << endl;

    cout << endl << aA << endl << endl << aA.toString(true) << endl;
    cout << endl << "*** Qubo ***" << endl << aA.qubo(false) << endl << aA.qubo() << endl;

    Qanalyzer analyzeA(aA.qubo());
    cout << endl << "# of nodes: " << analyzeA.nodesNo() << "\t# of branches: " << analyzeA.branchesNo() << endl;

    aA.solve();
    cout << endl << aA.solutions();
}

#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

int pymain() {
    py::scoped_interpreter python;

    auto math = py::module::import("math");
    double root_two = math.attr("sqrt")(2.0).cast<double>();

    std::cout << "The square root of 2 is: " << root_two << "\n";

    return 0;
}

void testSolver()
{
    ofstream fout("testSolver.txt");
    Qbit a("a"), b("b"), N("N");
    Qassign<Qbit> aN = N = a & b;
    Qsolver sN(aN.qubo(), false);
    fout << endl << "Nodes #: " << sN.nodesNo() << ", Threds #: " << size_t(pow(2, int(log2(sN.nodesNo()) - 0.1)) / 2) << endl;
    sN.solution(fout);

    Qint i1(2, "a"), i2(2, "b"), A(2, "A");
    fout << endl << endl << "A =>" << A.toString(true) << endl;
    Qassign<Qint> aA = A = i1 + i2;
    fout << "A =>" << aA.assignee()->toString(true) << endl;
    fout << endl << aA << endl << aA.qubo() << endl;
    clock_t begin_time = clock();
    aA.solve();
    clock_t end_time = clock();
    fout << endl << "Running time: " << to_string(float(end_time - begin_time) / CLOCKS_PER_SEC) << "s" << endl;
    fout << aA.solutions();

    Qsolver sA(aA.qubo(), false);
    fout << endl << "Nodes #: " << sA.nodesNo() << ", Threds #: " << size_t(pow(2, int(log2(sA.nodesNo()) - 0.1)) / 2) << endl;
    begin_time = clock();
    sA.solution();
    end_time = clock();
    fout << endl << "Running time: " << to_string(float(end_time - begin_time) / CLOCKS_PER_SEC) << "s" << endl;
    sA.solution(fout);
    fout.close();
}

void testPNcandidates()
{
    ifstream pnFstream;
    char line[800];
    vector<ULint> primes;
#ifdef _WINDOWS
    pnFstream.open("..//.//d5o_ta//d5o_py//PNs.txt");
#else
    pnFstream.open("//System//Volumes//Data//Developer//Work//d5o2//d5o_ta//d5o_py//PNs.txt");
#endif
    bool fo = pnFstream.is_open();
    while (pnFstream.getline(line, 800))
    {
        cout << line << endl;
        ULint prime(line);
        primes.push_back(prime);
    }
    sort(primes.begin(), primes.end());
    pnFstream.close();
    ofstream spnFstream;
#ifdef _WINDOWS
    spnFstream.open("..//.//d5o_ta//d5o_py//PNs_update.txt");
#else
    spnFstream.open("//System//Volumes//Data//Developer//Work//d5o2//d5o_ta//d5o_py//PNs_update.txt");
#endif
    ULint zero;
    for (auto candidate : primes)
    {
        size_t factorExp = size_t(candidate.noBits() / 2 + 0.5); // ROUNDUP(LOG(131072, 2) / 2, 0)
        ULint two(2), maxFactor = two.pow(factorExp);
        for (auto prime : primes)
        {
            if (prime < maxFactor)//(prime == candidate)
            {
//                cout << candidate.toString() << " is PRIME" << endl;
                spnFstream << candidate.toString() << endl;
                break;
            }
            if (candidate % prime == zero)
            {
                cout  << "**** NOT a PRIME: " << candidate.toString() << " **** divisable with: " << prime.toString() << endl;
                break;
            }
        }
    }
    spnFstream.close();
}

int main(int argc, const char * argv[])
{
    Qwhole x(2, "x"), y("y", 5), z(1, "z");
    Qexpr<Qwhole> xA = x + y + z;
    cout << xA << endl << xA.toString(true) << endl;
    cout << xA.qubo() << endl;
    cout << xA.solve();

    UTestQbit utQbit;
    utQbit.runAll(cout);
    UTestQbool utQbool;
    utQbool.runAll(cout);
    UTestQbin utQbin;
//    utQbin.friends_enemies(cout);
    utQbin.runAll(cout);
    UTestQwhole utQwhole;
    utQwhole.runAll(cout); 

//    pymain();

//    testSolver();

//    testPNcandidates();

    Qwhole a(2,"a"), b(2,"b"), A("A", 6);
    Qassign<Qwhole> addAsgn = A = a * b;
    cout << addAsgn << endl << addAsgn.toString(true) << endl;
    Qubo q = addAsgn.qubo();
    cout << addAsgn.qubo(false) << endl << q << endl;
    Qsolver slvr(q, false);
    cout << slvr.nodesNo() << " nodes with " << slvr.branchesNo() << " branches." << endl;
    slvr.solution(cout);
/*
    Qubo q0o, q1o;    // a break to 2 arbitrary qubos is not working
    size_t count = 0;
    for(auto elmnt : q)
    {
        if(count < 12)
            q0o[elmnt.first] = elmnt.second;
        else
            q1o[elmnt.first] = elmnt.second;
        count++;
    }

    Qbit a0("a0"), a1("a1"), b0("b0"), b1("b1"), A0("A0"), A1("A1"), A2("A2"),  hA0("#[A0]"), hA1("#[A0]");
    QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AdderQT::cMark);
    pOp->inputs({ a1.clone(), b1.clone(), hA0.clone() });
    pOp->output(A1.clone());
    Qassign<Qbit>   add0 = A0 = a0 ^ b0,
                    add1 = A1 = Qexpr<Qbit>(pOp),
                    add2 = A2 = hA1;
    Qubo q0 = add0.qubo(), q1 = add1.qubo(), q2 = add2.qubo(), q1p = Qubo();
    for(auto elmnt : q1)
    {
        Qkey key = elmnt.first;
        if(key.first == "#[A1]")
            key.first = "A2";
        if(elmnt.first.second == "#[A1]")
            key.second = "A2";
        q1p[key] = elmnt.second;
    }
    cout << endl << q0 << endl << q1p << endl << q2 << endl;
    Qsolver slvr0(q0, false), slvr1(q1p, false);
    cout << endl << slvr0.nodesNo() << " nodes with " << slvr0.branchesNo() << " branches." << endl;
    slvr0.solution(cout);
    Qevaluations evltns0 = slvr0.solution();
    cout << endl << slvr1.nodesNo() << " nodes with " << slvr1.branchesNo() << " branches." << endl;
    slvr1.solution(cout);
    Qevaluations evltns1 = slvr1.solution();

    Qevaluations evltns = evltns0 + evltns1;
    cout << evltns << endl;
 */
    Qubos qubos = addAsgn.qubos(10);
    Qevaluations evaluations;
    for(auto qubo : qubos)
    {
        Qsolver solver(qubo, false);
        Qevaluations evs = solver.solution();
        if(evaluations.size() == 0)
            evaluations = evs;
        else
            evaluations = evaluations + evs;
    }
    cout << endl << "**** Evaluations ****:" << endl << evaluations << endl;
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
