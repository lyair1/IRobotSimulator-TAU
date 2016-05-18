/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "200945657_A_.h"
#include "Path.h"

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

	// If on docking
	if (mLocation == mDockingLocation)
	{
		if (isHouseClean())
		{
			chosenDirection = Direction::Stay;
		}
		else if (isHouseMapped())
		{
			// TODO: check if has enough battery to finish:

		}
		else if (mBatteryLeft < mConfiguration.BatteryCapacity)
		{
			chosenDirection = Direction::Stay;
		}
		else
		{
			chosenDirection = getStepFromDocking();
		}
	}
	else
	{
		// In Position Somewhere
		Path path = getShortestPathToDocking(mLocation);
		if (info.dirtLevel > 0 && mBatteryLeft > path.length)
		{
			// If on dirt
			chosenDirection = Direction::Stay;
		}else if (isHouseClean())
		{
			// If all is clean
			chosenDirection = getDirectionFromPoint(mLocation, path.path[1]);
		}
		else if (mNotWallSet.size() > 0)
		{
			// Looking for the not wall
			Path pathToNotWall = findClosestNotWall();
			chosenDirection = getDirectionFromPoint(mLocation, pathToNotWall.path[1]);
		}
	}

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