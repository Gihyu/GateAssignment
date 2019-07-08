#include"Algorithm.h"


Algorithm::Algorithm(IO * io)
{
	_allStands = io->getAllStands();
	_allOprt = io->getAllOprt();
	_allAct = io->getAllAct();

	_allArc = io->getAllArc();
	_allNode = io->getAllNode();
}

//page 603, EJOR
//The process of one of our partner airports is performed manually and is close to the following greedy algorithm.
void Algorithm::greedy()
{	
	//1.Sort operations by increasing number of compatiblestands.
	sort(_allOprt.begin(), _allOprt.end(), Algorithm::sortCompaIncrease);

	//2.Iteratively assign each operation to the compatible and available stand that maximizes the objective function. 
	//In case of multiplicity, choose the stands in lexicographic order.

	for (int i = 0; i < _allOprt.size(); i++)
	{
		vector<Stand *> compa_stands = _allOprt[i]->getCompa_stand();
		Util::useForSort = _allOprt[i];

		// note that capacity of dummy gate is unlimited
		sort(compa_stands.begin(), compa_stands.end()-1, Algorithm::sortPrefDecrease);

		bool find_stand = false;
		for (int j = 0; j < compa_stands.size() - 1; j++)
		{
			if (compa_stands[j]->check_ava_oprt(_allOprt[i]))
			{
				find_stand = true;
				compa_stands[j]->pushOprt(_allOprt[i]);
				_allOprt[i]->setAssignStand(compa_stands[j]);
				break;
			}
		}
		// put into dummy gate
		if (!find_stand)
		{
			compa_stands[compa_stands.size() - 1]->pushOprt(_allOprt[i]);
			_allOprt[i]->setAssignStand(compa_stands[compa_stands.size() - 1]);
		}

	}
	// swap at first beacause simple move is contained before
	
	bool still_increase = true;
	while (still_increase)
	{	
		still_increase = false;
		// swap move (swap the assignment of two operations). Only moves improving the objective are performed.
		for (int k = 0; k < _allOprt.size(); k++)
		{
			for (int m = k + 1; m < _allOprt.size(); m++)
			{
				if (check_swap_oprt(_allOprt[k], _allOprt[m]))
				{
					still_increase = true;
					swap(_allOprt[k], _allOprt[m]);
					break;
				}
			}
		}

		// simple move (switch the assignment of an operation to another compatible and available stand)
		for (int n = 0; n < _allOprt.size(); n++)
		{
			for (int p = 0; p < _allStands.size(); p++)
			{
				if (check_simpleMove_oprt(_allOprt[n], _allStands[p]))
				{	
					still_increase = true;
					simpleMove(_allOprt[n], _allStands[p]);
					break;
				}
			}
		}
	}
	
}

