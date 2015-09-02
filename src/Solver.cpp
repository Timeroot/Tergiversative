#include "Solver.h"

void printProblem(const char* prefix,std::unordered_set<Clause,ClauseHash>& clauses){
	std::unordered_set<Clause,ClauseHash>::iterator clauseIterator = clauses.begin();
	while(clauseIterator != clauses.end()){
		printf("%s: %d(%d,%d,%d)\n",prefix,clauseIterator->n,clauseIterator->a,clauseIterator->b,clauseIterator->c);
		clauseIterator++;
	}
	fflush(stdout);
}


void printSolution(const char* prefix, std::vector<VarAssignment>& solution){
	std::vector<VarAssignment>::iterator solutionIterator = solution.begin();
	while(solutionIterator != solution.end()){
		printf("%s: %d\n",prefix,solutionIterator->val);
		solutionIterator++;
	}
	fflush(stdout);
}
