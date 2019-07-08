#include"Operation.h"
#include"Stand.h"

double Stand::getPrefForOprt(Operation * oprt)
{
	double k = oprt->getOprtPreference()[stand_id];
	return k;
}

double Stand::getPref_forSort()
{ 
	return Util::useForSort->getOprtPreference()[stand_id];
}

bool Stand::check_ava_oprt(Operation*oprt)
{
	if (oprt_list.empty())
	{
		return true;
	}
	else
	{
		for (int i = 0; i < oprt_list.size(); i++)
		{				
			if (Util::oprt_overlap_Matrix[oprt_list[i]->getOprtID()][oprt->getOprtID()] < 0)
			{
				return false;
			}
		}
		return true;
	}
}


void Stand::delete_oprt(Operation * oprt)
{
	if (!oprt_list.empty())
	{
		int deleteIndex = -1;
		for (int i = 0; i< oprt_list.size(); i++)
		{
			if (oprt_list[i] == oprt)
			{
				deleteIndex = i;
				break;
			}
		}
		if (deleteIndex == -1)
		{
			cout << " here is a bug when delete one operation from its assign-stand" << endl;
		}
		else
		{
			oprt_list.erase(oprt_list.begin() + deleteIndex);
		}
	}
	else
	{
		cout << " here is a bug when delete one operation from a empty list" << endl;
	}
}