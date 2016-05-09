/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "AlgorithmBase.h"

// setSensor is called once when the Algorithm is initialized 
void AlgorithmBase::setSensor(const AbstractSensor& sensor)
{
	mLastDirection = -1;
	mPrevLastDirection = -1;
	delete mMoves;
	mMoves = new list <Direction>();
	mSensor = &sensor;
}

// setConfiguration is called once when the Algorithm is initialized - see below 
void AlgorithmBase::setConfiguration(map<string, int> config)
{
	mConfiguration = &config;
}

// step is called by the simulation for each time unit 

/*	the algorithm MUST take into account the actual
	steps that were taken, provided as a parameter to 'step', not relying on the recommended
	steps returned from the calls to 'step' to be the actual steps taken*/
Direction AlgorithmBase::step(Direction prevStep){
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
		chosenDirection = getNextStep(info, prevStep);
	}
	return chosenDirection;
}

// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void AlgorithmBase::aboutToFinish(int stepsTillFinishing){
	mAboutToFinish = true;
}

void AlgorithmBase::cleanResources(){
	delete mMoves;
}

Direction AlgorithmBase::oppositeDirection(Direction direction_)
{
	switch (direction_)
	{
	case Direction::East: return Direction::West;
	case Direction::West: return Direction::East;
	case Direction::North: return Direction::South;
	case Direction::South: return Direction::North;
	case Direction::Stay: return Direction::Stay;
	}
	return Direction::Stay;
}

// Default implementation is a random choose between all of the non-wall options
Direction AlgorithmBase::getNextStep(SensorInformation info, Direction prevStep)
{
	Direction chosenDirection;
	if (mAboutToFinish)
	{
		chosenDirection = mMoves->front();
		mMoves->pop_front();

		return chosenDirection;
	}

	chosenDirection = (Direction)(rand() % 4);
	while (info.isWall[(int)chosenDirection])
	{
		//robot is stepping into a wall - choose a different direction!
		chosenDirection = (Direction)(rand() % 4);
	}
	mMoves->push_front(prevStep);

	return chosenDirection;
}
