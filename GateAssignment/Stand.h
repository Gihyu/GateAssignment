#pragma once

enum StandType
{
	CONTACT = 1,
	REMOTE = 2,
	DUMMY = 3
};

class Activity;
class Node;
class Operation;
class Stand
{
private:
	int stand_id;
	string stand_name;

	StandType stand_type;

	//vector<Fleet * > compa_fleets;

	//for the whole plan period, maybe it should be divided into three parts( one algo one vector )
	vector<Operation *> oprt_list;
	vector<Activity * > act_list;

	//Operation * _useForSort;

	Node * _belongNode;

public:
	Stand(int sid, string sn, StandType st) { stand_id = sid, stand_name = sn, stand_type = st; }
	int getStandID() { return stand_id; }

	string getStandName() { return stand_name; }

	void pushOprt(Operation* oprt) { oprt_list.push_back(oprt); }
	vector<Operation *> getOprtList() { return oprt_list; }

	void pushAct(Activity * act) { act_list.push_back(act); }
	vector<Activity *> getActList() { return act_list; }

	void setBelongNode(Node * nd) { _belongNode = nd; }
	Node * getBelongNode() { return _belongNode; }

	StandType getStandtype() { return stand_type; }

	double getPrefForOprt(Operation * oprt);
	//double getPrefForOprt(Operation * oprt) { return 1.0; }

	//void setUseForSort(Operation * ufs) { _useForSort = ufs; }
	double getPref_forSort();
	//double getPref_forSort() { return 1.0; }

	//bool check_ava_oprt(Opertion * oprt);
	bool check_ava_oprt(Operation*oprt);

	void delete_oprt(Operation * oprt);
};
