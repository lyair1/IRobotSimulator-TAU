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
	bool crashedIntoWall;
	int positionInCompetition;
	bool isOutOfBattery;
	int stepsCounter;
	int initialDustSumInHouse;
	int dirtCollected;
	bool isBackInDocking;
	try
	{
		// this is not a member function of simulation class!!! 
		winnerNumberOfSteps = score_params.at("winnerNumberOfSteps");
		simulationStepsCounter = score_params.at("simulationStepsCounter");
		crashedIntoWall = (score_params.at("crashedIntoWall") != 0);
		positionInCompetition = score_params.at("positionInCompetition");
		isOutOfBattery = (score_params.at("isOutOfBattery") != 0);
		stepsCounter = score_params.at("stepsCounter");
		initialDustSumInHouse = score_params.at("initialDustSumInHouse");
		dirtCollected = score_params.at("dirtCollected");
		isBackInDocking = (score_params.at("isBackInDocking") != 0);
	}
	catch(out_of_range & e)
	{
		if (DEBUG)
		{
			cout <<"calculateSimulationScore couldn't find one of the parameters of the score_params "<< e.what() << endl;
		}
		return -1;
	}

	if (crashedIntoWall){
		return 0;
	}

	int score = 2000;
	score -= (positionInCompetition - 1) * 50;
	if (isOutOfBattery)
	{
		score += (winnerNumberOfSteps - simulationStepsCounter) * 10;
	}
	else
	{
		score += (winnerNumberOfSteps - stepsCounter) * 10;
	}

	score -= (initialDustSumInHouse - dirtCollected) * 3;
	score += (isBackInDocking ? 50 : -200);
	return score < 0 ? 0 : score;
}
#endif