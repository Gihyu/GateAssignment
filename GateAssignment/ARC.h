#pragma once
#include"Node.h"

enum ARCType
{	
	// operation
	OPRT_OPRT = 1,
	OPRT_STAND = 2,
	STAND_STAND = 3,
	// activity
	ACT_ACT = 4,
	ACT_STAND = 5
};

class ARC
{
private:
	int arc_id;
	ARCType arc_type;

	//undirected
	pair<Node*, Node*> link_nodes;
	double _weight;
	
public:
	ARC(int aid, ARCType atp, Node * nd1, Node * nd2);
	ARC(int aid, Node*nd1, Node * nd2);

	ARCType getARCtype() { return arc_type; }

	pair<Node*, Node*> getNodePair(){return link_nodes; }

	double getWeight() { return _weight; }
};
