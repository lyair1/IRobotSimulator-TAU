/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "200945657_A_.h"

// setSensor is called once when the Algorithm is initialized 
void _200945657_A::setSensor(const AbstractSensor& sensor)
{
	mSensor = &sensor;
}
    
// setConfiguration is called once when the Algorithm is initialized - see below 
void _200945657_A::setConfiguration(map<string, int> config)
{
	mConfiguration = &config;
}
    
// step is called by the simulation for each time unit 
Direction _200945657_A::step(){
	SensorInformation info = mSensor->sense();
	//if there's still dust - stay in current location:
	Direction chosenDirection = Direction::Stay;
	if (info.dirtLevel >= 1 && info.dirtLevel <= 9)
	{
		return chosenDirection;
	}
	//if there's no dust - move from current location:
	else if (info.dirtLevel == 0)
	{
		chosenDirection = (Direction) (rand() % 4);
		while (info.isWall[(int)chosenDirection])
		{
			//robot is stepping into a wall - choose a different direction!
			chosenDirection = (Direction) (rand() % 4);
		}
	}
	return chosenDirection;
}
    
// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void _200945657_A::aboutToFinish(int stepsTillFinishing){

	// for ex1 this function is left empty since the algorithm is naive.
} 

void _200945657_A::cleanResources(){

}

#ifndef _WIN32
extern "C" AbstractAlgorithm* getAbstractAlgorithm()
{
	return new AlgorithmNaive();
}
#endif