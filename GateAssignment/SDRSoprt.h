#pragma once
#include"ShadowRS.h"


class SDRSoprt
{
private:
	Operation * oprt_i;

	Operation * oprt_j;

	ShadowRS * belongSDRS;

	bool haveBeenCovered;

public:
	SDRSoprt(Operation * oi,Operation*oj,ShadowRS* sdrs) { oprt_i = oi, oprt_j = oj, belongSDRS = sdrs, haveBeenCovered = false; }

	Operation * getOprt_i() { return oprt_i; }
	Operation * getOprt_j() { return oprt_j; }

	Stand * getStand_k() { return belongSDRS->getStand_k(); }
	Stand * getStand_l() { return belongSDRS->getStand_l(); }

	void setCovered() { haveBeenCovered = true; }
	bool getCoverStatus() { return haveBeenCovered; }
};