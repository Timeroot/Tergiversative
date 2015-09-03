/*
 * Solver.h
 *
 *  Created on: Aug 27, 2015
 *      Author: Timeroot
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#define RESULT_SAT 2
#define RESULT_UNSAT 1
#define RESULT_UNK 0

#include "Clause.h"
#include <unordered_set>
#include <vector>

typedef std::unordered_set<Clause,ClauseHash> clauseSet;
typedef std::vector<VarAssignment> assignmentSet;

class Solver {
public:
	virtual int solve(clauseSet& clauses) = 0;
	virtual assignmentSet& getSolution() = 0;
	virtual void printPerformance() = 0;
};

void printProblem(const char* prefix,std::unordered_set<Clause,ClauseHash>& clauses);

void printSolution(const char* prefix,std::vector<VarAssignment>& solution);

#endif /* SOLVER_H_ */
