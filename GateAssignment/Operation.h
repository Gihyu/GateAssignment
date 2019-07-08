#pragma once
#include"Node.h"
class SDRSoprt;
class BetterSDRSset;

//Exact and heuristic approaches to the airport stand allocation problem
//EJOR
//page599  about operation
enum OprtType
{
	SHORT_ALL = 1,
	MEDIUM_1 = 2,  //disebk + half of waiting
	MEDIUM_2 = 3,  //half waiting + ebk
	LONG_1 = 4,  //disebk
	LONG_2 = 5,  //waiting
	LONG_3 = 6   //ebk
};

class Operation
{
private:

	int oprt_id;
	OprtType oprt_type;

	// temporarily use int to express time
	int oprt_st;
	int oprt_et;
	int oprt_du;

	// if type is 1, thest two are NULL
	Operation* nextOprt;
	Operation* frontOprt;

	double oprt_preference[TOTAL_STAND];

	//greedy algorithm needs it.
	//including dummy stand in the end
	vector<Stand * > compa_stands;
	
	vector<Activity *> act_list;  // if short then 3 elements, if medium then 2, if long then 1.
	Stand * assign_stand;

	vector<SDRSoprt *> coverSDRSoprts;

	// better MIP formulation
	vector<Operation *> _overlapSet;
	vector<BetterSDRSset *> cover_better_SDRSoprtSet;

	vector<Operation *> inWhoseOai;

	//Model 
	int coverRngID;
	// for output details
	int xVarIndex[TOTAL_STAND];
	int towVarIndex[TOTAL_STAND];

	//cmp
	Node * _belongNode;

public:
	Operation(int oid, OprtType ot, int ost, int oet){oprt_id = oid, oprt_type = ot, oprt_st = ost, oprt_et = oet, oprt_du = oet-ost;}
	int getOprtID() { return oprt_id; }

	void pushAct(Activity * act) { act_list.push_back(act); }
	vector<Activity *> getActList() { return act_list; }

	void setNextOprt(Operation * noprt) { nextOprt = noprt; }
	Operation * getNextOprt() { return nextOprt; }
	void setFrontOprt(Operation * foprt) { frontOprt = foprt; }
	Operation * getFrontOprt() { return frontOprt; }

	void setAssignStand(Stand * s) { assign_stand = s; }
	Stand * getAssignStand() { return assign_stand; }

	void pushCompa_stand(Stand * cs) { compa_stands.push_back(cs); }
	vector<Stand * > getCompa_stand() { return compa_stands; }

	OprtType getOprttype() { return oprt_type; }

	//double getOprtPreference(int stid) { return oprt_preference[stid]; }
	double * getOprtPreference() { return oprt_preference; }
	void setOprtPreference(int idx, double pre) { oprt_preference[idx] = pre; }

	double getAssignScore();

	void pushSDRSoprt(SDRSoprt * sdrsoprt) { coverSDRSoprts.push_back(sdrsoprt); }
	vector<SDRSoprt *> getSDRSoprts() { return coverSDRSoprts; }

	int getStartT() { return oprt_st; }
	int getEndT() { return oprt_et; }
	int getDuration() { return oprt_du; }

	void pushOverlap_oprt(Operation * oprt) { _overlapSet.push_back(oprt); }
	vector<Operation *> getOverlapSet() { return _overlapSet; }

	void setCoverRngID(int crid) { coverRngID = crid; }
	int getCoverRngID() { return coverRngID; }

	void pushBetterSDRSset(BetterSDRSset * bsdrs) { cover_better_SDRSoprtSet.push_back(bsdrs); }
	vector<BetterSDRSset *> getBetterSDRSset() { return cover_better_SDRSoprtSet; }

	void pushOai(Operation * oai) { inWhoseOai.push_back(oai); }
	vector<Operation *> getInWhoseOai() { return inWhoseOai; }

	bool isThisStandCompatible(Stand * sd);

	//cmp
	Node * getBelongNode() { return _belongNode;}
	void setBelongNode(Node * nd) { _belongNode = nd; }

	//model
	void setXvarIndex(int sdid, int idx) { xVarIndex[sdid] = idx; }
	int * getXvarIndexBox() { return xVarIndex; }
	void setTowVarIndex(int sdid,int tvidx) { towVarIndex[sdid] = tvidx; }
	int * getTowVarIndexBox() { return towVarIndex; }
};