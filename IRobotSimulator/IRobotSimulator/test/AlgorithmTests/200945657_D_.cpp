/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "200945657_D_.h"
#include "Path.h"
#include "AlgorithmRegistration.h"
#ifndef _WIN32
#include "MakeUnique.cpp"
#endif

// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void _200945657_D::aboutToFinish(int stepsTillFinishing){
	mAboutToFinish = true;
	if (mBatteryLeft > stepsTillFinishing)
	{
		mBatteryLeft = stepsTillFinishing;
	}
}

void _200945657_D::cleanResources(){

}

Direction _200945657_D::getNextStep(SensorInformation info, Direction prevStep)
{
	//HIT THE WALL ALGORITHM
	return Direction::East;
}


REGISTER_ALGORITHM(_200945657_D)
