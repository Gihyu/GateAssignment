#pragma once
#include"Fleet.h"
#include"Stand.h"

enum ActType
{
	ARRIVAL = 1,
	PARKING = 2,
	DEPART = 3
};

class Node;
class Activity
{
private:

	int act_id;
	ActType act_type;

	// temporarily use int(minute) to express time
	int act_predictT; //input Time
	int act_st;   //occupation startTime
	int act_et;	  //occupation endTime
	int act_du;   //occupation duration

	Activity * nextAct;
	Activity * frontAct;
	Fleet * _fleet;

	//vector<Stand * > compa_stands;

	string flightNum;
	//if it is parking 
	pair<string, string> flightNum_pair;

	double act_preference[TOTAL_STAND];

	Stand * assign_stand;

	vector<Stand * > compa_stands;

	//TS
	Node * belongNode;
	vector<Stand * > bestStands;
	bool _MDTRdummy;   //如果是前一天停到今天的，那么就有assign_stand值以及该布尔值为true

	// others
	string _cnnAct_str; // for IO
	string _destination;
public:
	Activity(int aid, ActType at, int ast, int aet, string fn, Fleet * ft, string cnn) {
		act_id = aid, act_type = at, act_st = ast, act_et = aet,
			act_du = aet - ast, flightNum = fn, _MDTRdummy = false, _fleet = ft, _cnnAct_str = cnn;
	}

	Activity(int aid, ActType at, int ast, int aet, pair<string, string> fnp, Fleet * ft) {
		act_id = aid, act_type = at, act_st = ast, act_et = aet,
			act_du = aet - ast, flightNum_pair = fnp, _MDTRdummy = false, _fleet = ft,
			flightNum = fnp.first+"-parking-"+fnp.second;
	}

	int getActID() { return act_id; }

	void setPredictTime(int pt) { act_predictT = pt; }
	int getPredictTime() { return act_predictT; }

	string getActName() { return flightNum; }
	pair<string, string> getParkingPair() { return flightNum_pair; }
	//when there is a shot time between arrival and depture
	void modifyST(int ast) { act_st = ast , act_du = act_et- act_st; }
	void modifyET(int aet) { act_et = aet , act_du = act_et - act_st; }
	int getStartTime() { return act_st; }
	int getEndTime() { return act_et; }
	int getDuration() { return act_du; }

	void setNextAct(Activity * nact) { nextAct = nact; }
	Activity * getNextAct() { return nextAct; }
	void setFrontAct(Activity * fact) { frontAct = fact; }
	Activity * getFrontAct() { return frontAct; }

	void setAssignStand(Stand * s) { assign_stand = s; }
	Stand * getAssignStand() { return assign_stand; }

	ActType getActType() { return act_type; }
	Fleet * getFleet() { return _fleet; }

	void setActPreference(int id, double ap) { act_preference[id] = ap; }
	double * getActPreference() { return act_preference; }

	void pushCompa_stand(Stand * cs) { compa_stands.push_back(cs); }
	vector<Stand * > getCompa_stand() { return compa_stands; }
	bool isThisStandCompatible(Stand * sd);
	void cmpBestStands();
	vector<Stand *> getBestStands() { return bestStands; }

	void setBelongNode(Node * nd) { belongNode = nd; }
	Node * getBelongNode() { return belongNode; }

	void setMDTRdummy() { _MDTRdummy = true; }

	//error:" 明显调用的表达式前的XXXX"
	// 局部变量与函数名重名
	bool isMDTRdummy() { return _MDTRdummy; }

	string getCnnActStr() { return _cnnAct_str; }

	//
};