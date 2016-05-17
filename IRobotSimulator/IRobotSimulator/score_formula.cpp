#include "score_formula.h"


score_formula::score_formula()
{
}


score_formula::~score_formula()
{
}

#ifndef _WIN32

/*The function calculates score according to score_params and returns the calculated score.
In case score cannot be calculated for some reason, the function shall return 1.
If this happens the simulation shall ignore it and continue, but add a last line of error, after the
results table and after all other errors, saying:
	Score formula could not calculate some scores, see -1 in the results table
		*/
extern "C" int calc_score(const map<string, int>& score_params)
{
	int winnerNumberOfSteps;
	int simulationStepsCounter;
	int positionInCompetition;
	int actualPositionInCompetition;
	int stepsCounter;
	int initialDustSumInHouse;
	int dirtCollected;
	bool isBackInDocking;
	try
	{
		// this is not a member function of simulation class!!! 
		winnerNumberOfSteps = score_params.at("winner_num_steps");
		simulationStepsCounter = score_params.at("simulation_steps");
		actualPositionInCompetition = score_params.at("actual_position_in_competition");
		stepsCounter = score_params.at("this_num_steps");
		initialDustSumInHouse = score_params.at("sum_dirt_in_house");
		dirtCollected = score_params.at("dirt_collected");
		isBackInDocking = (score_params.at("is_back_in_docking") != 0);
	}
	catch(out_of_range & e)
	{
		if (SCORE_DEBUG)
		{
			cout <<"calculateSimulationScore couldn't find one of the parameters of the score_params "<< e.what() << endl;
		}
		return -1;
	}
	if (initialDustSumInHouse == dirtCollected  && isBackInDocking)
		{
			positionInCompetition = actualPositionInCompetition < 4 ? actualPositionInCompetition : 4;
			
		}
	else
	{
		positionInCompetition = 10;
	}

	int score = 2000;
	score -= (positionInCompetition - 1) * 50;
	score += (winnerNumberOfSteps - stepsCounter) * 10;
	score -= (initialDustSumInHouse - dirtCollected) * 3;
	score += (isBackInDocking ? 50 : -200);
	return score < 0 ? 0 : score;
}
#endif