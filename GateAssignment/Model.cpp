#include"Model.h"

vector<Operation *> Model::_oprtList;
vector<Stand *> Model::_standList;
vector<BetterSDRSset *> Model::_betterSDRSlist;

Model::Model(IO * io)
{
	_oprtList = io->getAllOprt();
	_standList = io->getAllStands();
	_betterSDRSlist = io->get_better_SDRSoprt_set();

	_model = IloModel(_env, "Maximize");
	//_obj = IloMinimize(_env);
	_obj = IloMaximize(_env);

	// Var.
	_assignVar = IloNumVarArray(_env) ;	//x(ij)
	_towVar = IloNumVarArray(_env);		//y(ij)

	// Cons.
	 _btMIP_oprtCoverRng = IloRangeArray(_env);	// operation cover cons
	 _btMIP_overlapRng = IloRangeArray(_env);	// overlap cons
	 _btMIP_shadowRng = IloRangeArray(_env);	// shadow restrictions
	 _btMIP_towRng = IloRangeArray(_env);		// tow calculation

}


void Model::initialize_btMIP_Model()
{
	cout << endl << " ********************* initialization of betterMIP formulation   *********************" << endl;

	//maximize
	_obj = IloAdd(_model, IloMaximize(_env));

	/*   Constraints   */
	//operation cover cons
	for (int i = 0; i < _oprtList.size(); i++)
	{
		string coverConsName = string("Operation_") + to_string(_oprtList[i]->getOprtID());
		_oprtList[i]->setCoverRngID(i);
		//cout << coverConsName<<endl;

		_btMIP_oprtCoverRng.add(IloRange(_env, 1, 1, coverConsName.c_str()));

		//if relaxed
		//_cusCoverRng.add(IloRange(_env, 1, IloInfinity, cusConsName.c_str()));

	}
	_model.add(_btMIP_oprtCoverRng);
	cout << "# operation cover constraint:" << _btMIP_oprtCoverRng.getSize() << endl;

	//overlap cons
	int _standRngCount = 0;

	for (int i = 0; i < _oprtList.size(); i++)
	{	
		if (!_oprtList[i]->getCompa_stand().empty())
		{
			vector<Stand *> compaStands = _oprtList[i]->getCompa_stand();

			for (int j = 0; j < compaStands.size(); j++)
			{	
				// dummy stand does not have limited capacity
				if (compaStands[j]->getStandtype() != DUMMY)
				{
					int oprtID = _oprtList[i]->getOprtID();
					int standID = compaStands[j]->getStandID();
					string overlapConsName = string("Overlap_Operation_") + to_string(oprtID) + string("_Stand_") + to_string(standID);
					Util::btMIP_overlapRngID_Matrix[oprtID][standID] = _standRngCount;
					_standRngCount++;
					//cout << overlapConsName<<endl;

					_btMIP_overlapRng.add(IloRange(_env, 0, 1, overlapConsName.c_str()));
				}
				else
				{
					int oprtID = _oprtList[i]->getOprtID();
					int standID = compaStands[j]->getStandID();
					string overlapConsName = string("Overlap_Operation_") + to_string(oprtID) + string("_Stand_") + to_string(standID);
					Util::btMIP_overlapRngID_Matrix[oprtID][standID] = _standRngCount;
					_standRngCount++;
					//cout << overlapConsName<<endl;

					_btMIP_overlapRng.add(IloRange(_env, 0, IloInfinity, overlapConsName.c_str()));
				}
			}
		}	
	}
	_model.add(_btMIP_overlapRng);
	cout << "# operation overlap constraint:" << _btMIP_overlapRng.getSize() << endl;

	//shadow restrictions
	for (int i = 0; i < _betterSDRSlist.size(); i++)
	{
		string sdrsConsName = string("betterSDRS_") + to_string(i);
		_betterSDRSlist[i]->setShadowRngID(i);
		//cout << sdrsConsName<<endl;

		_btMIP_shadowRng.add(IloRange(_env, 0, 1, sdrsConsName.c_str()));
	}
	_model.add(_btMIP_shadowRng);
	cout << "# shadow constraint:" << _btMIP_shadowRng.getSize() << endl;

	//tow calculation
	int _towRngCount = 0;

	for (int i = 0; i < _oprtList.size(); i++)
	{	
		//U(i)!=0
		if (_oprtList[i]->getNextOprt()!=NULL)
		{
			vector<Stand *> compaStands = _oprtList[i]->getCompa_stand();

			for (int j = 0; j < compaStands.size(); j++)
			{
				int oprtID = _oprtList[i]->getOprtID();
				int standID = compaStands[j]->getStandID();
				string towConsName = string("towCalculate_Operation_") + to_string(oprtID) + string("_Stand_") + to_string(standID);

				Util::btMIP_towRngID_Matrix[oprtID][standID] = _towRngCount;
				_towRngCount++;
				//cout << towConsName<<endl;

				_btMIP_towRng.add(IloRange(_env, -IloInfinity, 0, towConsName.c_str()));
			}
		}
	}
	_model.add(_btMIP_towRng);
	cout << "# tow calculation constraint:" << _btMIP_towRng.getSize() << endl;


	/*   Variables   */

	//x(ij)
	int xCount = 0;
	for (int k = 0; k < _oprtList.size(); k++)
	{	
		vector<Stand *> compaStands = _oprtList[k]->getCompa_stand();
		vector<Operation * > inWhoseOai = _oprtList[k]->getInWhoseOai();
		vector<BetterSDRSset * > coverBetterSet = _oprtList[k]->getBetterSDRSset();
		int oprtID = _oprtList[k]->getOprtID();

		for (int m = 0; m < compaStands.size(); m++)
		{				
			int standID = compaStands[m]->getStandID();
			double preference = _oprtList[k]->getOprtPreference()[standID];
			

			IloNumColumn col(_env);
			col += _obj(preference * Util::alpha);

			//coverRng
			col += _btMIP_oprtCoverRng[_oprtList[k]->getCoverRngID()](1);
			//overlapRng
			col += _btMIP_overlapRng[Util::btMIP_overlapRngID_Matrix[_oprtList[k]->getOprtID()][standID]](1);
			if (!inWhoseOai.empty())
			{
				for (auto & woai : inWhoseOai)
				{	
					//both need compatibility
					if (woai->isThisStandCompatible(compaStands[m]))
					{
						col += _btMIP_overlapRng[Util::btMIP_overlapRngID_Matrix[woai->getOprtID()][standID]](1);
					}
				}
			}
			//shadow restrictions
			if (!coverBetterSet.empty())
			{
				for (auto & better : coverBetterSet)
				{
					if (better->coverThisPair(_oprtList[k], compaStands[m]))
					{
						col += _btMIP_shadowRng[better->getShadowRngID()](1);
					}
				}
			}
			//tow calculation
			// self  and  front
			if(_oprtList[k]->getNextOprt() !=NULL)
			{
				col += _btMIP_towRng[Util::btMIP_towRngID_Matrix[oprtID][standID]](1);
			}
			if (_oprtList[k]->getFrontOprt() != NULL)
			{	
				int frontID = _oprtList[k]->getFrontOprt()->getOprtID();
				// -1 !
				col += _btMIP_towRng[Util::btMIP_towRngID_Matrix[frontID][standID]](-1);
			}
			

			string varName = string("x_Opertaion") + to_string(oprtID) + string("_inStand_") + to_string(standID)
				+ string("_var_") + to_string(xCount);

			_oprtList[k]->setXvarIndex(standID, xCount);

			xCount++;
				
			/*cout << varName;*/
			_assignVar.add(IloNumVar(col, 0, 1, ILOINT, varName.c_str()));

		}
		
	}
	_model.add(_assignVar);
	cout << "# assign var x:" << _assignVar.getSize() << endl;

	//y(ij)
	int yCount = 0;

	for (int n = 0; n < _oprtList.size(); n++)
	{
		// judge if there exists this var
		if (_oprtList[n]->getNextOprt() != NULL )
		{	
			vector<Stand *> compaStands = _oprtList[n]->getCompa_stand();
			int oprtID = _oprtList[n]->getOprtID();

			for (int m = 0; m < compaStands.size(); m++)
			{	
				int standID = compaStands[m]->getStandID();

				IloNumColumn col(_env);
				col += _obj(-Util::beta);

				// -1 !
				col += _btMIP_towRng[Util::btMIP_towRngID_Matrix[oprtID][standID]](-1);

				string varName = string("y_tow_Opertaion") + to_string(oprtID) + string("_inStand_") + to_string(standID)
					+ string("_var_") + to_string(yCount);

				_oprtList[n]->setTowVarIndex(standID, yCount);

				yCount++;

				/*cout << varName;*/
				_towVar.add(IloNumVar(col, 0, IloInfinity, ILOFLOAT, varName.c_str()));
			}
		}
	}

	_model.add(_towVar);
	cout << "# tow var y:" << _towVar.getSize() << endl;


	cout << endl << " ********************* Model initialization finish *********************" << endl << endl;
}

