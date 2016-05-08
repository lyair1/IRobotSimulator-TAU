#include "score_formula.h"


score_formula::score_formula()
{
}


score_formula::~score_formula()
{
}

#ifndef _WIN32



extern "C" int calc_score(const map<string, int>& score_params)
{
	cout << "calc_score" << endl;
	return 123456;
}
#endif