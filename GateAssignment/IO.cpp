//#include"IO.h"
#include"Algorithm.h"

IO::~IO()
{

}

//PUDONG data

//void IO::Input_integrated_data()
//{
//	// 读文件  
//	ifstream inFile(Util::integrated_Path, ios::in);
//	string lineStr;
//	//vector<int > disebk_predictTimeSet;
//	//vector<int > ebk_predictTimeSet;
//
//	vector<vector<string>> disebk_str;
//	vector<vector<string>> ebk_str;
//	int lineIndex = 0;
//	
//	while (getline(inFile, lineStr))
//	{	
//		bool disebkOK = true;
//		bool ebkOK = true;
//		if (lineIndex != 0) //第一行读不读取决于文件的第一行
//		{
//			// 存成二维表结构  
//			stringstream ss(lineStr);
//			string str;
//
//			vector<string> disebk_item;
//			vector<string> ebk_item;
//
//			int columnIndex = 0;
//
//			while (getline(ss, str, ',')) // 按照逗号分隔  
//			{	
//				if (columnIndex == 0) //判断入港航班是否为None
//				{
//					if (str == "None") 
//					{	
//						disebkOK = false;
//						cout << " here disebk is failed : line " << lineIndex << endl;
//					}
//					else
//					{
//						disebk_item.push_back(str);
//					}
//				}
//				else if (columnIndex < Util::ebkColumnStart) // 入港航班信息，注意input改的话此处IO也得改
//				{	
//					if (disebkOK == true)
//					{
//						disebk_item.push_back(str);
//					}
//				}
//				else if (columnIndex == Util::ebkColumnStart) //判断出港航班是否为None
//				{
//					if (str == "None")
//					{
//						ebkOK = false;
//						cout << " here ebk is failed : line " << lineIndex << endl;
//					}
//					else
//					{
//						ebk_item.push_back(str);
//					}
//				}
//				else if (columnIndex > Util::ebkColumnStart)
//				{
//					if (ebkOK == true)
//					{
//						ebk_item.push_back(str);
//					}
//				}
//				columnIndex++;
//			}
//
//			if (!disebk_item.empty())
//			{
//				disebk_str.push_back(disebk_item);
//			}
//			if (!ebk_item.empty())
//			{
//				ebk_str.push_back(ebk_item);
//			}
//		}
//
//		lineIndex++;
//
//	}
//
//	cout << endl<<"now show input disebk details:" << endl;
//	for (auto & item : disebk_str)
//	{
//		for (auto it : item)
//		{
//			cout << it << "\t";
//		}
//		cout << endl;
//	}
//	cout << endl << "now show input ebk details:" << endl;
//	for (auto & item : ebk_str)
//	{
//		for (auto it : item)
//		{
//			cout << it << "\t";
//		}
//		cout << endl;
//	}
//	cout << endl<<" num of disebk is " << disebk_str.size() << " and num of ebk is " << ebk_str.size() << endl;
//
//
//	//generate arrival activity and fleet
//	int disebkCount = 0;
//	for (int i = 0; i < disebk_str.size(); i++)
//	{	
//		string plan_time = disebk_str[i][1]; //先判断时间是否在规划范围内
//		//2018/8/30  0:05:00
//		char *cha = (char*)plan_time.data();
//		int year, month, day, hour, minute;
//		sscanf_s(cha, "%d/%d/%d %d:%d", &year, &month, &day, &hour, &minute);
//		//cout << str << " : ";
//		//cout << year << "年" << month << "月" << day << "日" << hour << "时" << minute << "分"<<endl;
//		if (year == Util::YEAR && month == Util::MONTH && day == Util::DAY)
//		{
//			string flight_num = disebk_str[i][0];
//			string origin = disebk_str[i][2];
//			string cnn_num = disebk_str[i][3];
//			string PD_gate = disebk_str[i][4];  
//			string fleet_type = disebk_str[i][5];
//
//			int plan_t = hour * 60 + minute;
//
//			int st = plan_t - Util::bufferTime / 2; //入港是前面需要buffer
//			int et = st + Util::bufferTime;
//
//			int fltIndex = checkWhetherExistFleet(fleet_type);
//			Fleet * nflt = NULL;
//			if (fltIndex == -1)
//			{
//				nflt = new Fleet(_allFleet.size(), fleet_type);
//				_allFleet.push_back(nflt);
//			}
//			else
//			{
//				nflt = _allFleet[fltIndex];
//			}
//			Activity * newAct = new Activity(disebkCount, ARRIVAL, st, et, flight_num, nflt, cnn_num);
//			// do not forget PD_gate
//			newAct->setPredictTime(plan_t);
//			_allAct.push_back(newAct);
//			disebkCount++;
//		}
//
//	}
//	ebkStartIndex = disebkCount;
//	cout <<endl<< " totally generate " << ebkStartIndex << " ARRIVAL activity and " << _allFleet.size() << " types of fleets" << endl;
//	for (auto & flt : _allFleet)
//	{
//	cout << flt->getFlettName() << endl;
//	}
//
//
//	//generate departure activity and fleet
//	int ebkCount = _allAct.size();
//	for (int i = 0; i < ebk_str.size(); i++)
//	{
//		string plan_time = disebk_str[i][8]; //先判断时间是否在规划范围内
//		//2018/8/30  0:05:00
//		char *cha = (char*)plan_time.data();
//		int year, month, day, hour, minute;
//		sscanf_s(cha, "%d/%d/%d %d:%d", &year, &month, &day, &hour, &minute);
//		//cout << str << " : ";
//		//cout << year << "年" << month << "月" << day << "日" << hour << "时" << minute << "分"<<endl;
//		if (year == Util::YEAR && month == Util::MONTH && day == Util::DAY)
//		{
//			string flight_num = ebk_str[i][6];
//			string cnn_num = ebk_str[i][7];
//			
//			//int st = predictTimeSet[i];
//			//int et = st + Util::ebkTime;
//			int st = predictTimeSet[i] - Util::bufferTime / 2;  //来是前有buffer
//			int et = st + Util::bufferTime;
//
//			int fltIndex = checkWhetherExistFleet(fltn);
//			Fleet * nflt = NULL;
//			if (fltIndex == -1)
//			{
//				nflt = new Fleet(_allFleet.size(), fltn);
//				_allFleet.push_back(nflt);
//			}
//			else
//			{
//				nflt = _allFleet[fltIndex];
//			}
//			Activity * newAct = new Activity(ebkCount, DEPART, st, et, fn, nflt, cnn);
//			newAct->setPredictTime(predictTimeSet[i]);
//			_allAct.push_back(newAct);
//			ebkCount++;
//		}
//	}
//	parkingIndex = ebkCount;
//	cout << " totally generate " << parkingIndex << " activity and " << _allFleet.size() << " types of fleets" << endl;
//
//}

