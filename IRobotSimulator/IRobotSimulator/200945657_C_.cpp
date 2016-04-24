/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "200945657_C_.h"

// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void _200945657_C::aboutToFinish(int stepsTillFinishing){
	_aboutToFinish = true;
}

void _200945657_C::cleanResources(){

}

Direction _200945657_C::getNextStep(SensorInformation info)
{
	Direction chosenDirection;
	if (_aboutToFinish)
	{
		chosenDirection = moves->front();
		moves->pop_front();

		return oppositeDirection(chosenDirection);
	}
	if (lastDirection == -1)
	{
		lastDirection = 2;
	}

	int moveDirection = lastDirection;
	chosenDirection = static_cast<Direction>((moveDirection*moves->size() * 5) % 4);
	while (info.isWall[static_cast<int>(chosenDirection)])
	{
		//robot is stepping into a wall - choose a different direction!
		moveDirection++;
		chosenDirection = static_cast<Direction>(moveDirection % 4);
	}
	moves->push_front(chosenDirection);

	return chosenDirection;
}

#ifndef _WIN32
extern "C" AbstractAlgorithm* getAbstractAlgorithm()
{
	return new _200945657_C();
}
#endif