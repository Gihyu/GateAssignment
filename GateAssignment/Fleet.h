#pragma once
#include"Util.h"

class Fleet
{
private:
	int fleet_id;

	//mayby can be used in the future
	int psg_cap;
	double cargo_cap;

	//maybe there is an attribute of production date
	string fleetName;


public:
	Fleet(int fid, string fn) { fleet_id = fid, fleetName = fn ; }

	string getFlettName() { return fleetName; }
};
