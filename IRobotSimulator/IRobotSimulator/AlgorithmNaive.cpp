/*
Author: Nir Orman ID 201588902
Yair Levi ID 200945657
*/
#include "AlgorithmNaive.h"

// setSensor is called once when the Algorithm is initialized 
void AlgorithmNaive::setSensor(const AbstractSensor& sensor)
{
	mSensor = &sensor;
}
    
// setConfiguration is called once when the Algorithm is initialized - see below 
void AlgorithmNaive::setConfiguration(map<string, int> config)
{
	mConfiguration = config;
}
    
// step is called by the simulation for each time unit 
Direction AlgorithmNaive:: step(){
	SensorInformation info = mSensor->sense();
	int randomDirection = rand() % 5 ;
	while (randomDirection != 4 && info.isWall[randomDirection])
	{
		//robot is stepping into a wall - choose a different direction!
		randomDirection = rand() % 5 ;
	}
	Direction dir = (Direction)randomDirection;
	return dir;
}
    
// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void AlgorithmNaive:: aboutToFinish(int stepsTillFinishing){

	// for ex1 this function is left empty since the algorithm is naive.
} 

void AlgorithmNaive::cleanResources(){

}