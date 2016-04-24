/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "200945657_B_.h"

// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void _200945657_B::aboutToFinish(int stepsTillFinishing){
	_aboutToFinish = true;
}

void _200945657_B::cleanResources(){

}

Direction _200945657_B::getNextStep(SensorInformation info)
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
		lastDirection = 1;
	}

	int moveDirection = (int)lastDirection;
	chosenDirection = static_cast<Direction>((moveDirection*moves->size() * 3) % 4);
	while (info.isWall[static_cast<int>(chosenDirection)])
	{
		//robot is stepping into a wall - choose a different direction!
		moveDirection++;
		chosenDirection = static_cast<Direction>(moveDirection%4);
	}
	moves->push_front(chosenDirection);

	return chosenDirection;
}

#ifndef _WIN32
extern "C" AbstractAlgorithm* getAbstractAlgorithm()
{
	return new _200945657_B();
}
#endif