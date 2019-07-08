#include"Model.h"
using namespace std;

int main() 
{	
	IO * io = new IO();

	// PUDONG DATA
	//io->Input_integrated_data();

	//case
	io->InputCsee_stand();

	io->InputCase_Arr();
	io->InputCase_Dep();
	io->CreateCase_Parking();
	
	io->generate_act_OverlapMatrix();
	io->generateAllNodes();
	io->generateAllArcs();

	Algorithm * alg = new Algorithm(io);
	alg->Ejection_chain();

	//model
	cout << endl << "-----------------------------------Here is the approach of exact model---------------------------------" << endl << endl;

	io->generateAllOprt_by_dupAct();
	io->generate_oprt_OverlapMatrix_by_dupAct();
	io->create_overlapSet_forBetterMIP();

	Model * md = new Model(io);

	md->initialize_btMIP_Model();
	md->solveMIP_betterModel();
	md->end_Model();

	return 0;
}
