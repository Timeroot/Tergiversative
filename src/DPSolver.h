/*
 * DPSolver.h
 *
 *  Created on: Aug 28, 2015
 *      Author: Timeroot
 */

#ifndef DPSOLVER_H_
#define DPSOLVER_H_

#include "Solver.h"

class DPSolver: public Solver{
	int choices=0;
	int reversions=0;

public:
	virtual int solve(std::unordered_set<Clause,ClauseHash>& clauses, std::vector<VarAssignment>& solution) final;

	virtual void printPerformance() final;

	DPSolver();
	virtual ~DPSolver();
};

#endif /* DPSOLVER_H_ */
