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
	cout << "Starting get next step!" << endl;
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
			//chosenDirection = getStepFromDocking();

			// Looking for the not wall
 			cout << "Looking for not wall! docking" << endl;
			Path pathToNotWall = findClosestNotWall();
			chosenDirection = getDirectionFromPoint(mLocation, pathToNotWall.path[1]);
		}
	}
	else
	{
		// In Position Somewhere
		Path path = getShortestPathToDocking(mLocation);
		if (info.dirtLevel > 0 && mBatteryLeft > path.length + 1)
		{
			// If on dirt
			cout << "On Dirt!" << endl;
			chosenDirection = Direction::Stay;
		}
		else if(mBatteryLeft <= path.length + 1)
		{
			cout << "Going back to charge! Battery: " << mBatteryLeft << endl;
			chosenDirection = getDirectionFromPoint(mLocation, path.path[1]);
		}
		else if (isHouseClean())
		{
			// If all is clean
			cout << "All is clean!" << endl;
			chosenDirection = getDirectionFromPoint(mLocation, path.path[1]);
		}
		else if (mNotWallSet.size() >= 0)
		{
			if (mNotWallSet.size() == 0) // in case there is no more not walls (and also dirt) go back to docking
			{
				addNotWallToMatrix(mDockingLocation);
			}
			// Looking for the not wall
			cout << "Looking for not wall!" << endl;
			Path pathToNotWall = findClosestNotWall();
			chosenDirection = getDirectionFromPoint(mLocation, pathToNotWall.path[1]);
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