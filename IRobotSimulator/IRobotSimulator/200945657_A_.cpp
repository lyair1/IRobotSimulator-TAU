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
	_aboutToFinish = true;
}

void _200945657_A::cleanResources(){

}

Direction _200945657_A::getNextStep(SensorInformation info)
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
		lastDirection = 0;
	}

	int moveDirection = lastDirection;
	chosenDirection = static_cast<Direction>((moveDirection*moves->size()*2) % 4);
	while (info.isWall[static_cast<int>(chosenDirection)])
	{
		//robot is stepping into a wall - choose a different direction!
		moveDirection++;
		chosenDirection = (Direction)(moveDirection % 4);
	}
	moves->push_front(chosenDirection);

	lastDirection = (int)chosenDirection;
	return chosenDirection;
}

#ifndef _WIN32
extern "C" AbstractAlgorithm* getAbstractAlgorithm()
{
	return new _200945657_A();
}
#endif