//case
int IO::ebkStartIndex = 0;
int IO::parkingIndex = 0;
void IO::InputCase_Arr()
{
		// 读文件  
		ifstream inFile(Util::disebk_Path, ios::in);
		string lineStr;
		vector<int > predictTimeSet;
		vector<vector<string>> disebk_strInput;
		int lineIndex = 0;
		while (getline(inFile, lineStr))
		{	
			if (lineIndex != 0)
			{
				// 存成二维表结构  
				stringstream ss(lineStr);
				string str;
				vector<string> disebk_item;
				int columnIndex = 0;

				// 按照逗号分隔  
				while (getline(ss, str, ','))
				{	
					//第三行是预计起飞时间
					if (columnIndex == 2)
					{	
						//double atof; int atoi but return XX.00. input str
						//stoi , input string
						int pdT = atoi(str.c_str());
						predictTimeSet.push_back(pdT);
					}
					else
					{
						string f_detail = str.c_str();
						disebk_item.push_back(f_detail);
					}
					columnIndex++;
				}
				disebk_strInput.push_back(disebk_item);
			}
			lineIndex++;
		}


		//generate arrival activity and fleet
		
		int disebkCount = 0;
		for (int i = 0; i < disebk_strInput.size(); i++)
		{	
			string fn = disebk_strInput[i][0];
			string cnn = disebk_strInput[i][1];
			string fltn = disebk_strInput[i][2];
			//int st = predictTimeSet[i];
			//int et = st + Util::disebkTime;
			int st = predictTimeSet[i] - Util::bufferTime / 2;
			int et = st + Util::bufferTime;

			int fltIndex = checkWhetherExistFleet(fltn);
			Fleet * nflt = NULL;
			if (fltIndex == -1)
			{
				nflt = new Fleet(_allFleet.size(), fltn);	
				_allFleet.push_back(nflt);
			}
			else
			{
				nflt = _allFleet[fltIndex];
			}
			Activity * newAct = new Activity(disebkCount, ARRIVAL, st, et, fn, nflt, cnn);
			newAct->setPredictTime(predictTimeSet[i]);
			_allAct.push_back(newAct);
			disebkCount++;
		}
		ebkStartIndex = disebkCount;
		cout << " totally generate " << ebkStartIndex << " ARRIVAL activity and " << _allFleet.size() << " types of fleets" << endl;
		/*for (auto & flt : _allFleet)
		{
			cout << flt->getFlettName() << endl;
		}*/
}

