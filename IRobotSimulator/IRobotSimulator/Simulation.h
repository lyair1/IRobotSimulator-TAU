/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef SIMULATION_H
#define SIMULATION_H
#include "House.h"
#include "configReader.h"
#include "AbstractAlgorithm.h"
#include "Sensor.h"
#include <typeinfo>


class Simulation
{
public:
	Simulation(AbstractAlgorithm* algorithm, House* house, map<string, int>* parametersMap):
		mStepsCounter(0),
		mScore(0),
		mDirtCollected(0),
		mBattreyConsumptionRate(0),
		mCrashedIntoWall(false),
		mPositionInCompetition(-1),
		mIsRunning(true), // TODO: make sure the robot is in a legal location in the house before setting mIsRunning to true
		mIsAboutToFinish(false)
	{
		mAlgorithm = algorithm;
		mHouse = house;
		mSensor = new Sensor(mHouse->getDockingLocation(), mHouse);
		mConfiguration = parametersMap;		
		mInitialDustSumInHouse = mHouse->getDustInHouse();
		mIsBackInDocking = mHouse->isCleanHouse() ?true : false; // we start simulation at location 'D'. if the house is clean - we're considered back in docking
		//all parameters are located in the configuration file, otherwise it's an Illegal file and program would have exit at ConfigReader.
		mBattreyConsumptionRate = mConfiguration->find(BATTERY_CONSUMPTION_RATE)->second;
		mMaxSteps = mHouse->getMaxSteps();
		mBatteryRechargeRate = mConfiguration->find(BATTERY_RECHARGE_RATE)->second;
		mBatteryCapacity = mConfiguration->find(BATTERY_CAPACITY)->second;
		mBatteryLeft = mBatteryCapacity;
		mMaxStepsAfterWinner = mConfiguration->find(MAX_STEPS_AFTER_WINNER)->second;
		mIsOutOfBattery = (mBatteryLeft <= 0)? true: false;
	}

	bool isSimulationRunning() const;
	bool makeSimulationStep();
	int getNumberOfSteps() const;
	const AbstractSensor * getSensor() const;
	int getPositionInCompetition() const;
	void setPositionInCompetition(int actualPositionInCompetition);
	void resetMaxStepsAccordingToWinner();
	const House* getHouse() const;
	const int getSimulationScore() const;
	void setSimulationScore(int score) ;
	void cleanResources();
	void printSimulationStepsHistory();

	
	//inline functions:
	bool getCrashedIntoWall() const{ return mCrashedIntoWall; } 
	bool getIsOutOfBattery() const{ return mIsOutOfBattery; } 
	int getStepsCounter() const{ return mStepsCounter; } 
	int getInitialDustSumInHouse () const{ return mInitialDustSumInHouse; } 
	int getDirtCollected() const{ return mDirtCollected; } 
	bool getIsBackInDocking() const{ return mIsBackInDocking; } 

	
private:
	long mStepsCounter;
	int mScore;
	int mDirtCollected;
	int mBattreyConsumptionRate;
	bool mCrashedIntoWall;
	int mPositionInCompetition;
	bool mIsRunning;
	bool mIsAboutToFinish;
	AbstractAlgorithm* mAlgorithm;
	House* mHouse;
	Sensor* mSensor;
	map<string, int>* mConfiguration;
	int mInitialDustSumInHouse;
	bool mIsBackInDocking;
	int mMaxSteps;
	int mBatteryRechargeRate; 
	int mBatteryCapacity;
	int mBatteryLeft;
	int mMaxStepsAfterWinner;
	bool mIsOutOfBattery;
	list<Direction> mStepsHistory;
	
};
#endif // SIMULATION_H