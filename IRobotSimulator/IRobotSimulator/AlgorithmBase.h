#ifndef __ALGORITHM_BASE__H_
#define __ALGORITHM_BASE__H_

#include <vector>
#include <map>
#include <limits.h>
using namespace std;

#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "House.h"
#include "Sensor.h"

class AlgorithmBase : public AbstractAlgorithm
{

public:
	AlgorithmBase():mAboutToFinish(false),
					mLastDirection(-1),
					mSensor(NULL),
					mConfiguration(NULL)
	{
		mMoves = new list<Direction>();
	}
	~AlgorithmBase()
	{
		cleanResources();
	}
	// setSensor is called once when the Algorithm is initialized 
	void setSensor(const AbstractSensor& sensor) override;

	// setConfiguration is called once when the Algorithm is initialized - see below 
	void setConfiguration(map<string, int> config) override;

	// step is called by the simulation for each time unit 
	Direction step() override;

	// getNextStep is being called by step() after performing some default checks
	virtual Direction getNextStep(SensorInformation info) = 0;

	// this method is called by the simulation either when there is a winner or 
	// when steps == MaxSteps - MaxStepsAfterWinner 
	// parameter stepsTillFinishing == MaxStepsAfterWinner 
	void aboutToFinish(int stepsTillFinishing) override;

	void cleanResources();

	Direction oppositeDirection(Direction direction_);
protected:
	const AbstractSensor* mSensor; // the only info the algorithm has about the house it's cleaning comes from the sensor itself.
	//House * house; //algorithm does not have any given information about the house, but it can gather that information along the way
	std::pair <int, int> mLocation;
	list<Direction> *mMoves;
	int mLastDirection;
	map<string, int>* mConfiguration;
	bool mAboutToFinish;
};

#endif