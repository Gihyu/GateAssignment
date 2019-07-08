#pragma once
#include<vector>
#include<array>
#include<ctime>
#include<queue>
#include<iomanip>
#include <process.h>
#include <windows.h>
#include<iostream>
#include<time.h>


using namespace std;

#define MAX_THREAD 8

// I should realize the dynamic after the first case
#define TOTAL_STAND 3  //notice whether there is a dummy gate(stand)
#define TOTAL_OPRT 38
#define TOTAL_ACTIVITY 38
//case 1: 23
//case 2: 38

class Operation;
class Util
{
public:
	static string stands_Path;
	static string disebk_Path;    //ebk: embarkation
	static string ebk_Path;
	static string integrated_Path;  // PUDONG data

	//IO
	static int ebkColumnStart;

	//define schedule scope
	static int YEAR;
	static int MONTH;
	static int DAY;

	static int bufferTime;
	static int divide_oprt_time;

	static array<array<int, TOTAL_OPRT>, TOTAL_OPRT > oprt_overlap_Matrix;
	static array<array<int, TOTAL_ACTIVITY>, TOTAL_ACTIVITY > act_overlap_Matrix;
	static int btMIP_overlapRngID_Matrix[TOTAL_OPRT][TOTAL_STAND];
	static int btMIP_towRngID_Matrix[TOTAL_OPRT][TOTAL_STAND];

	static int int_bigM;
	static double double_bigM;

	// objective parameter
	static double alpha;  //preference	
	static double beta;   //tow	
	static double gamma;  //bufferTime


	//algorithm
	static Operation * useForSort;

	//Ejection chain algorithm
	static int StandStartNodeIndex;
	static int Alg_Itr;

	//exact model
	static int CplexSolveTime;

	//case
	static int itrMark;
	//static int disebkTime;
	//static int ebkTime;
	static int endOfOneDay;
	static int startOfOneDay;

	static int remote_pre_upp;
	static int remote_pre_low;
	static int contact_pre_upp;
	static int contact_pre_low;
	static double pre_divisor;
};