bool Algorithm::check_swap_oprt(Operation * a, Operation * b)
{	
	//check overlap
	if (a->getAssignStand()->getStandtype() != DUMMY)
	{
		vector<Operation*> a_list = a->getAssignStand()->getOprtList();

		for (auto & aoprt : a_list)
		{
			if (aoprt != a)
			{
				//cmp with b
				if (Util::oprt_overlap_Matrix[aoprt->getOprtID()][b->getOprtID()] < 0)
				{
					return false;
				}
			}
		}

	}
	
	if (b->getAssignStand()->getStandtype() != DUMMY)
	{
		vector<Operation*> b_list = b->getAssignStand()->getOprtList();
		for (auto & boprt : b_list)
		{
			if (boprt != b)
			{
				//cmp with a
				if (Util::oprt_overlap_Matrix[boprt->getOprtID()][a->getOprtID()] < 0)
				{
					return false;
				}
			}
		}
	}
	

	//check preference better
	double now_a = a->getOprtPreference()[a->getAssignStand()->getStandID()];
	double now_b = b->getOprtPreference()[b->getAssignStand()->getStandID()];

	double future_a = a->getOprtPreference()[b->getAssignStand()->getStandID()];
	double future_b = b->getOprtPreference()[a->getAssignStand()->getStandID()];

	if ((future_a + future_b) > (now_a + now_b))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Algorithm::swap(Operation * a, Operation * b)
{
	Stand * astand = a->getAssignStand();
	Stand * bstand = b->getAssignStand();

	a->setAssignStand(bstand);
	b->setAssignStand(astand);

	astand->delete_oprt(a);
	bstand->delete_oprt(b);

	astand->pushOprt(b);
	bstand->pushOprt(a);

}

bool Algorithm::check_simpleMove_oprt(Operation * a, Stand * s)
{
	// robust : dummy gate also cmp
	if (s->check_ava_oprt(a))
	{
		double now_pref = a->getAssignScore();
		double future_pref = a->getOprtPreference()[s->getStandID()];
		if (future_pref > now_pref)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Algorithm::simpleMove(Operation * a, Stand * s)
{
	Stand * astand = a->getAssignStand();

	astand->delete_oprt(a);

	s->pushOprt(a);
	a->setAssignStand(s);
}



void Algorithm::Ejection_chain()
{
	Ejection_chain_initialSoln();
	Ejection_chain_nbhSearch(init_cluSet);
	Ejection_chain_changingCrrSoln();
}


// left hand side of TS page298
//Algorithm 2
void Algorithm::Ejection_chain_initialSoln()
{	
	cout << endl << "-----Ejection_chain_initialSoln-----" << endl<<endl;
	int cluCount = 0;
	//0.Start with the feasible solution in which no two vertices are in relation.
	for (auto & node : _allNode)
	{
		Cluster * newClu = new Cluster(cluCount, node);

		cout << "create cluster " << cluCount << " node " << node->getNodeID() << " type " << node->getNodeType() << endl;

		cluCount++;

		init_cluSet.push_back(newClu);

		node->setBelongCluster(newClu);

		// note that here may cause confusion between ACT and OPRT
		if (node->getNodeType() == N_ACT)
		{
			Activity * act = node->getBelongAct();
			//all arrivals
			//original paper: U(i)!=0 and U(U(i))!=0
			//no parking arrivals are included here
			//or there is not any no parking arrivals in default
			if (act->getNextAct() != NULL && act->getFrontAct() == NULL)
			{	
				Activity * nextAct = act->getNextAct();
				if (nextAct->getNextAct() != NULL)
				{	
					//P298 U(i)!=0 & U(U(i))!=0
					node->markNonTabu();
					//cout << act->getActID() << " name " << act->getActName() << " is marked nontabu" << endl;
				}
				else
				{
					node->markTabu();
				}
			}
			else
			{
				node->markTabu();
			}
		}
		else
		{
			node->markTabu();
		}
		
	}

	cout << endl;
	int initAssignCount = 0;
	for (auto & node : _allNode)
	{	
		Node * i_star = node;
		Cluster * D_star = NULL;
		double judge = 0.0;
		
		// maybe use a vector 
		// write a erase function
		
		if (!i_star->isTabu())
		{	
			cout << " now is tabuNode " << node->getNodeID()<< endl;
			Node * nextNode = i_star->getBelongAct()->getNextAct()->getBelongNode();

			//there is not any no parking arrivals in default
			//otherwise here will exist a judge-empty function
			Node * next_nextNode = nextNode->getBelongAct()->getNextAct()->getBelongNode();

			for (auto & D : init_cluSet)
			{	
				//cout << "  try to insert in cluster " << D->getCluID() << endl;
				//shadow check
				if ((D->checkPushNode(node) || node->getBelongCluster() == D) &&
					(D->checkPushNode(nextNode) || nextNode->getBelongCluster() == D) &&
					(D->checkPushNode(next_nextNode) || next_nextNode->getBelongCluster() == D))
				{
					double sumScore = function_h(node, node->getBelongCluster(), D) +
						function_h(nextNode, nextNode->getBelongCluster(), D) +
						function_h(next_nextNode, next_nextNode->getBelongCluster(), D);

					/*if (node->getNodeID()==4 && D->getCluID() == 21)
					{
						cout << "f1_" << function_h(node, node->getBelongCluster(), D) << " f2_" << function_h(nextNode, nextNode->getBelongCluster(), D) << " f3_" << function_h(next_nextNode, next_nextNode->getBelongCluster(), D)<<endl;
					}*/

					if (sumScore > judge)
					{	
						cout << "   cluster " << D->getCluID() << " score " << sumScore << " is better than before " << judge<< endl;
						judge = sumScore;
						D_star = D;
						
					}
				}
					
			}
				
		}

		// whether stop

		if ( D_star != NULL)
		{	
			initAssignCount++;
			cout << "        init assign "<< initAssignCount <<"th arrival i_star " << i_star->getNodeID()<<" name "<< i_star->getBelongAct()->getActName()<<" to cluster "<<D_star->getCluID()<<endl<< endl;
			Node * nextNode = i_star->getBelongAct()->getNextAct()->getBelongNode();

			Node * next_nextNode = nextNode->getBelongAct()->getNextAct()->getBelongNode();

			D_star->pushNode(i_star);
			i_star->getBelongCluster()->deleteNode(i_star);
			i_star->setBelongCluster(D_star);
				
			D_star->pushNode(nextNode);
			nextNode->getBelongCluster()->deleteNode(nextNode);
			nextNode->setBelongCluster(D_star);

			D_star->pushNode(next_nextNode);
			next_nextNode->getBelongCluster()->deleteNode(next_nextNode);
			next_nextNode->setBelongCluster(D_star);
		
		}
		// Denote i_start as tabu.
		i_star->markTabu();
		
	}

	cout << endl << "-----Finish Ejection_chain_initialSoln-----" << endl << endl;
}

// right hand side of TS page297 ; nbh = neighborhood
//abbVersion = abbreviation version, which means that here does not exist the duplicated solution.

//Algorithm 1
//remember to change the algorithm 1 as a funtion which should input something
void Algorithm::Ejection_chain_nbhSearch(vector<Cluster *> inputClus)
{	
	double initScore = get_clusters_score(inputClus);

	cout << endl << "----- Ejection_chain_nbhSearch-----" << endl << endl;

	bool stop = false;
	int itrCount = 0;
	while (!stop)
	{
		itrCount++;
		int r = Ejection_chain_nbhSearch_abbVersion(inputClus);
		//int r = Ejection_chain_nbhSearch_realVersion(inputClus);
		if (r == 0)
		{
			stop = true;
		}
		cout <<endl<< " print itr " << itrCount << " score :" << endl;;
		double thisItrScore = get_clusters_score(inputClus);
	}

	cout << endl << "----- Finish Ejection_chain_nbhSearch-----" << endl << endl;
}

int Algorithm::Ejection_chain_nbhSearch_abbVersion(vector<Cluster *> inputClus)
{
	for (auto & node : _allNode)
	{
		if (node->getNodeType() == N_ACT)
		{
			node->markNonTabu();
		}
		else
		{
			node->markTabu();
		}
	}
	//1.1 Finding a best move: 
	//Repeat 1.1 to the solution until all vertices are marked tabu or until no feasible neighbor can be found.

	bool stop = false;
	// r is determined by the paper
	int r = 0;
	while (!stop)
	{
		Node * i_star = NULL;
		Cluster * D_star = NULL;
		double judgeScore = 0.0;
		// Find a best move
		for (auto & node : _allNode)
		{
			if (!node->isTabu())
			{	
				//cout << " now is tabuNode" << node->getNodeID() << endl;
				Cluster * C_i = node->getBelongCluster();
				
				for (auto & D : inputClus)
				{	
					//shadow check
					if (D != C_i && D->checkPushNode(node))
					{
						double thisScore = function_h(node, C_i, D);
						// because of the symbol ">" , there may be a situation that the succeeding flight activities
						// are assigned to the different dummy gate, which will be a breach of the assumption( middle of the TS page296, left hand side)
						if (thisScore > judgeScore)
						{	
							cout << "   node "<<node->getNodeID()<<" to cluster " << D->getCluID() << " score " << thisScore << " is better than before " << judgeScore << endl;
							judgeScore = thisScore;
							i_star = node;
							D_star = D;
						}
					}
				}

			}
		}
		//Perform this move to the solution, mark the moved vertex i_star as tabu, and update all clusters.
		if (i_star != NULL && D_star != NULL)
		{	
			cout << "        assign node "<< i_star->getNodeID() << " name " << i_star->getBelongAct()->getActName() << " to cluster " << D_star->getCluID() << endl << endl;
			D_star->pushNode(i_star);
			i_star->getBelongCluster()->deleteNode(i_star);
			i_star->setBelongCluster(D_star);

			i_star->markTabu();
			r++;
		}
		else
		{
			break;
		}
		stop = true;
		for (auto & node : _allNode)
		{
			if (!node->isTabu())
			{
				stop = false;
				break;
			}
		}
	}
	return r;
}

//Algorithm 3
void Algorithm::Ejection_chain_changingCrrSoln()
{	
	cout << endl << "----- Ejection_chain_changingCrrSoln-----" << endl << endl;

	int itrCount = 0;

	//Save the result as the current solution and as the best solution
	double bestScore = get_clusters_score(init_cluSet);
	int bestIndex = 0;

	cout << " now best score is " << bestScore << " and bestSolnIndex is " << bestIndex << endl;

	_solns.push_back(init_cluSet);
	vector<Cluster *> dpc_cluSet = duplicate_clus(init_cluSet);

	// release and generate relationship
	releaseRelationWithClusters();
	generateRelationWithClusters(dpc_cluSet);

	bool stop = false;

	while (!stop)
	{	
		//If the current solution only has mandatory dummy gate assignments or 
		//if this step is reached seven times, then stop and return the best solution.
		itrCount++;
		if (itrCount > Util::Alg_Itr)
		{
			break;
		}
		
		vector<Node *> nonMDTRdummySet;
		vector<Cluster *> emptyClusters;
		for (auto & newclu : dpc_cluSet)
		{			
			if (!newclu->getNodeSet().empty())
			{	
				vector<Node *> ndSet = newclu->getNodeSet();
				bool isDummyAss = true;
				for (auto & nd : ndSet)
				{
					if (nd->getNodeType() == N_STAND)
					{
						isDummyAss = false;
						break;
					}
				}
				if (isDummyAss)
				{
					for (auto & nd : ndSet)
					{
						if (!nd->getBelongAct()->isMDTRdummy())
						{
							nonMDTRdummySet.push_back(nd);
							cout << " find that node " << nd->getNodeID() << " is nonMDTR dummy assignment" << endl;
						}
					}
				}
			}		
			else
			{
				emptyClusters.push_back(newclu);
			}
		}
		if (nonMDTRdummySet.empty())
		{
			break;
		}
		
		// 2. Changing current solution: For all nonmandatory dummy gate assignments i belongs to N do
		int emptyUseIndex = 0;
		int emptyUseIndex_max = emptyClusters.size() - 1;
		cout << endl << " Algorithm 3 : in the iteration of " << itrCount << ", there are " << emptyClusters.size() << " empty clusters. " << endl;
		for (auto ndga : nonMDTRdummySet)
		{
			// 2.1 Assigning i to a gate
			int randSize = ndga->getBelongAct()->getBestStands().size();
			int randIndex = rand() % randSize;
			
			Cluster * wantTo = ndga->getBelongAct()->getBestStands()[randIndex]->getBelongNode()->getBelongCluster();
			Cluster * belongClu = ndga->getBelongCluster();
			belongClu->deleteNode(ndga);

			if (!wantTo->getNodeSet().empty())
			{	
				cout <<endl<<"	now node " << ndga->getNodeID() << " want cluster " << wantTo->getCluID() << endl;

				//这里暂时只进行“把所有的conflict activity移去dummy gate”的操作
				//并没有原文中 还需要将其移动到“同样也是dummy assignment的前活动或后活动”的cluster
				//因为空cluster本身就蛮多（创建的时候以node为单位的），因此之后有需要再回来完善这个地方
				vector<Node *> conflictNodes;
				for (auto & wtnd : wantTo->getNodeSet())
				{
					if (wtnd->getArcWithNode(ndga)->getWeight() == -Util::double_bigM)
					{
						conflictNodes.push_back(wtnd);
						cout <<"		node " << wtnd->getNodeID() << " is conflicted" << endl;
					}
				}
				if (!conflictNodes.empty())
				{
					for (auto & cfnd : conflictNodes)
					{	
						//delete from current Cluster
						//set relationship with Cluster
						wantTo->deleteNode(cfnd);
						emptyClusters[emptyUseIndex]->pushNode(cfnd);
						cfnd->setBelongCluster(emptyClusters[emptyUseIndex]);

						cout<< "			node " << cfnd->getNodeID() << " is assigned to " << emptyClusters[emptyUseIndex]->getCluID()<< endl;

						emptyUseIndex++;
						if (emptyUseIndex > emptyUseIndex_max)
						{
							vector<Cluster *> emptyClusters_new;
							for (auto & findEmpty : dpc_cluSet)
							{
								if (findEmpty->getNodeSet().empty())
								{
									emptyClusters_new.push_back(findEmpty);
								}
							}
							emptyUseIndex = 0;
							emptyUseIndex_max = emptyClusters_new.size()-1;
							emptyClusters = emptyClusters_new;
							cout << "				now reset the size of empty cluster new size is " << emptyUseIndex_max + 1 << endl;
						}
					}
				}
				
				//assign
				wantTo->pushNode(ndga);
				ndga->setBelongCluster(wantTo);

				//2.3 Eliminate shadow conflicts

				vector<Node *> SDRSnodes;
				Fleet * f_i = ndga->getBelongAct()->getFleet();
				Stand * s_k = ndga->getBelongAct()->getBestStands()[randIndex];
				for (auto sdrs : _allSDRS)
				{
					if (sdrs->getFleet_i() == f_i && sdrs->getStand_k() == s_k )
					{	
						//j  l
						Fleet * f_j = sdrs->getFleet_j();
						Stand * s_l = sdrs->getStand_l();

						Cluster * checkClu = s_l->getBelongNode()->getBelongCluster();
						for (auto & lnd : checkClu->getNodeSet())
						{
							if (lnd->getNodeType() != N_STAND)
							{
								if (lnd->getBelongAct()->getFleet() == f_j)
								{
									SDRSnodes.push_back(lnd);
								}
							}
						}
					}
					else if(sdrs->getFleet_j() == f_i && sdrs->getStand_l() == s_k)
					{	
						// i  k
						Fleet * f_j = sdrs->getFleet_i();
						Stand * s_l = sdrs->getStand_k();

						Cluster * checkClu = s_l->getBelongNode()->getBelongCluster();
						for (auto & lnd : checkClu->getNodeSet())
						{
							if (lnd->getNodeType() != N_STAND)
							{
								if (lnd->getBelongAct()->getFleet() == f_j)
								{
									SDRSnodes.push_back(lnd);
								}
							}
						}
					}
				}
				
				if (!SDRSnodes.empty())
				{
					for (auto & sdrsnd : SDRSnodes)
					{	
						Cluster * bClu = sdrsnd->getBelongCluster();
						//delete from current Cluster
						bClu->deleteNode(sdrsnd);
						emptyClusters[emptyUseIndex]->pushNode(sdrsnd);
						sdrsnd->setBelongCluster(emptyClusters[emptyUseIndex]);
						emptyUseIndex++;
					}
				}
			}
		}

		//3. Restart: Use the current solution as initial solution of Algorithm 1.
		Ejection_chain_nbhSearch(dpc_cluSet);
		double thisItrScore = get_clusters_score(dpc_cluSet);
		_solns.push_back(dpc_cluSet);
		int thisIndex = _solns.size() - 1;
		if (thisItrScore > bestScore)
		{
			bestIndex = thisIndex;
			bestScore = thisItrScore;
			cout <<endl<< " update now best score is " << bestScore << " and bestSolnIndex is " << bestIndex << " itr is "<< itrCount<<endl<<endl;
		}
		cout << endl << " now best score is still " << bestScore << " and bestSolnIndex is " << bestIndex << " itr is " << itrCount << endl << endl;
		dpc_cluSet = duplicate_clus(dpc_cluSet);
		// release and generate relationship
		releaseRelationWithClusters();
		generateRelationWithClusters(dpc_cluSet);
	}

	final_cluSet = _solns[bestIndex];

	cout << endl << "----- Finish Ejection_chain_changingCrrSoln-----" << endl << endl;

	cout << " now print the best soln details :" << endl;
	int count = 0;
	for (auto & sln : _solns)
	{
		cout << endl << count << endl;
		double bestSc = get_clusters_score(sln);
		count++;
	}

	double bestSc = get_clusters_score(final_cluSet);

}

// left hand side of TS page297 
double Algorithm::function_h(Node * nd, Cluster * belongClu, Cluster * wanto)
{	
	if (belongClu == wanto)
	{
		return 0.0;
	}
	else
	{
		vector<Node *> nowNodes = belongClu->getNodeSet();
		double nowScore = 0.0;
		for (auto & nowNode : nowNodes)
		{
			if (nowNode != nd)
			{
				ARC * newarc = nd->getArcWithNode(nowNode);
				nowScore += newarc->getWeight();
			}
		}

		vector<Node * > futureNodes = wanto->getNodeSet();

		double futureScore = 0.0;
		for (auto & fuNode : futureNodes)
		{
			ARC * newarc = nd->getArcWithNode(fuNode);
			futureScore += newarc->getWeight();
		}

		//return (nowScore - futureScore);
		return (futureScore - nowScore  );
	}	
}


//auxiliary functions
double Algorithm::get_clusters_score(vector<Cluster *> inputClus)
{
	cout << "now print the score details: " << endl;

	double totalScore = 0.0;

	for (auto & clu : inputClus)
	{
		if (!clu->getNodeSet().empty())
		{
			Node * sd_nd = NULL;
			bool existStand = false;
			vector<Node *> nodeset = clu->getNodeSet();
			for (auto & nd : nodeset)
			{
				if (nd->getNodeType() == N_STAND)
				{
					sd_nd = nd;
					existStand = true;
					break;
				}
			}

			double cluScore = 0.0;
			for (int i = 0; i < nodeset.size() - 1; i++)
			{
				for (int j = i + 1; j < nodeset.size(); j++)
				{
					ARC * newarc = nodeset[i]->getArcWithNode(nodeset[j]);
					cluScore += newarc->getWeight();
				}
			}

			if (existStand)
			{
				cout << "the clique "<<clu->getCluID()<<" of stand " << sd_nd->getBelongStand()->getStandID() << " : activities are ";
				for (auto & nd : nodeset)
				{
					if (nd != sd_nd)
					{
						cout << nd->getBelongAct()->getActID() << "\t";
					}
				}
				cout << " || clique score is " << cluScore << endl;
			}
			else
			{
				cout << "a dummy stand clique "<<clu->getCluID()<<" : activities are ";
				for (auto & nd : nodeset)
				{
					cout << nd->getBelongAct()->getActID() << "\t";
				}
				cout << " || clique score is " << cluScore << endl;
			}

			totalScore += cluScore;
		}
	}

	cout  << " the total assignment score is " << totalScore << endl;
	return totalScore;
}

vector<Cluster *> Algorithm::duplicate_clus(vector<Cluster *> inputClus)
{	
	vector<Cluster *> newCluSet;
	//empty cluster should also be duplicated
	for (auto & inputClu : inputClus)
	{	
		int cluID = inputClu->getCluID();
		Cluster * newclu = new Cluster(cluID+Util::itrMark);
		
		if (!inputClu->getNodeSet().empty())
		{
			for (auto & nd : inputClu->getNodeSet())
			{
				newclu->pushNode(nd);
			}
		}

		newCluSet.push_back(newclu);
	}

	return newCluSet;
}

void Algorithm::generateRelationWithClusters(vector<Cluster *> inputClus)
{
	for (auto & clu : inputClus)
	{
		if (!clu->getNodeSet().empty())
		{
			vector<Node*> nodes = clu->getNodeSet();
			for (auto & nd : nodes)
			{
				nd->setBelongCluster(clu);
			}
		}
	}
}

void Algorithm::releaseRelationWithClusters()
{
	for (auto & nd : _allNode)
	{
		nd->setBelongCluster(NULL);
	}
}

bool Algorithm::is_node_in_cluster(Node * nd, Cluster * clu)
{
	if (clu->getNodeSet().empty())
	{
		return false;
	}
	else
	{
		vector<Node *> ndset = clu->getNodeSet();
		for (auto & cnd : ndset)
		{
			if (cnd == nd)
			{
				return true;
			}
		}
		return false;
	}
}