void IO::InputCase_Dep()
{
	// 读文件  
	ifstream inFile(Util::ebk_Path, ios::in);
	string lineStr;
	vector<int > predictTimeSet;
	vector<vector<string>> ebk_strInput;
	int lineIndex = 0;
	while (getline(inFile, lineStr))
	{
		if (lineIndex != 0)
		{
			// 存成二维表结构  
			stringstream ss(lineStr);
			string str;
			vector<string> ebk_item;
			int columnIndex = 0;

			// 按照逗号分隔  
			while (getline(ss, str, ','))
			{
				//第三行是预计起飞时间
				if (columnIndex == 2)
				{
					//double atof; int atoi but return XX.00. input str
					//stoi , input string
					int pdT = atoi(str.c_str());
					predictTimeSet.push_back(pdT);
				}
				else
				{
					string f_detail = str.c_str();
					ebk_item.push_back(f_detail);
				}
				columnIndex++;
			}
			ebk_strInput.push_back(ebk_item);
		}
		lineIndex++;
	}

	//check code
	/*cout <<endl<< "now show the details of embarkation: " << endl;
	cout <<"size of disebk strVector: "<< ebk_strInput.size() << endl;
	cout <<"size of disebk predictTime: " << predictTimeSet.size() << endl;

	cout << endl << " now print the ebk matrix " << endl;
	for (int i = 0; i < ebk_strInput.size(); i++)
	{
	for (int j = 0; j < 2; j++)
	{
	cout << ebk_strInput[i][j]<< "\t";
	}
	cout << predictTimeSet[i] << "\t";
	cout << ebk_strInput[i][2];
	cout << endl;
	}*/

	//generate departure activity and fleet

	int ebkCount = _allAct.size();
	for (int i = 0; i < ebk_strInput.size(); i++)
	{
		string fn = ebk_strInput[i][0];
		string cnn = ebk_strInput[i][1];
		string fltn = ebk_strInput[i][2];
		//int st = predictTimeSet[i];
		//int et = st + Util::ebkTime;
		int st = predictTimeSet[i] - Util::bufferTime / 2;  //来是前有buffer
		int et = st + Util::bufferTime;

		int fltIndex = checkWhetherExistFleet(fltn);
		Fleet * nflt = NULL;
		if (fltIndex == -1)
		{
			nflt = new Fleet(_allFleet.size(), fltn);
			_allFleet.push_back(nflt);
		}
		else
		{
			nflt = _allFleet[fltIndex];
		}
		Activity * newAct = new Activity(ebkCount, DEPART, st, et, fn, nflt, cnn);
		newAct->setPredictTime(predictTimeSet[i]);
		_allAct.push_back(newAct);
		ebkCount++;
	}
	parkingIndex = ebkCount;
	cout << " totally generate " << parkingIndex << " activity and " << _allFleet.size() << " types of fleets" << endl;

	

}

