/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "200945657_A_.h"
#include "Path.h"
#include "AlgorithmRegistration.h"
#ifndef _WIN32
#include "MakeUnique.cpp"
#endif

// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void _200945657_A::aboutToFinish(int stepsTillFinishing){
	mAboutToFinish = true;
	if (mBatteryLeft > stepsTillFinishing)
	{
		mBatteryLeft = stepsTillFinishing;
	}
}

void _200945657_A::cleanResources(){

}

Direction _200945657_A::getNextStep(SensorInformation info, Direction prevStep)
{
	Direction chosenDirection = Direction::North;
	addInfoFromSensor();
	debugPrint("Starting get next step! battery level: " + mBatteryLeft);
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
			debugPrint("Charging battery");
			chosenDirection = Direction::Stay;
		}
		else
		{
			//chosenDirection = getStepFromDocking();

			// Looking for the not wall
			debugPrint("Looking for not wall! docking");
			Point p = mLocation;
			if (isNotWall(getPointFromDirection(p, Direction::South))) // OPtimization
			{
				chosenDirection = Direction::South;
			}
			else
			{
				Path pathToNotWall = findClosestNotWall(true, true);
				chosenDirection = getDirectionFromPoint(mLocation, pathToNotWall.path[1]);
			}
		}
	}
	else
	{
		// In Position Somewhere
		Path path = getShortestPathToDocking(mLocation);

		if (path.length < 3 && mBatteryLeft < mConfiguration.BatteryCapacity/2)
		{
			chosenDirection = getDirectionFromPoint(mLocation, path.path[1]);
		}
		else if (info.dirtLevel > 0 && mBatteryLeft >(int)(path.length + 1 + (path.length*fakeStatistics / 100)))
		{
			// If on dirt
			debugPrint("On Dirt!");
			chosenDirection = Direction::Stay;
		}
		else if(mBatteryLeft <= (int)(path.length + 1 + (path.length*fakeStatistics/100)))
		{
			debugPrint("Going back to charge! Battery: " + mBatteryLeft);
			chosenDirection = getDirectionFromPoint(mLocation, path.path[1]);
		}
		else if (isHouseClean())
		{
			// If all is clean
			debugPrint("All is clean!");
			chosenDirection = getDirectionFromPoint(mLocation, path.path[1]);
		}
		else if (mNotWallSet.size() >= 0)
		{
			if (mNotWallSet.size() == 0) // in case there is no more not walls (and also dirt) go back to docking
			{
				addNotWallToMatrix(mDockingLocation);
			}
			// Looking for the not wall
			debugPrint("Looking for not wall!");
			Path pathToNotWall = findClosestNotWall(true, true);
		
			if (pathToNotWall.length + getShortestPathToDocking(pathToNotWall.dest).length > (size_t)mBatteryLeft)
			{
				chosenDirection = getDirectionFromPoint(mLocation, path.path[1]);
			}else
			{
				chosenDirection = getDirectionFromPoint(mLocation, pathToNotWall.path[1]);
			}			
		}
	}

	return chosenDirection; // Make sure we don't get here, we can hit the wall
}

#ifndef _WIN32
extern "C" AbstractAlgorithm* getAbstractAlgorithm()
{
	return new _200945657_A();
}
#endif

#ifndef _WIN32
REGISTER_ALGORITHM(_200945657_A)
#endif