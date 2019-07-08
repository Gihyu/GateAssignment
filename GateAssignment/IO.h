#pragma once
#include <fstream>  
#include <sstream> 
#include"Cluster.h"
#include"BetterSDRSset.h"

class IO
{
private:

	vector<Stand * > _allStands;
	vector<Operation * > _allOprt;
	vector<Activity * > _allAct;
	vector<Fleet *> _allFleet;

	vector<ARC* > _allArc;
	vector<Node * > _allNode;
	
	vector<ShadowRS *> _inputSDRS;

	//EJOR method
	vector<SDRSoprt * > _SDRSoprt_set;
	vector<BetterSDRSset *> _betterSDRSset;
	
	//count
	static int ebkStartIndex;
	static int parkingIndex;

public:
	IO() { ; }
	~IO();

	void generateAllNodes();
	void generateAllArcs();

	//case
	void generateAllOprt_by_dupAct();
	void generate_oprt_OverlapMatrix_by_dupAct();
	

	// note that it should be set
	void generate_act_OverlapMatrix();


	vector<Stand * > getAllStands() { return _allStands; }
	vector<Operation * > getAllOprt() { return _allOprt; }
	vector<Activity * > getAllAct() { return _allAct; }
	vector<ARC* > getAllArc() { return _allArc; }
	vector<Node * > getAllNode() { return _allNode; }

	vector<SDRSoprt *> getSDRSoprt_set() { return _SDRSoprt_set; }
	void create_better_SDRSoprt_set();
	vector<BetterSDRSset *> get_better_SDRSoprt_set() { return _betterSDRSset; }

	void create_overlapSet_forBetterMIP();

	//PUDONG data
	void Input_integrated_data();


	// case Data
	void InputCase_Arr();
	void InputCase_Dep();
	void CreateCase_Parking();
	void InputCsee_stand();
	//void CreateCase_Oprt();

	//auxiliary function
	int checkWhetherExistFleet(string fltn);
};
