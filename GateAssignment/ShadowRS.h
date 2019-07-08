#pragma once
#include"Operation.h"


class ShadowRS
{
private:
	Fleet * fleet_i;
	Stand * stand_k;

	Fleet * fleet_j;	
	Stand * stand_l;

public:
	ShadowRS(Fleet * fi, Stand * sk, Fleet * fj, Stand * sl) { fleet_i = fi, stand_k = sk, fleet_j = fj, stand_l = sl; }

	Fleet * getFleet_i() { return fleet_i; }
	Fleet * getFleet_j() { return fleet_j; }

	Stand * getStand_k() { return stand_k; }
	Stand * getStand_l() { return stand_l; }
};

