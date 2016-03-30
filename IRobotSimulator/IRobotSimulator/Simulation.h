/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef SIMULATION_H
#define SIMULATION_H
#include "House.h"
#include "AlgorithmNaive.h"

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
		mIsRunning(true) // TODO: make sure the robot is in a legal location in the house before setting mIsRunning to true
	{
		mAlgorithm = algorithm;
		mHouse = house;
		mSensor = new Sensor(mHouse->getDockingLocation(), mHouse);
		mConfiguration = parametersMap;		
		mInitialDustSumInHouse = mHouse->getDustInHouse();
		mIsBackInDocking = mHouse->isCleanHouse() ?true : false; // we start simulation at location 'D'. if the house is clean - we're considered back in docking
		//all parameters are located in the configuration file, otherwise it's an Illegal file and program would have exit at ConfigReader.
		mBattreyConsumptionRate = mConfiguration->find("BatteryConsumptionRate")->second;
		mMaxSteps = mConfiguration->find("MaxSteps")->second;
		mBatteryRechargeRate = mConfiguration->find("BatteryRechargeRate")->second;
		mBatteryCapacity = mConfiguration->find("BatteryCapacity")->second;
		mBatteryLeft = mBatteryCapacity;
		mBatteryCapacity = mConfiguration->find("MaxStepsAfterWinner")->second;
		mIsOutOfBattery = (mBatteryLeft <= 0)? true: false;
	}

	bool isSimulationRunning();
	bool makeSimulationStep();
	int getNumberOfSteps();
	const AbstractSensor * getSensor();
	int getPositionInCompetition();
	void setPositionInCompetition(int actualPositionInCompetition);
	void resetMaxStepsAccordingToWinner();
	void setSimulationScore(int winnerNumberOfSteps, int simulationStepsCounter);
	const House* getHouse();
	const int getSimulationScore();
	void cleanResources();
	void printSimulationScore(string houseName, int score);
private:
	AlgorithmNaive* mAlgorithm;
	House* mHouse;
	Sensor* mSensor;
	bool mIsRunning;
	long mStepsCounter;
	int mScore;
	int mBatteryLeft;
	int mBattreyConsumptionRate;
	int mMaxSteps;
	int mBatteryRechargeRate; 
	int mBatteryCapacity;
	int mDirtCollected;
	int mMaxStepsAfterWinner;
	int mInitialDustSumInHouse;
	map<string, int>* mConfiguration;
	int mPositionInCompetition;
	bool mIsBackInDocking;
	bool mCrashedIntoWall;
	bool mIsOutOfBattery;
	
};
#endif // SIMULATION_H