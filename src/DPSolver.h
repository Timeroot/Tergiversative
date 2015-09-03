/*
 * DPSolver.h
 *
 *  Created on: Aug 28, 2015
 *      Author: Timeroot
 */

#ifndef DPSOLVER_H_
#define DPSOLVER_H_

#include "Solver.h"

#include <stack>

class DPSolver: public Solver{

	//The current problem and solution.
	assignmentSet solution = assignmentSet();
	clauseSet unsatClauses;

	//Performance monitoring
	int choices = 0;
	int reversions = 0;
	int clauseChecks = 0;

	//For unitPropagate. Contains all the variables yet to be propagated.
	std::deque<Clause> toPropagate = std::deque<Clause>();

	//One level of the DP recursion.
	int solve_level();

	//Unit propagation. Returns false if it finds a contradiction, otherwise true.
	//Expects empty clauseSets in oldClauses and newClauses. It will write to those
	//and newAsses with information on how it modified unsatClauses and solution,
	//so that the changes can be undone if needed.
	bool unitPropagate(
			clauseSet& oldClauses,
			clauseSet& newClauses,
			VarAssignment newAss,
			int& newAsses
			);

	//Takes the oldClauses, newClauses, and newAsses constructed by unitPropagate
	//and restores unsatClauses to its previous state.
	void revert(
			clauseSet& oldClauses,
			clauseSet& newClauses,
			int newAsses
			);

	//Select a variable assignment to try.
	VarAssignment chooseVar();

public:
	virtual int solve(clauseSet& clauses) final;
	virtual assignmentSet& getSolution() final;
	virtual void printPerformance() final;

	DPSolver();
	virtual ~DPSolver();
};

#endif /* DPSOLVER_H_ */