void IO::CreateCase_Parking()
{	
	int parkID = parkingIndex;
	for (int i = 0; i < ebkStartIndex; i++)
	{
		Activity * findMatch = _allAct[i];
		string cnnDetail = findMatch->getCnnActStr();
		bool canFind = false;
		for (int j = ebkStartIndex; j < parkingIndex; j++)
		{
			if (cnnDetail == _allAct[j]->getActName())
			{
				int st = findMatch->getEndTime();
				int et = _allAct[j]->getStartTime();

				// if the time interval between them is long enough
				if (st < et)
				{
					pair<string, string> fnpair(findMatch->getActName(), cnnDetail);
					// for future use
					if (findMatch->getFleet() != _allAct[j]->getFleet())
					{
						cout << " there is a bug when generate parking activities : front Fleet is not the same as next Fleet" << endl;
					}
					Activity * parkAct = new Activity(parkID, PARKING, st, et, fnpair, findMatch->getFleet());

					findMatch->setNextAct(parkAct);
					parkAct->setFrontAct(findMatch);

					parkAct->setNextAct(_allAct[j]);
					_allAct[j]->setFrontAct(parkAct);

					_allAct.push_back(parkAct);
					parkID++;

					//setPreference score
					for (int pid = 0; pid < _allStands.size();pid++)
					{	
						if (_allStands[pid]->getStandtype() == CONTACT)
						{	
							//C++ 除号会自动根据分母取精确度 因此需要10.0
							int pre = (rand() % (Util::contact_pre_upp - Util::contact_pre_low + 1)) + Util::contact_pre_low;
							double pre_score = pre / Util::pre_divisor;
							findMatch->setActPreference(_allStands[pid]->getStandID(), pre_score);
							parkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
							_allAct[j]->setActPreference(_allStands[pid]->getStandID(), pre_score);
						}
						else if (_allStands[pid]->getStandtype() == REMOTE)
						{
							int pre = (rand() % (Util::remote_pre_upp - Util::remote_pre_low + 1)) + Util::remote_pre_low;
							double pre_score = pre / Util::pre_divisor;
							findMatch->setActPreference(_allStands[pid]->getStandID(), pre_score);
							parkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
							_allAct[j]->setActPreference(_allStands[pid]->getStandID(), pre_score);
						}
						else
						{
							findMatch->setActPreference(_allStands[pid]->getStandID(), 0.0);
							parkAct->setActPreference(_allStands[pid]->getStandID(), 0.0);
							_allAct[j]->setActPreference(_allStands[pid]->getStandID(), 0.0);
						}
					}
				}
				// case of "st = et" should not generate parking activity
				else
				{
					int overLapTime_half = (st - et)/2;
					findMatch->modifyET(st - overLapTime_half);
					_allAct[j]->modifyST(et + overLapTime_half);

					findMatch->setNextAct(_allAct[j]);
					_allAct[j]->setFrontAct(findMatch);

					//setPreference score
					for (int pid = 0; pid < _allStands.size(); pid++)
					{
						if (_allStands[pid]->getStandtype() == CONTACT)
						{
							int pre = (rand() % (25 - 15 + 1)) + 15;
							double pre_score = pre / 10.0;
							findMatch->setActPreference(_allStands[pid]->getStandID(), pre_score);
							_allAct[j]->setActPreference(_allStands[pid]->getStandID(), pre_score);
						}
						else if (_allStands[pid]->getStandtype() == REMOTE)
						{
							int pre = (rand() % (15 - 5 + 1)) + 5;
							double pre_score = pre / 10.0;
							findMatch->setActPreference(_allStands[pid]->getStandID(), pre_score);
							_allAct[j]->setActPreference(_allStands[pid]->getStandID(), pre_score);
						}
						else
						{
							findMatch->setActPreference(_allStands[pid]->getStandID(), 0.0);
							_allAct[j]->setActPreference(_allStands[pid]->getStandID(), 0.0);
						}
					}
				}

				canFind = true;
				break;
			}
		}

		// create a parking act till the end of one day
		if (!canFind)
		{
			int st = findMatch->getEndTime();
			int et = Util::endOfOneDay;
			pair<string, string> fnpair(findMatch->getActName(), findMatch->getActName());

			Activity * parkAct = new Activity(parkID, PARKING, st, et, fnpair, findMatch->getFleet());

			findMatch->setNextAct(parkAct);
			parkAct->setFrontAct(findMatch);

			//setPreference score
			for (int pid = 0; pid < _allStands.size(); pid++)
			{
				if (_allStands[pid]->getStandtype() == CONTACT)
				{
					int pre = (rand() % (25 - 15 + 1)) + 15;
					double pre_score = pre / 10.0;
					findMatch->setActPreference(_allStands[pid]->getStandID(), pre_score);
					parkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
				}
				else if (_allStands[pid]->getStandtype() == REMOTE)
				{
					int pre = (rand() % (15 - 5 + 1)) + 5;
					double pre_score = pre / 10.0;
					findMatch->setActPreference(_allStands[pid]->getStandID(), pre_score);
					parkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
				}
				else
				{
					findMatch->setActPreference(_allStands[pid]->getStandID(), 0.0);
					parkAct->setActPreference(_allStands[pid]->getStandID(), 0.0);
				}
			}

			_allAct.push_back(parkAct);
			parkID++;
		}
	}

	//for future use : parking at the beginning of one day
	for (int k = ebkStartIndex; k < parkingIndex; k++)
	{
		Activity * ebkAct = _allAct[k];
		string cnnDetail = ebkAct->getCnnActStr();
		bool canMatch = false;
		for (int m = 0; m < ebkStartIndex; m++)
		{
			if (cnnDetail == _allAct[m]->getActName())
			{
				canMatch = true;
				break;
			}
		}

		if (!canMatch)
		{
			int st = Util::startOfOneDay;
			int et =ebkAct->getStartTime();
			pair<string, string> fnpair(ebkAct->getActName(), ebkAct->getActName());

			Activity * parkAct = new Activity(parkID, PARKING, st, et, fnpair, ebkAct->getFleet());

			parkAct->setNextAct(ebkAct);
			ebkAct->setFrontAct(parkAct);

			//setPreference score
			for (int pid = 0; pid < _allStands.size(); pid++)
			{
				if (_allStands[pid]->getStandtype() == CONTACT)
				{
					int pre = (rand() % (25 - 15 + 1)) + 15;
					double pre_score = pre / 10.0;
					ebkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
					parkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
				}
				else if (_allStands[pid]->getStandtype() == REMOTE)
				{
					int pre = (rand() % (15 - 5 + 1)) + 5;
					double pre_score = pre / 10.0;
					ebkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
					parkAct->setActPreference(_allStands[pid]->getStandID(), pre_score);
				}
				else
				{
					ebkAct->setActPreference(_allStands[pid]->getStandID(), 0.0);
					parkAct->setActPreference(_allStands[pid]->getStandID(), 0.0);
				}
			}

			_allAct.push_back(parkAct);
			parkID++;
		}
	}

	cout << " totally generate " << _allAct.size() << " activity and " << _allFleet.size() << " types of fleets" << endl;
	//check code
	cout << endl << "ActID" << "\t" << "ActName" << "\t" << "nextActID" <<"\t"<<"nextActName"<< "\t"<< "PredictTime" << "\t" <<"StartTime"<<"\t" "EndTime" << "\t" << "Fleet" << "\t" << "TYPE" << "\t" <<"compaStandSize"<<"\t"<< "stand_0_pre" "\t" << "stand_1_pre" "\t" << "stand_2_pre" "\t" << endl;
	for (auto & act : _allAct)
	{	
		//setCompaStand
		for (auto & st : _allStands)
		{
			act->pushCompa_stand(st);
		}

		//cmp best stands for ejection_alg_3
		act->cmpBestStands();

		string pdt;
		if (act->getActType() == PARKING)
		{
			pdt = "-";
		}
		else
		{
			pdt = std::to_string(act->getPredictTime());
		}
	
		if (act->getNextAct() != NULL)
		{
			cout << act->getActID() << "\t" << act->getActName() << "\t" << act->getNextAct()->getActID()<< "\t" << act->getNextAct()->getActName() << "\t"<< pdt << "\t" <<act->getStartTime() << "\t" << act->getEndTime() << "\t" << act->getFleet()->getFlettName() << "\t" << act->getActType() << "\t" << act->getCompa_stand().size() << "\t" << act->getActPreference()[0] << "\t" << act->getActPreference()[1] << "\t" << act->getActPreference()[2] << endl;
		}
		else
		{
			cout << act->getActID() << "\t" << act->getActName() << "\t" << "None" << "\t" <<"-" << "\t" << pdt << "\t" << act->getStartTime() << "\t" << act->getEndTime() << "\t" << act->getFleet()->getFlettName() << "\t" << act->getActType() << "\t" << act->getCompa_stand().size() << "\t" << act->getActPreference()[0] << "\t" << act->getActPreference()[1] << "\t" << act->getActPreference()[2] << endl;
		}
	}

	//check link function
	/*cout << endl << endl << " now check the links" << endl;;
	for (auto & act : _allAct)
	{
		cout << act->getActID() << "\t" << act->getActName();
		if (act->getNextAct() != NULL)
		{
			Activity * na = act->getNextAct();
			cout <<" next is"<< na->getActID() << " " << na->getActName();
			if (na->getNextAct() != NULL)
			{
				Activity * nna = na->getNextAct();
				cout << " next next is" << nna->getActID() << " " << nna->getActName();

			}
		}
		cout << endl;
	}*/
}

