#pragma once
#include"IO.h"

class Algorithm
{
private:

	vector<Stand * > _allStands;
	vector<Operation * > _allOprt;
	vector<Activity * > _allAct;
	vector<ShadowRS *> _allSDRS;

	vector<ARC* > _allArc;
	vector<Node * > _allNode;
	
	vector<Cluster *> init_cluSet;
	vector<Cluster *> final_cluSet;

	vector<vector<Cluster *>> _solns;

public:
	Algorithm(IO * io);

	static bool sortCompaIncrease(Operation * a, Operation*b) { return (a->getCompa_stand().size() < b->getCompa_stand().size()); }

	static bool sortPrefDecrease(Stand * a, Stand * b) { return(a->getPref_forSort()>b->getPref_forSort()); }

	static bool sortStartTimeIncrease(Operation * a, Operation *b) { return(a->getStartT() < b->getStartT()); }

	void greedy();

	bool check_swap_oprt(Operation * a, Operation * b);
	void swap(Operation * a, Operation * b);

	bool check_simpleMove_oprt(Operation * a, Stand * s);

	void simpleMove(Operation * a, Stand * s);

	void Ejection_chain();
	void Ejection_chain_initialSoln();
	void Ejection_chain_nbhSearch(vector<Cluster *> inputClus);
	int Ejection_chain_nbhSearch_abbVersion(vector<Cluster *> inputClus);
	void Ejection_chain_changingCrrSoln();

	double function_h(Node * nd, Cluster * belong, Cluster * wanto);
	static double get_clusters_score(vector<Cluster *> inputClus);
	vector<Cluster *> duplicate_clus(vector<Cluster *> inputClus);
	void generateRelationWithClusters(vector<Cluster *> inputClus);
	void releaseRelationWithClusters();
	static bool is_node_in_cluster(Node * nd, Cluster * clu);
	
	vector<Cluster *> getFinalSoln() { return final_cluSet; }
};