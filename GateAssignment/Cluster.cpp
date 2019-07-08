#include"Cluster.h"

bool Cluster::checkPushNode(Node * nd)
{	
	// can only push activity
	if (_nodeSet.empty())
	{
		return true;
	}
	else
	{
		for (int i = 0; i < _nodeSet.size(); i++)
		{	
			if (_nodeSet[i]->getNodeType() == N_ACT)
			{
				if (Util::act_overlap_Matrix[_nodeSet[i]->getBelongAct()->getActID()][nd->getBelongAct()->getActID()] < 0)
				{
					return false;
				}
			}			
		}
		return true;
	}
}

void Cluster::deleteNode(Node * nd)
{
	if (!_nodeSet.empty())
	{
		int deleteIndex = -1;
		for (int i = 0; i< _nodeSet.size(); i++)
		{
			if (_nodeSet[i] == nd)
			{
				deleteIndex = i;
				break;
			}
		}
		if (deleteIndex == -1)
		{
			cout << " here is a bug when delete one activity from its assign-stand" << endl;
		}
		else
		{	
			//score calculation
			for (auto & clu : _nodeSet)
			{
				if (clu != nd)
				{	
					ARC * delete_arc = clu->getArcWithNode(nd);
					_score -= delete_arc->getWeight();
				}
			}
			_nodeSet.erase(_nodeSet.begin() + deleteIndex);
		}
	}
	else
	{
		cout << " here is a bug when delete one activity from a empty list" << endl;
	}
}

void Cluster::pushNode(Node * nd)
{
	//score calculation
	if (!_nodeSet.empty())
	{
		for (auto & clu : _nodeSet)
		{	
			ARC * newarc = clu->getArcWithNode(nd);
			_score += newarc->getWeight();
		}
	}
	_nodeSet.push_back(nd);
}

double Cluster::getScoreIfPush(Node * nd)
{
	double futureScore = _score;
	if (!_nodeSet.empty())
	{
		for (auto & clu : _nodeSet)
		{
			ARC * newarc = clu->getArcWithNode(nd);
			futureScore += newarc->getWeight();
		}
	}

	return futureScore;
}
