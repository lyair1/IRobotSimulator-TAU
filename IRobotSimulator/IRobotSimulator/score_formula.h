#pragma once

using namespace std;
#include <iostream>
#include <map>
#define SCORE_DEBUG 0

class score_formula
{
public:
	score_formula();
	~score_formula();
	
};

extern "C" int calc_score(const map<string, int>& score_params);