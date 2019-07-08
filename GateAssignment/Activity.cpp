#include"Activity.h"

bool Activity::isThisStandCompatible(Stand * sd)
{
	for (auto & compa : compa_stands)
	{
		if (compa == sd)
		{
			return true;
		}
	}
	return false;
}

void Activity::cmpBestStands()
{	
	if (!compa_stands.empty())
	{
		double bestPre = -Util::double_bigM;
		for (auto & cs : compa_stands)
		{
			if (act_preference[cs->getStandID()] > bestPre)
			{
				bestPre = act_preference[cs->getStandID()];
			}
		}

		for (auto & cs : compa_stands)
		{
			if (act_preference[cs->getStandID()] == bestPre)
			{
				bestStands.push_back(cs);
			}
		}
	}

}