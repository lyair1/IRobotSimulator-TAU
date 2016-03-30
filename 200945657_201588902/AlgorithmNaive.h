/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef ALGO_NAIVE_H
#define ALGO_NAIVE_H
#include "AbstractAlgorithm.h"
#include "House.h"
#include "Sensor.h"
using namespace std;


/*	The purpose of the algorithm:
	Clean the maximum amount of dust during a limited amount of time
	Using the sensor, the algorithm for the robotic cleaner should guide it through the house and the cleaning process.
	It should decide to which direction to advance, it may also decide to stay several time units on the same spot if the amount of dust at that spot is high.
	So, the possible steps for the algorithm to decide upon are:
		1.	Stay on the same spot
			(e.g., a spot which is very dirty may require several steps until it is completely cleaned).
		2.	Advance to one square to one of the directions:
			East, West, North, or South (only one of the directions may be chosen in a single step).
*/

class AlgorithmNaive : public AbstractAlgorithm
{
public:
	AlgorithmNaive(){}
	~AlgorithmNaive()
	{
		cleanResources();
	}
    // setSensor is called once when the Algorithm is initialized 
    void setSensor(const AbstractSensor& sensor) ; 
    
    // setConfiguration is called once when the Algorithm is initialized - see below 
    void setConfiguration(map<string, int> config); 
    
    // step is called by the simulation for each time unit 
    Direction step(); 
    
    // this method is called by the simulation either when there is a winner or 
    // when steps == MaxSteps - MaxStepsAfterWinner 
    // parameter stepsTillFinishing == MaxStepsAfterWinner 
    void aboutToFinish(int stepsTillFinishing); 

	void cleanResources();
private:
	const AbstractSensor* mSensor; // the only info the algorithm has about the house it's cleaning comes from the sensor itself.
	//House * house; //algorithm does not have any given information about the house, but it can gather that information along the way
	std:: pair <int, int> mLocation;
	map<string, int>* mConfiguration;
};

#endif //ALGO_NAIVE_H