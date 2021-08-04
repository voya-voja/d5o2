#pragma once

#include <pybind11/pybind11.h>

#include <Qdef.h>
#include <Qtype.h>
#include <Qexpr.h>
#include <Qassign.h>

namespace dann5 {
	namespace ocean {

		class Qbool : public Qcell, public Qtype
		{
		public:
			// Q definition shared pointer 
			typedef shared_ptr<Qbool> Sp;

			// Qbool default superposition value, 
			// though any value except 0 and 1 will be considered superposition
			static const Qvalue cTrue	= 'T'; // value 84
			static const Qvalue cFalse	= 'F'; // value 71

			// Initialize Q bool with a given id and supperposition value
			Qbool(const string& id) : Qtype(), Qcell(id), mValue(gSuperposition) {};

			// Initialize Q bool with a given id and value. If value is different from 'T' or 'F', 
			// Qbool into superposition state, i.e. value 'S'
			Qbool(const string& id, Qvalue value) : Qtype(), Qcell(id), mValue(value)
			{
				if (value != cTrue && value != cFalse) mValue = gSuperposition;
			};

			// copy constructor
			Qbool(const Qbool& right) : Qtype(right), Qcell(right), mValue(right.mValue) {};

			// type conversion operator
			operator Qvalue() { return mValue; };
			operator const Qvalue() const { return mValue; };

			// set the new value of this Q bit
			virtual void value(Qvalue v);

			// constant and non-constant get a value of this Q bit
			virtual Qvalue value() const;
			virtual Qvalue value();

			// Return a Qdef's shared pointer pointing to a copy of this object 
			virtual Qdef::Sp clone() const { return Qdef::Sp(new Qbool(*this)); };

			// Returns the number of Q bits that the Q binary holds
			virtual std::size_t noqbs() const noexcept { return 1; };


			// return string presentation of this Qoperand
			virtual string toString(bool decomposed = false,
									size_t bitLevel = ULLONG_MAX) const;

			// Override to set solution values from the sample set for this deffinition
			virtual void solutions(const Qsolver::Samples& samples);

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			/*** Assignment ***/
			// assignment operator changes the identity and value of this object to be
			// the same as right object
			Qbool& operator=(const Qbool& right);

			// assignment of an Q expression creates a Q bool assignment where this 
			// Q bit is an assignee
			Qassign<Qbool> operator=(const Qexpr<Qbool>& right);

			/*** Compound Assignments ***/
			// change value of this object as result of and operation between this 
			// and right Q bool
			Qbool& operator&=(const Qbool& right);

			// 'and assignment' of an Q expression creates a following Q bool assignment
			// [this] = [this] & [right]
			Qassign<Qbool> operator&=(const Qexpr<Qbool>& right);

			// change value of this object as result of or operation between this 
			// and right Q bool
			Qbool& operator|=(const Qbool& right);

			// 'or assignment' of an Q expression creates a following Q bool assignment
			// [this] = [this] | [right]
			Qassign<Qbool> operator|=(const Qexpr<Qbool>& right);

			// change value of this object as result of xor operation between this 
			// and right Q bool
			Qbool& operator^=(const Qbool& right);

			/*** Logical ***/
			// instantiate Q expression with inversion logic, e.g. for Qbool with id 'x'
			// the expression is '!x' != 'x'
			Qexpr<Qbool> operator not() const;

			// instantiate Q expression with and logic, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' && 'y'
			Qexpr<Qbool> operator and(const Qbool& right) const;

			// instantiate Q expression with or logic, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' || 'y'
			Qexpr<Qbool> operator or(const Qbool& right) const;

			// instantiate Q expression with and logic, e.g. for Qbool id 'x' and [right]
			// object the expression is 'x' && [right]
			Qexpr<Qbool> operator and(const Qexpr<Qbool>& right) const;

			// instantiate Q expression with or logic, e.g. for Qbool id 'x' and [right]
			// object the expression is 'x' || [right]
			Qexpr<Qbool> operator or(const Qexpr<Qbool>& right) const;

			/*** Comparison ***/

			// instantiate Q expression with comparison, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' == 'y'
			Qexpr<Qbool> operator==(const Qbool& right) const;

			// instantiate Q expression with comparison, e.g. for Qbool ids 'x' and 'y'
			// the expression is 'x' != 'y'
			Qexpr<Qbool> operator!=(const Qbool& right) const;

		protected:
		private:
			Qvalue mValue;
		};
	};
};