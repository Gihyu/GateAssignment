#include"ARC.h"

ARC::ARC(int aid, ARCType atp, Node * nd1, Node * nd2)
{
	arc_id = aid;
	arc_type = atp;

	link_nodes.first = nd1;
	link_nodes.second = nd2;

	link_nodes = pair<Node*, Node*> (nd1, nd2);

	// cmp weight according to the type
	// _weight = 
}

ARC::ARC(int aid, Node*nd1, Node * nd2)
{
	arc_id = aid;

	link_nodes.first = nd1;
	link_nodes.second = nd2;

	link_nodes = pair<Node*, Node*>(nd1, nd2);

	//TS page295

	if (nd1->getNodeType() == N_ACT)	
	{
		if (nd2->getNodeType() == N_ACT)
		{
			// w(ij) = -infinity if t(ij)<0
			//			beta if t(ij)>=0 && (U(i)=j or U(j)=i)
			//			-gamma * max{t(buffer)-t(ij),0} if t(ij)>=0 && U(i)!=j and U(j)!=i
			arc_type = ACT_ACT;
			Activity * act1 = nd1->getBelongAct();
			Activity * act2 = nd2->getBelongAct();
			if (Util::act_overlap_Matrix[act1->getActID()][act2->getActID()] < 0)
			{
				_weight = -Util::double_bigM;
			}
			else
			{
				if (act1->getNextAct() == act2 || act2->getNextAct() == act1)
				{
					_weight = Util::beta;
				}
				else
				{
					int minus_buffer = Util::bufferTime - Util::act_overlap_Matrix[act1->getActID()][act2->getActID()];
					
					if (minus_buffer > 0)
					{
						_weight = -Util::gamma *minus_buffer;
					}
					else
					{
						_weight = 0.0;
					}
				}
			}
		}
		// -infinity if incompatibility
		// alpha * preference
		else if (nd2->getNodeType() == N_STAND)
		{	
			arc_type = ACT_STAND;
			if (nd1->getBelongAct()->isThisStandCompatible(nd2->getBelongStand()))
			{
				_weight = Util::alpha * nd1->getBelongAct()->getActPreference()[nd2->getBelongStand()->getStandID()];
				//cout << " create stand-" << nd2->getNodeID() << "-act-" << nd1->getNodeID() << " arc weight is " << _weight << endl;
			}
			else
			{
				_weight = -Util::double_bigM;
				//cout << " create stand-" << nd2->getNodeID() << "-act-" << nd1->getNodeID() << " arc weight is " << _weight << endl;
			}
		}
	}
	// next time
	else if (nd1->getNodeType() == N_OPRT)
	{

	}
	else if (nd1->getNodeType() == N_STAND)
	{
		if (nd2->getNodeType() == N_STAND)
		{
			arc_type = STAND_STAND;
			_weight = -Util::double_bigM;
		}
		else if (nd2->getNodeType() == N_ACT)
		{
			arc_type = ACT_STAND;
			if (nd2->getBelongAct()->isThisStandCompatible(nd1->getBelongStand()))
			{
				_weight = Util::alpha * nd2->getBelongAct()->getActPreference()[nd1->getBelongStand()->getStandID()];
			}
			else
			{
				_weight = -Util::double_bigM;
			}
		}
	}
}