void IO::InputCsee_stand()
{
	// 读文件  
	ifstream inFile(Util::stands_Path, ios::in);
	string lineStr;
	vector<vector<string>> stand_strInput;
	int lineIndex = 0;
	while (getline(inFile, lineStr))
	{
		if (lineIndex != 0)
		{
			// 存成二维表结构  
			stringstream ss(lineStr);
			string str;
			vector<string> stand_item;
			int columnIndex = 0;

			// 按照逗号分隔  
			while (getline(ss, str, ','))
			{
				stand_item.push_back(str.c_str());
			}
			stand_strInput.push_back(stand_item);
		}
		lineIndex++;
	}

	int sdID = 0;
	for (auto & sd : stand_strInput)
	{
		string sdName = sd[0];
		string sdType = sd[1];
		if (sdType == "remote")
		{
			Stand * newsd = new Stand(sdID, sdName, REMOTE);
			_allStands.push_back(newsd);
			sdID++;
		}
		else if(sdType == "contact")
		{	
			Stand * newsd = new Stand(sdID, sdName, CONTACT);
			_allStands.push_back(newsd);
			sdID++;
		}
		else
		{
			cout << " There is a bug when generate stands : unknown type" << endl;
		}

	}

	//dummy stand
	Stand * newsd = new Stand(sdID, "DUMMY", DUMMY);
	_allStands.push_back(newsd);
	sdID++;

	cout << endl << "now print stands' details :" << endl;
	cout << "standID" << "\t" << "standName" << "\t" << "standType" << endl;
	for (auto & sd : _allStands)
	{
		cout << sd->getStandID() << "\t" << sd->getStandName() << "\t" << sd->getStandtype() << endl;
	}
}

int IO::checkWhetherExistFleet(string fltn)
{
	if (_allFleet.empty())
	{
		return -1;
	}
	else
	{
		for (int i = 0;i<_allFleet.size(); i++ )
		{
			if (_allFleet[i]->getFlettName() == fltn)
			{
				return i;
			}
		}
		return -1;
	}
}

