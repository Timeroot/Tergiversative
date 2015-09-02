//============================================================================
// Name        : Tergiversative.cpp
// Author      : Alex Meiburg
// Description : Main entry point
//============================================================================

#include <iostream>
#include <fstream>
#include <cstring>

#include "Clause.h"
#include "Solver.h"

#include "Algos.h"


int main(int argc, char ** argv) {

	if(argc!=2){
		std::cerr << "Please enter a filename" << std::endl;
		return -1;
	}

	std::ifstream infile;
	infile.open(argv[1]);

	if(!infile.is_open()){
		std::cerr << "File not found" << std::endl;
		return -1;
	}

	char line[200];


	//DIMACS format: http://www.satcompetition.org/2009/format-benchmarks2009.html
	do{
		infile.getline(line, 200);
	}while(line[0]=='c');

	char head[6];
	int nVar, nClause;
	sscanf(line, "%5c %d %d", head, &nVar, &nClause);
	if(strcmp(head,"p cnf")!=0){
		std::cerr << "Unknown problem type: \"" << line << "\"" << std::endl;
		return -2;
	}

	std::vector<Clause> clList;
	clList.reserve(nClause);

	for(int clI=0; clI<nClause; clI++){
		int varNames[3];
		int literalCount;

		for(int varI=0; ; varI++){
			int token;
			infile >> token;
			if(token == 0){
				literalCount = varI;
				break;
			}
			if(varI >= 3){
				std::cerr << "Max clause length 3; encountered more at " << clI;
				return -3;
			}
			varNames[varI] = token;
		}

		switch(literalCount){
		case 0:
			std::cerr << "0 length clause at " << clI;
			return -3;
		case 1:
			clList.push_back( Clause(varNames[0]) );
			break;
		case 2:
			clList.push_back( Clause(varNames[0],varNames[1]) );
			break;
		case 3:
			clList.push_back( Clause(varNames[0],varNames[1],varNames[2]) );
			break;
		default:
			std::cerr << "Max length clause 3; encountered " << literalCount << " at " << clI;
			return -3;
		}

	}

	Solver* s = new DPSolver();
	std::vector<VarAssignment> solution;

	std::unordered_set<Clause,ClauseHash> problem(clList.begin(), clList.end());

//	{std::vector<Clause>::iterator solutionIterator = clList.begin();
//	while(solutionIterator != clList.end()){
//		printf("Cl: %d %d %d\n",solutionIterator->a,solutionIterator->b,solutionIterator->c);
//		solutionIterator++;
//	}
//	}

	if( s->solve(problem, solution) == RESULT_SAT ){
		printSolution("Final sol",solution);
	} else {
		printf("No solution!\n");
	}

	s->printPerformance();

	puts("Done!");
	fflush(stdout);

	return 0;

}
