#include"Operation.h"

double Operation::getAssignScore()
{
	if (assign_stand == NULL)
	{
		cout << " there is a bug when get score but does not assign !" << endl;
		return 0.00;
	}
	else
	{
		return oprt_preference[assign_stand->getStandID()];
	}
}


bool Operation::isThisStandCompatible(Stand * sd)
{
	for(auto & compa : compa_stands)
	{
		if (compa == sd)
		{
			return true;
		}
	}
	return false;
}