//TS page295
void IO::generateAllNodes()
{
	// TS page295, index from 1 to n+m-1
	// for easy
	int nodeIndex = 0;
	for (auto & act : _allAct)
	{
		Node * actNode = new Node(nodeIndex, act);
		act->setBelongNode(actNode);
		nodeIndex++;
		_allNode.push_back(actNode);
	}

	Util::StandStartNodeIndex = nodeIndex;

	// the last one is dummy gate, which should not be generated as a node
	for (int i = 0; i < _allStands.size() - 1; i++)
	{
		Node * standNode = new Node(nodeIndex, _allStands[i]);
		_allStands[i]->setBelongNode(standNode);
		nodeIndex++;
		_allNode.push_back(standNode);
	}

	cout << endl << "totally generate " << _allNode.size() << " nodes" << endl;
}

void IO::generateAllArcs()
{	
	int arcIndex = 0;
	for (int i = 0; i < _allNode.size()-1; i++)
	{	
		for (int j = i+1; j < _allNode.size(); j++)
		{
			ARC * newArc = new ARC(arcIndex, _allNode[i], _allNode[j]);

			_allNode[i]->pushArc(newArc);
			_allNode[j]->pushArc(newArc);

			arcIndex++;
			_allArc.push_back(newArc);
		}
	}
}


void IO::generate_act_OverlapMatrix()
{
	//t(ij)>=0 describes buffer time, where t(ij)<0 implies an overlap of |t(ij)| time units.

	// just act
	Util::act_overlap_Matrix[TOTAL_ACTIVITY - 1][TOTAL_ACTIVITY - 1] = -_allAct[TOTAL_ACTIVITY - 1]->getDuration();
	for (int i = 0; i < _allAct.size()-1; i++)
	{	
		Util::act_overlap_Matrix[i][i] = -_allAct[i]->getDuration();
		if (Util::act_overlap_Matrix[i][i] >= 0)
		{             
			cout << " bug exist here : actOverLapMatrix[i][i] " << Util::act_overlap_Matrix[i][i] << " and index is " << i << endl;
		}
		for (int j = i + 1; j < _allAct.size(); j++)
		{
			int i_st;
			int i_et;

			int j_st;
			int j_et;
			
			//i_startT在前  
			if (_allAct[i]->getStartTime() <= _allAct[j]->getStartTime())
			{
				i_st = _allAct[i]->getStartTime();
				i_et = _allAct[i]->getEndTime();

				j_st = _allAct[j]->getStartTime();
				j_et = _allAct[j]->getEndTime();

				// 1_________1
				//   1____1
				if (j_et < i_et)
				{
					Util::act_overlap_Matrix[i][j] = j_st - j_et;
					Util::act_overlap_Matrix[j][i] = j_st - j_et;
				}
				else
				{	
					// 1_______1
					//    1________1
					if (j_st <= i_et)
					{
						Util::act_overlap_Matrix[i][j] = j_st - i_et;
						Util::act_overlap_Matrix[j][i] = j_st - i_et;
					}
					// 1_____1
					//          1______1
					else
					{
						Util::act_overlap_Matrix[i][j] = j_st - i_et;
						Util::act_overlap_Matrix[j][i] = j_st - i_et;
					}
				}
			}
			//j_startT在前
			else
			{
				i_st = _allAct[j]->getStartTime();
				i_et = _allAct[j]->getEndTime();

				j_st = _allAct[i]->getStartTime();
				j_et = _allAct[i]->getEndTime();

				// 1_________1
				//   1____1
				if (j_et < i_et)
				{
					Util::act_overlap_Matrix[i][j] = j_st - j_et;
					Util::act_overlap_Matrix[j][i] = j_st - j_et;
				}
				else
				{
					// 1_______1
					//    1________1
					if (j_st <= i_et)
					{
						Util::act_overlap_Matrix[i][j] = j_st - i_et;
						Util::act_overlap_Matrix[j][i] = j_st - i_et;
					}
					// 1_____1
					//          1______1
					else
					{
						Util::act_overlap_Matrix[i][j] = j_st - i_et;
						Util::act_overlap_Matrix[j][i] = j_st - i_et;
					}
				}
			}
		}
	}

		//check code
		
		cout << endl << " now print the dis matrix " << endl;
		for (int i = 0; i < TOTAL_ACTIVITY; i++)
		{
			for (int j = 0; j < TOTAL_ACTIVITY; j++)
			{	
				if (Util::act_overlap_Matrix[i][j]!= Util::act_overlap_Matrix[j][i])
				{
					cout << "there is a wrong dis data in row " << i << " column " << j << endl;
				}
				cout << Util::act_overlap_Matrix[i][j]<< "\t";
			}
			cout << endl;
		}
}

