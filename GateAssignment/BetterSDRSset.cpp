#include"BetterSDRSset.h"

bool BetterSDRSset::checkCover(SDRSoprt * srs)
{
	if (coverRSs.empty())
	{
		return false;
	}
	else
	{
		for (auto cover_s : coverRSs)
		{
			if (cover_s == srs)
			{
				return true;
			}
		}
		return false;
	}
}


bool BetterSDRSset::check_in_is(Operation * oprt)
{
	for (auto & i : oprt_is)
	{
		if (i == oprt)
		{
			return true;
		}
	}
	return false;
}


bool BetterSDRSset::check_in_js(Operation * oprt)
{
	for (auto & j : oprt_js)
	{
		if (j == oprt)
		{
			return true;
		}
	}
	return false;
}


bool BetterSDRSset::coverThisStand(Stand * sd)
{
	if (stand_k == sd)
	{
		return true;
	}
	else if (stand_l == sd)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BetterSDRSset::coverThisPair(Operation * oprt, Stand * sd)
{
	if (check_in_is(oprt) && stand_k == sd)
	{
		return true;
	}
	else if (check_in_js(oprt) && stand_l == sd)
	{
		return true;
	}
	else
	{
		return false;
	}
}