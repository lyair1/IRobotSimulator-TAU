/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "200945657_A_.h"

// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void _200945657_A::aboutToFinish(int stepsTillFinishing){
	mAboutToFinish = true;
}

void _200945657_A::cleanResources(){

}

Direction _200945657_A::getNextStep(SensorInformation info, Direction prevStep)
{
	Direction chosenDirection = Direction::North;
	addInfoFromSensor();

	if (_ALGORITHM_DEBUG_)
	{
		createHouseMatrix();
		printHouseMatrix();
	}

	return chosenDirection;
}

#ifndef _WIN32
extern "C" AbstractAlgorithm* getAbstractAlgorithm()
{
	return new _200945657_A();
}
#endif