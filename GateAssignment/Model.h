#pragma once
#include"Algorithm.h"

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

//typedef IloArray<IloNumVarArray> NumVar2Matrix;
//typedef IloArray<IloNumArray> Num2Matrix;

class Model
{
private:

	static vector<Operation *>_oprtList;
	static vector<Stand *> _standList;
	// natural MIP

	// better MIP formulation
	static vector<BetterSDRSset *> _betterSDRSlist;

	IloEnv _env;
	IloModel _model;
	IloCplex _solver;
	IloObjective _obj;

	// Var.
	IloNumVarArray _assignVar;//x(ij)
	IloNumVarArray _towVar;//y(ij)

	// Cons.
	IloRangeArray _btMIP_oprtCoverRng;	// operation cover cons
	IloRangeArray _btMIP_overlapRng;	// overlap cons
	IloRangeArray _btMIP_shadowRng;		// shadow restrictions
	IloRangeArray _btMIP_towRng;		// tow calculation

public:
	Model(IO * io);

	void initialize_btMIP_Model();

	void solveMIP_betterModel();

	void end_Model();

	void run_cplex_case();
	
};