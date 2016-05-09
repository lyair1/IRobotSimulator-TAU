/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef ___200945657_B_
#define ___200945657_B_

#include "AlgorithmBase.h"

using namespace std;

/*	The purpose of the algorithm:
Clean the maximum amount of dust during a limited amount of time
Using the sensor, the algorithm for the robotic cleaner should guide it through the house and the cleaning process.
It should decide to which direction to advance, it may also decide to stay several time units on the same spot if the amount of dust at that spot is high.
So, the possible steps for the algorithm to decide upon are:
1.	Stay on the same spot
(e.g., a spot which is very dirty may require several steps until it is completely cleaned).
2.	Advance to one square to one of the directions:
East, West, North, or South (only one of the directions may be chosen in a single step).
*/

class _200945657_B : public AlgorithmBase
{
public:
	_200945657_B()
	{ 
	}
	~_200945657_B()
	{
		// base class will call clenaResources()
	}

	// this method is called by the simulation either when there is a winner or 
	// when steps == MaxSteps - MaxStepsAfterWinner 
	// parameter stepsTillFinishing == MaxStepsAfterWinner 
	void aboutToFinish(int stepsTillFinishing) override;

	void cleanResources();

	Direction getNextStep(SensorInformation info, Direction prevStep);
private:
};

#endif //ALGO_NAIVE_H