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
	Direction chosenDirection;
	if (mAboutToFinish)
	{
		if (mMoves->size() > 0)
		{
			chosenDirection = mMoves->front();
			mMoves->pop_front();

			return oppositeDirection(chosenDirection);
		}
		else
		{
			return Direction::Stay;
		}

	}

	if (mLastDirection == -1)
	{
		mLastDirection = 0;
	}
	if (mPrevLastDirection == -1)
	{
		mPrevLastDirection = 0;
	} 
	// robot changed direction
	if (mLastDirection != mPrevLastDirection && mLastDirection != static_cast<int>(Direction::North))
	{
		chosenDirection = oppositeDirection(static_cast<Direction>(mPrevLastDirection));
		mPrevLastDirection = static_cast<int>(chosenDirection);
		mLastDirection = static_cast<int>(chosenDirection);
	}
	else{
		chosenDirection = static_cast<Direction>(mLastDirection);
	}
	while (info.isWall[static_cast<int>(chosenDirection)])
	{
		//robot is stepping into a wall - choose a different direction!
		switch (chosenDirection){
		case Direction::East:
			chosenDirection = Direction::South;
			if (info.isWall[static_cast<int>(chosenDirection)]){
				chosenDirection = Direction::North;
			}
			break;
		case Direction::West:
 			chosenDirection = Direction::South;
			if (info.isWall[static_cast<int>(chosenDirection)]){
				chosenDirection = Direction::North;
			}
			break;
		case Direction::South:
			chosenDirection = Direction::East;
			if (info.isWall[static_cast<int>(chosenDirection)]){
				chosenDirection = Direction::West;
			}
			break;
		case Direction::North:
			chosenDirection = Direction::East;
			if (info.isWall[static_cast<int>(chosenDirection)]){
				chosenDirection = Direction::West;
			}
			break;
		case Direction::Stay:
			chosenDirection = Direction::East;
			break;
		}
	}
	/*In some cases, the robot (the simulator in our case) decides not to step according to the
	direction provided by the algorithm. Thus, for each step, the robot (the simulator in our case)
	provides the algorithm the previous step’s direction that was actually taken*/
	mMoves->push_front(prevStep); // we only save the step which actually happened!
	mPrevLastDirection = mLastDirection;
	mLastDirection = (int)chosenDirection;

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