#pragma once
#include"Activity.h"

enum NodeType
{	
	N_ACT = 1,
	N_OPRT = 2,
	N_STAND = 3
};

class Cluster;
class ARC;
class Operation;	
class Node
{
private:
	int node_id;
	NodeType node_type;

	//one of these is NULL
	Stand * thisStand;
	Operation * thisOprt;
	Activity * thisAct;

	vector<ARC *> _connectArcSet;

	//ejection chain algorithm
	bool tabu;
	Cluster * belongCluster;

public:
	Node(int ndid, Stand * ts) { node_id = ndid, node_type = N_STAND, thisStand = ts; }
	Node(int ndid, Operation * to) { node_id = ndid, node_type = N_OPRT, thisOprt = to; }
	Node(int ndid, Activity * act) { node_id = ndid, node_type = N_ACT, thisAct = act; }

	int getNodeID() { return node_id; }
	Activity * getBelongAct() { return thisAct; }
	Stand * getBelongStand() { return thisStand; }
	Operation * getBelongOprt() { return thisOprt; }

	NodeType getNodeType() { return node_type; }

	void pushArc(ARC * arc) { _connectArcSet.push_back(arc); }
	ARC * getArcWithNode(Node * nd);

	void markTabu() { tabu = true; }
	void markNonTabu() { tabu = false; }
	bool isTabu() { return tabu; }

	void setBelongCluster(Cluster * clu) { belongCluster = clu; }
	Cluster * getBelongCluster() { return belongCluster; }
};