//EJOR P602

//case
void IO::generateAllOprt_by_dupAct()
{
	//Operation(int oid, OprtType ot, int ost, int oet){oprt_id = oid, oprt_type = ot, oprt_st = ost, oprt_et = oet, oprt_du = oet-ost;}
	//_allOprt
	int oprtID = 0;

	//enum OprtType
	//{
	//	SHORT_ALL = 1,
	//	MEDIUM_1 = 2,  //disebk + half of waiting
	//	MEDIUM_2 = 3,  //half waiting + ebk
	//	LONG_1 = 4,  //disebk
	//	LONG_2 = 5,  //waiting
	//	LONG_3 = 6   //ebk
	//}
	
	for (auto & act : _allAct)
	{	
		OprtType thistype;
		if (act->getActType() == ARRIVAL)
		{
			thistype = LONG_1;
		}
		else if(act->getActType() == PARKING)
		{
			thistype = LONG_2;
		}
		else
		{
			thistype = LONG_3;
		}
		Operation * oprt = new Operation(oprtID, thistype, act->getStartTime(), act->getEndTime());
		_allOprt.push_back(oprt);

		for (auto & st : _allStands)
		{
			oprt->pushCompa_stand(st);
			oprt->setOprtPreference(st->getStandID(), act->getActPreference()[st->getStandID()]);
		}

		//setCover Act
		oprt->pushAct(act);

		oprtID++;
	}

	//set front next
	for (auto & oprt : _allOprt)
	{
		for (int i = 0; i < _allOprt.size(); i++)
		{
			if (_allOprt[i] != oprt)
			{
				Activity * thisAct = oprt->getActList()[0];
				Activity * cmpAct = _allOprt[i]->getActList()[0];

				if (thisAct->getNextAct() == cmpAct)
				{
					oprt->setNextOprt(_allOprt[i]);
					_allOprt[i]->setFrontOprt(oprt);
				}
				else if(thisAct->getFrontAct() == cmpAct)
				{
					oprt->setFrontOprt(_allOprt[i]);
					_allOprt[i]->setNextOprt(oprt);
				}
			}
		}
	}

}

void IO::generate_oprt_OverlapMatrix_by_dupAct()
{
	Util::oprt_overlap_Matrix = Util::act_overlap_Matrix;
}

