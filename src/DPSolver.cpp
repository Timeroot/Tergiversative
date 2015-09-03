/*
 * DPSolver.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: Timeroot
 */

#include "DPSolver.h"
#include <cstdio>

//typedef std::unordered_set<Clause,ClauseHash> clauseSet;
//typedef std::vector<VarAssignment> assignmentSet;

DPSolver::DPSolver() {
}

DPSolver::~DPSolver() {
}

VarAssignment DPSolver::chooseVar(){
	//TODO: Currently makes the (pretty uneducated) choice of whatever happens to be the first variable in the first clause.
	return ((*unsatClauses.begin()).a);
}

bool DPSolver::unitPropagate(
		clauseSet& oldClauses,
		clauseSet& newClauses,
		VarAssignment newAss,
		int& newAsses
		){

	newAsses = 0;
	toPropagate.push_front(Clause(newAss.val));

	while(toPropagate.size() > 0){
		//Each time there's a new assignment, go through the existing clauses and modify
		//those as needed. Then go through the newly created clauses and check for new units.

		VarAssignment curAss = toPropagate.front().a;
		toPropagate.pop_front();
		int var = VAR(curAss.val);
		solution.push_back(curAss);
		newAsses++;

		//TODO: Have lookup table from var -> clauses using it.
		//This is also what 'watch variables' are good for...

		std::unordered_set<Clause,ClauseHash>::iterator clauseIterator = unsatClauses.begin();
		while(clauseIterator != unsatClauses.end()){
			if(clauseIterator->a == 0){
				throw "Got junk memory! Oh no :(";
			}

			clauseChecks++;
			if(clauseIterator->mentions(var)){
				Clause oldClause = *clauseIterator;
				Clause newClause;

				//We want to save this location, since it will probably get deleted
				auto toDelete = clauseIterator;
				clauseIterator++;

				if(oldClause.assign(curAss, newClause)){
					//assign returned true, it's satisfied, no new clause.
					unsatClauses.erase(toDelete);
					oldClauses.insert(oldClause);
				} else {
					//We have a tightened constraint
					if(newClause.n==0){

						toPropagate.resize(0);
						return false; //Contradiction :(
					}
					if(newClause.n==1){
						toPropagate.push_front(newClause);
					}

					if(unsatClauses.count(newClause) == 0){
						//Add in the new one, record it in newClauses
						unsatClauses.insert(newClause);
						newClauses.insert(newClause);
					} else {
						//It's already there, we won't need to remove it later.
					}

					if(newClauses.count(oldClause) != 0){
						//This clause we're replacing is already artifical. Just drop it.
						newClauses.erase(oldClause);
					} else {
						//It's a real clause we started with, record it for re-adding
						oldClauses.insert(oldClause);
					}
						unsatClauses.erase(toDelete);
				}


			} else {
				//Otherwise, step forward normally
				clauseIterator++;
			}

		}//For each clause, check the var
	}//For each var to be propagated...

	return true;
}

//Undoes a previous assignment
void DPSolver::revert(
		clauseSet& oldClauses,
		clauseSet& newClauses,
		int newAsses
		){

	//Add old ones back in
	for(auto i=oldClauses.begin();i!=oldClauses.end();i++)
		unsatClauses.insert(*i);

	//Remove new clauses
	for(auto i=newClauses.begin();i!=newClauses.end();i++)
		unsatClauses.erase(*i);


	//.. and drop the new variable definitions
	solution.resize(solution.size()-newAsses);
}


int DPSolver::solve(clauseSet& clauses) {
	unsatClauses = clauses;
	return solve_level();
}

int DPSolver::solve_level() {

	std::unordered_set<Clause,ClauseHash> oldClauses;
	std::unordered_set<Clause,ClauseHash> newClauses;

	VarAssignment newAss = chooseVar();
	choices++;

	int newAsses;

	if(solution.size()<20){
		printf("Assuming %d (depth %i)\n",newAss.val,(int)solution.size());
		fflush(stdout);
	}

	if(unitPropagate(oldClauses, newClauses, newAss, newAsses)){
		if(unsatClauses.size()==0)
			return RESULT_SAT;
		if(solve_level() == RESULT_SAT)
			return RESULT_SAT;
	}
	revert(oldClauses, newClauses, newAsses);
	reversions++;

	newAss.val = -newAss.val; //Try with it the other way!

	if(solution.size()<20){
		printf("Instead Assuming %d (depth %i)\n",newAss.val,(int)solution.size());
		fflush(stdout);
	}

	if(unitPropagate(oldClauses, newClauses, newAss, newAsses)){
		if(unsatClauses.size()==0)
			return RESULT_SAT;
		if(solve_level() == RESULT_SAT)
			return RESULT_SAT;
	}
	revert(oldClauses, newClauses, newAsses);
	reversions++;

	return RESULT_UNSAT;
}

assignmentSet& DPSolver::getSolution(){
	return solution;
}

void DPSolver::printPerformance() {
	printf("Performance: %d choices, %d reversions, %d checks.\n",choices,reversions,clauseChecks);
}

