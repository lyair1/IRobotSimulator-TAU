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
	Simulation(AlgorithmNaive* algorithm, House* house, map<string, int>* parametersMap)
	{
		mAlgorithm = algorithm;
		mHouse = house;
		mSensor = new Sensor(mHouse->getDockingLocation(), mHouse);
		mIsRunning = true; // TODO: make sure the robot is in a legal location in the house before setting mIsRunning to true
		mStepsCounter = 0;
		mScore = 0;
		mDirtCollected = 0;
		mConfiguration = parametersMap;
		mBattreyConsumptionRate = 0;
		mCrashedIntoWall = false;
		mInitialDustSumInHouse = mHouse->getDustInHouse();
		mIsBackInDocking = mHouse->isCleanHouse() ?true : false; // we start simulation at location 'D'. if the house is clean - we're considered back in docking
		if (mConfiguration->find("BatteryConsumptionRate") != mConfiguration->end())
		{
			mBattreyConsumptionRate = mConfiguration->find("BatteryConsumptionRate")->second;
		}
		else{
			cout << "Simulation constructor could not find BattreyConsumptionRate in parameters from configuration file." << endl;
		}
		mMaxSteps = 0;
		if (mConfiguration->find("MaxSteps") != mConfiguration->end())
		{
			mMaxSteps = mConfiguration->find("MaxSteps")->second;
		}
		else{
			cout << "Simulation constructor could not find maxSteps in parameters from configuration file." << endl;
		}
		mBatteryRechargeRate = 0;
		if (mConfiguration->find("BatteryRechargeRate") != mConfiguration->end()) // grammer mistake is on purpose! this is how it was written in the exercise file.
		{
			mBatteryRechargeRate = mConfiguration->find("BatteryRechargeRate")->second;
		}
		else{
			cout << "Simulation constructor could not find BatteryRechargeRate in parameters from configuration file." << endl;
		}
		mBatteryCapacity = 0;
		mBatteryLeft = 0;
		if (mConfiguration->find("BatteryCapacity") != mConfiguration->end())
		{
			mBatteryCapacity = mConfiguration->find("BatteryCapacity")->second;
			mBatteryLeft = mBatteryCapacity;
		}
		else{
			cout << "Simulation constructor could not find BatteryCapacity in parameters from configuration file." << endl;
		}
		mMaxStepsAfterWinner = 0;
		if (mConfiguration->find("MaxStepsAfterWinner") != mConfiguration->end())
		{
			mBatteryCapacity = mConfiguration->find("MaxStepsAfterWinner")->second;
		}
		else{
			cout << "Simulation constructor could not find MaxStepsAfterWinner in parameters from configuration file." << endl;
		}

		mPositionInCompetition = -1;
		mIsOutOfBattery = (mBatteryLeft <= 0)? true: false;
	}

	bool isSimulationRunning();
	bool makeSimulationStep();
	int getNumberOfSteps();
	const AbstractSensor * getSensor();
	int Simulation::getPositionInCompetition();
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