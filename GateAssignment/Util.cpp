#include "Operation.h"

string Util::stands_Path = string("C:/Users/gihyu/Desktop/gateSource/debugCase/stand.csv");
string Util::disebk_Path = string("C:/Users/gihyu/Desktop/gateSource/debugCase/caseIn.csv");
string Util::ebk_Path = string("C:/Users/gihyu/Desktop/gateSource/debugCase/caseOut.csv");

string Util::integrated_Path = string("C:/Users/gihyu/Desktop/gateSource/day/8-30-debug.csv");

int Util::ebkColumnStart = 6;

int Util::YEAR = 2018;
int Util::MONTH = 8;
int Util::DAY = 30;

int Util::int_bigM       = 1000000000;
double Util::double_bigM = 1000000000.00;

array<array<int, TOTAL_OPRT>, TOTAL_OPRT > Util::oprt_overlap_Matrix = {};
array<array<int, TOTAL_ACTIVITY>, TOTAL_ACTIVITY > Util::act_overlap_Matrix = {};
 int Util::btMIP_overlapRngID_Matrix[TOTAL_OPRT][TOTAL_STAND];
 int Util::btMIP_towRngID_Matrix[TOTAL_OPRT][TOTAL_STAND];


// objective parameter
double Util::alpha = 1.0;  //preference	
double Util::beta = 1.0;   //tow	
double Util::gamma = 0.1;  //bufferTime

//algorithm
Operation * Util::useForSort = NULL;

//Ejection chain algorithm
int Util::StandStartNodeIndex;
int Util::bufferTime = 30;  //minutes  t(max)
int Util::Alg_Itr = 7;

 //exact model
int Util::divide_oprt_time = 45;
int Util::CplexSolveTime = 1000;

//case
int Util::itrMark = 1000;
//int Util::disebkTime = 30;
//int Util::ebkTime = 15;
int Util::endOfOneDay = 1440;
int Util::startOfOneDay = 0;


//会出现tow
/*int Util::remote_pre_upp = 15;
int Util::remote_pre_low = 10;
int Util::contact_pre_upp = 25;
int Util::contact_pre_low = 20;
double Util::pre_divisor = 10.0;*/

//好例子 刚好差一点 找不到最优， remote stand的跳不出来
//这些参数是设置preference的
int Util::remote_pre_upp = 15;
int Util::remote_pre_low = 10;
int Util::contact_pre_upp = 23;
int Util::contact_pre_low = 18;
double Util::pre_divisor = 10.0;

//无法解析的外部符号 是忘了写XX::的
//->左边必须是类/结构 什么的  主要是include 在h和cpp之间的结构不够好
