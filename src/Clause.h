/*
 * Clause.h
 *
 *  Created on: Aug 27, 2015
 *      Author: Timeroot
 */

#ifndef CLAUSE_H_
#define CLAUSE_H_

#include <unordered_set>

#define VAR(x) abs(x)
#define TRUTHY(x) (x>0)

struct VarAssignment {
	int val;
	VarAssignment(int x) : val(x) {}
	VarAssignment(): val(0) {}
};

struct ClauseHash;

class Clause {
	friend ClauseHash;

public:

	int n=0;
	int a=0,b=0,c=0;

	Clause(const Clause& other);
	Clause(); // == False
	Clause(int a_);
	Clause(int a_, int b_);
	Clause(int a_, int b_, int c_);
	bool operator==(const Clause& other) const;

	bool mentions(int var) const;

	//Returns true if this clause is satisfied and is unnecessary.
	//Otherwise, returns the reduced clause in dest. If dest.n == 0, then
	//it is unsatisfiable. Otherwise it contains the remaining components.
	//This clause itself is unmodified
	bool assign(VarAssignment newAss, Clause& dest) const;
};


struct ClauseHash {
	size_t operator () (const Clause &f) const {
		return ((f.a + f.b + f.c) * 0x111100) ^ ( ((f.a * f.b) ^ (f.a * f.c) ^ (f.b * f.c)) * 0xF1F0 ) ^ (f.a*f.b*f.c);
	}
};

#endif /* CLAUSE_H_ */