void Model::end_Model()
{
	_env.end();
}

void Model::solveMIP_betterModel()
{
	cout << endl << "---------------------------start to solve betterModel MIP-------------------------------" << endl;
	//system("PAUSE");
	double t1 = clock();
		
	_solver = IloCplex(_model);
	_solver.exportModel("C:/Users/gihyu/Desktop/betterModel-mip.lp");
	//_solver.exportModel("C:/Users/gihyu/Desktop/gateSource/betterModel-mip.lp");

	cout << "Now print cons size:" << endl;
	cout << "# coverCons:" << _btMIP_oprtCoverRng.getSize() << endl;
	cout << "# operlapCons:" << _btMIP_overlapRng.getSize() << endl;
	cout << "# shadowCons:" << _btMIP_shadowRng.getSize() << endl;
	cout << "# towCons:" << _btMIP_towRng.getSize() << endl;


	cout << endl << "Now print var size:" << endl;
	cout << "# assign var x:" << _assignVar.getSize() << endl;
	cout << "# tow var y:" << _towVar.getSize() << endl;
	try
	{
		//_solver.setParam(IloCplex::Threads, 6);
		//_solver.setParam(IloCplex::EpGap, 0.01);
		_solver.setParam(IloCplex::TiLim, Util::CplexSolveTime);
		//_solver.setParam(IloCplex::Param::Benders::Strategy,IloCplex::BendersFull);
		_solver.solve();

	}
	catch (IloException & e) { cerr << "Concert exception caught: " << e << endl; }
	catch (...) { cerr << "Unknown exception caught" << endl; }
	double t2 = clock();

	cout << "Solution status: " << _solver.getStatus() << endl;

	if (_solver.getStatus() == IloAlgorithm::Infeasible)
	{
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! Infeasibility in IP sloving!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		return;
	}

	cout << "Optimal value: " << _solver.getObjValue() << endl;
	cout << "cplex running time is  " << t2 - t1 << "  ms." << endl << endl;

	//show details

	cout << "Now show var details" << endl << endl;
	IloNumArray xVar_soln = IloNumArray(_env);
	_solver.getValues(xVar_soln, _assignVar);
	IloNumArray yVar_soln = IloNumArray(_env);
	_solver.getValues(yVar_soln, _towVar);
	cout << " var x:" << endl;
	for (int i = 0; i < xVar_soln.getSize(); i++)
	{
		if (xVar_soln[i] > 0.001)
		{
			cout << _assignVar[i] << " : " << xVar_soln[i] << endl;
		}
	}
	cout <<endl<< " var y:" << endl;
	for (int i = 0; i < yVar_soln.getSize(); i++)
	{
		if (yVar_soln[i] > 0.001)
		{
			cout << _towVar[i] << " : " << yVar_soln[i] << endl;
		}
	}
	//next time


	cout << endl << " More details:" << endl << endl;
	for (auto * stad : _standList)
	{	
		cout << " Stand " << stad->getStandID() << " type " << stad->getStandtype() << " covers these operations : ";

		// for case output
		vector< Operation *> oprtList;

		for (auto & oprt : _oprtList)
		{
			if (oprt->isThisStandCompatible(stad))
			{
				if (xVar_soln[oprt->getXvarIndexBox()[stad->getStandID()]] > 0.001)
				{	
					oprtList.push_back(oprt);
					cout << oprt->getOprtID();
					if (yVar_soln[oprt->getTowVarIndexBox()[stad->getStandID()]] > 0.001)
					{
						Operation * nextOprt = oprt->getNextOprt();
						cout << "(tow,nextOprt-" << nextOprt->getOprtID() << " is in stand-";
						for (int findStand = 0; findStand < nextOprt->getCompa_stand().size(); findStand++)
						{
							if (xVar_soln[nextOprt->getXvarIndexBox()[nextOprt->getCompa_stand()[findStand]->getStandID()]] > 0.001)
							{
								cout << nextOprt->getCompa_stand()[findStand]->getStandID();
								break;
							}
						}
						cout << ")";
					}
					cout << ", ";
				}			
			}
		}
		cout << endl;

		sort(oprtList.begin(), oprtList.end(), Algorithm::sortStartTimeIncrease);
		cout << "	for more details:";
		for (auto oprtOrder : oprtList)
		{
			cout << oprtOrder->getOprtID() << "-start-" << oprtOrder->getStartT() << "-end-" << oprtOrder->getEndT() << ", ";
		}
		cout << endl << endl;
	}
	cout << endl;
	
}

void Model::run_cplex_case()
{
	IloEnv env;
	try {
		IloModel model(env);
		IloNumVarArray vars(env);
		vars.add(IloNumVar(env, 0.0, 40.0));  // 0 <= x1 <= 40
		vars.add(IloNumVar(env));  // 0 <= x2
		vars.add(IloNumVar(env));  // 0 <= x3
		model.add(IloMaximize(env, vars[0] + 2 * vars[1] + 3 * vars[2])); //maximize x1 + 2 x2 + 3 x3
		model.add(-vars[0] + vars[1] + vars[2] <= 20);//subject to -x1 + x2 + x3 <= 20
		model.add(vars[0] - 3 * vars[1] + vars[2] <= 30);//x1 - 3 x2 + x3 <=30
		IloCplex cplex(model);
		if (!cplex.solve()) {
			env.error() << "Failed to optimize LP." << endl;
			throw(-1);
		}
		IloNumArray vals(env);
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value = " << cplex.getObjValue() << endl;
		cplex.getValues(vals, vars);
		env.out() << "Values = " << vals << endl;
	}
	catch (IloException & e) { cerr << "Concert exception caught: " << e << endl; }
	catch (...) { cerr << "Unknown exception caught" << endl; }
	env.end();
	//system("pause");
}
