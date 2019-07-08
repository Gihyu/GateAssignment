#pragma once
#include"ARC.h"

class Cluster
{
private:

	int cluster_id;

	vector<Node * > _nodeSet;

	//ejection chain algorithm
	double _score;

public:
	Cluster(int cid,Node * initNode) { cluster_id  = cid, _nodeSet.push_back(initNode),_score = 0.0;}
	Cluster(int cid) { cluster_id = cid,_score = 0.0; }

	int getCluID() { return cluster_id; }

	bool checkPushNode(Node * nd);

	void pushNode(Node* nd);

	void deleteNode(Node * nd);

	double getScore() { return _score; }

	double getScoreIfPush(Node * nd);

	vector<Node * > getNodeSet() { return _nodeSet; }

};