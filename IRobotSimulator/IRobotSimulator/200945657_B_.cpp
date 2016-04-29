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
	mAboutToFinish = true;
}

void _200945657_B::cleanResources(){

}

Direction _200945657_B::getNextStep(SensorInformation info)
{
	Direction chosenDirection;
	if (mAboutToFinish)
	{
		chosenDirection = mMoves->front();
		mMoves->pop_front();

		return oppositeDirection(chosenDirection);
	}

	if (mLastDirection == -1)
	{
		mLastDirection = 1;
	}

	int moveDirection = mLastDirection;
	chosenDirection = static_cast<Direction>((moveDirection*mMoves->size() * 3) % 4);
	while (info.isWall[static_cast<int>(chosenDirection)])
	{
		//robot is stepping into a wall - choose a different direction!
		moveDirection++;
		chosenDirection = static_cast<Direction>(moveDirection%4);
	}
	mMoves->push_front(chosenDirection);

	mLastDirection = (int)chosenDirection;
	return chosenDirection;
}

#ifndef _WIN32
extern "C" AbstractAlgorithm* getAbstractAlgorithm()
{
	return new _200945657_B();
}
#endif