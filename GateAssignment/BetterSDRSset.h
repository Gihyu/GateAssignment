#pragma once
#include"SDRSoprt.h"

//better shadow restriction set
class BetterSDRSset
{
private:
	vector<SDRSoprt * > coverRSs;

	vector<Operation *> oprt_is;
	Stand * stand_k;

	vector<Operation *>oprt_js;
	Stand * stand_l;

	//Model
	int shadowRngID;

public:
	BetterSDRSset(Stand * k, Stand * l) { stand_k = k, stand_l; }

	bool checkCover(SDRSoprt* srs);

	void pushSDRoprt(SDRSoprt* sdrsoprt) { coverRSs.push_back(sdrsoprt); }

	void pushOprt_i(Operation * oprt_i) { oprt_is.push_back(oprt_i); }

	void pushOprt_j(Operation * oprt_j) { oprt_js.push_back(oprt_j); }

	vector<Operation *> getOprt_is() { return oprt_is; }
	vector<Operation *> getOprt_js() { return oprt_js; }

	bool check_in_is(Operation * oprt);
	bool check_in_js(Operation * oprt);

	void setShadowRngID(int sdid) { shadowRngID = sdid; }
	int getShadowRngID() { return shadowRngID; }

	//model rng
	bool coverThisStand(Stand * sd);

	bool coverThisPair(Operation * oprt, Stand * sd);
};