class Algorithm;
void IO::create_better_SDRSoprt_set()
{
	sort(_allOprt.begin(), _allOprt.end(), Algorithm::sortStartTimeIncrease);

	for(auto & sdrs: _SDRSoprt_set)
	{	
		if (!sdrs->getCoverStatus())
		{	
			sdrs->setCovered();

			Operation * o_i = sdrs->getOprt_i();
			Operation * o_j = sdrs->getOprt_j();

			Stand * s_k = sdrs->getStand_k();
			Stand * s_l = sdrs->getStand_l();

			BetterSDRSset * betterSet = new BetterSDRSset(s_k, s_l);
			betterSet->pushOprt_i(o_i);
			// in order to get the rngID
			o_i->pushBetterSDRSset(betterSet);

			betterSet->pushOprt_j(o_j);
			o_j->pushBetterSDRSset(betterSet);

			betterSet->pushSDRoprt(sdrs);

			//fullfill o_is
			for (int i = 0; i < _allOprt.size(); i++)
			{
				if (_allOprt[i] != o_i && _allOprt[i] != o_j)
				{
					vector<Operation *> o_is = betterSet->getOprt_is();
					// must overlap all i
					bool overlapAll = true;
					for (auto oi : o_is)
					{
						if (Util::oprt_overlap_Matrix[oi->getOprtID()][_allOprt[i]->getOprtID()] >= 0)
						{
							overlapAll = false;
						}
					}

					if (overlapAll)
					{
						// check exist one SDRSoprt
						for (int j = 0; j < _SDRSoprt_set.size(); j++)
						{
							Operation * target_i = _SDRSoprt_set[j]->getOprt_i();
							Operation * target_j = _SDRSoprt_set[j]->getOprt_j();

							Stand * target_k = _SDRSoprt_set[j]->getStand_k();
							Stand * target_l = _SDRSoprt_set[j]->getStand_l();

							if (target_i == _allOprt[i] && target_j == o_j && target_k == s_k && target_l == s_l)
							{
								_SDRSoprt_set[j]->setCovered();

								betterSet->pushOprt_i(_allOprt[i]);
								_allOprt[i]->pushBetterSDRSset(betterSet);

								betterSet->pushSDRoprt(_SDRSoprt_set[j]);

								break;
							}
								
						}
					}
				}
			}


			//fullfill o_js
			for (int k = 0; k < _allOprt.size(); k++)
			{
				if (_allOprt[k] != o_j && !betterSet->check_in_is(_allOprt[k]))
				{
					vector<Operation *> o_js = betterSet->getOprt_js();
					//must overlap all j
					bool overlapall = true;
					for (auto & oj : o_js)
					{
						if (Util::oprt_overlap_Matrix[oj->getOprtID()][_allOprt[k]->getOprtID()] >= 0)
						{
							overlapall = false;
						}
					}

					if (overlapall)
					{
						// must SDRS for any is
						vector<Operation *> o_is = betterSet->getOprt_is();
						bool sdrs_all = true;
						for (auto oi : o_is)
						{
							bool this_oi_can_cover = false;
							for (int m = 0; m < _SDRSoprt_set.size(); m++)							
							{
								//reduce cpu time
								if (!_SDRSoprt_set[m]->getCoverStatus())
								{
									Operation * target_i = _SDRSoprt_set[m]->getOprt_i();
									Operation * target_j = _SDRSoprt_set[m]->getOprt_j();

									Stand * target_k = _SDRSoprt_set[m]->getStand_k();
									Stand * target_l = _SDRSoprt_set[m]->getStand_l();

									if (target_i == oi && target_j == _allOprt[k] && target_k == s_k && target_l == s_l)
									{
										this_oi_can_cover = true;
										break;
									}
								}

							}

							if (!this_oi_can_cover)
							{
								sdrs_all = false;
								break;
							}
						}

						if (sdrs_all)
						{
							for (auto oi : o_is)
							{								
								for (int m = 0; m < _SDRSoprt_set.size(); m++)
								{
									//reduce cpu time
									if (!_SDRSoprt_set[m]->getCoverStatus())
									{
										Operation * target_i = _SDRSoprt_set[m]->getOprt_i();
										Operation * target_j = _SDRSoprt_set[m]->getOprt_j();

										Stand * target_k = _SDRSoprt_set[m]->getStand_k();
										Stand * target_l = _SDRSoprt_set[m]->getStand_l();

										if (target_i == oi && target_j == _allOprt[k] && target_k == s_k && target_l == s_l)
										{
											betterSet->pushSDRoprt(_SDRSoprt_set[m]);
											_SDRSoprt_set[m]->setCovered();
											break;
										}
									}

								}
								// after set cover, push once 
								betterSet->pushOprt_j(_allOprt[k]);

								_allOprt[k]->pushBetterSDRSset(betterSet);
							}
						}
					}
				}
			}

			//add to the global set
			_betterSDRSset.push_back(betterSet);
		}
			
	}
}


//EJOR P602
void IO::create_overlapSet_forBetterMIP()
{
	for (auto & oprt : _allOprt)
	{	
		// temporarily use int to express time
		int startTime = oprt->getStartT();
		for (int i = 0; i < _allOprt.size(); i++)
		{
			// ai<=t<di
			if (_allOprt[i] != oprt)
			{
				int ai = _allOprt[i]->getStartT();
				int di = _allOprt[i]->getEndT();
				if (ai <= startTime && startTime < di)
				{
					oprt->pushOverlap_oprt(_allOprt[i]);
					// record overlapRngID
					_allOprt[i]->pushOai(oprt);
				}
			}
		}
	}


	//print

	cout << " totally generate " << _allOprt.size() << " operations " << endl;
	//check code
	cout << endl << "OprtID" << "\t" << "nextOprtID" << "\t" << "StartTime" << "\t" "EndTime" << "\t" << "TYPE" << "\t" << "compaStandSize" << "\t" << "stand_0_pre" "\t" << "stand_1_pre" "\t" << "stand_2_pre" "\t" <<" Oai_set_size"<<"\t"<<"OaiSetList"<< endl;
	for (auto & oprt : _allOprt)
	{
		string nextID;
		if (oprt->getNextOprt() != NULL)
		{
			nextID = std::to_string(oprt->getNextOprt()->getOprtID());
		}
		else
		{
			nextID = "-";
		}

		cout << oprt->getOprtID() << "\t" << nextID << "\t" << oprt->getStartT() << "\t" << oprt->getEndT() << "\t" << oprt->getOprttype() << "\t" << oprt->getCompa_stand().size() << "\t" << oprt->getOprtPreference()[0] << "\t" << oprt->getOprtPreference()[1] << "\t" << oprt->getOprtPreference()[2] << "\t" << oprt->getOverlapSet().size()<<"\t";
		if (!oprt->getOverlapSet().empty())
		{	
			for (auto & opt : oprt->getOverlapSet())
			{
				cout << opt->getOprtID() << ",";
			}			
		}
		else
		{
			cout << "None";
		}
		cout<< endl;
	}
}