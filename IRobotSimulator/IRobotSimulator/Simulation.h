#ifndef SIMULATION_H
#define SIMULATION_H
#include "../include/House.h"
#include "../include/AlgorithmNaive.h"

class Simulation
{
public:
	Simulation(AlgorithmNaive* algorithm, House* house, const map<string, int> parametersMap)
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
		if (parametersMap.find("BatteryConsumptionRate") != parametersMap.end())
		{
			mBattreyConsumptionRate = parametersMap.find("BatteryConsumptionRate")->second;
		}
		else{
			cout << "Simulation constructor could not find BattreyConsumptionRate in parameters from configuration file." << endl;
		}
		mMaxSteps = 0;
		if (parametersMap.find("MaxSteps") != parametersMap.end())
		{
			mMaxSteps = parametersMap.find("MaxSteps")->second;
		}
		else{
			cout << "Simulation constructor could not find maxSteps in parameters from configuration file." << endl;
		}
		mBatteryRechargeRate = 0;
		if (parametersMap.find("BatteryRechargeRate") != parametersMap.end()) // grammer mistake is on purpose! this is how it was written in the exercise file.
		{
			mBatteryRechargeRate = parametersMap.find("BatteryRechargeRate")->second;
		}
		else{
			cout << "Simulation constructor could not find BatteryRechargeRate in parameters from configuration file." << endl;
		}
		mBatteryCapacity = 0;
		mBatteryLeft = 0;
		if (parametersMap.find("BatteryCapacity") != parametersMap.end())
		{
			mBatteryCapacity = parametersMap.find("BatteryCapacity")->second;
			mBatteryLeft = mBatteryCapacity;
		}
		else{
			cout << "Simulation constructor could not find BatteryCapacity in parameters from configuration file." << endl;
		}
		mMaxStepsAfterWinner = 0;
		if (parametersMap.find("MaxStepsAfterWinner") != parametersMap.end())
		{
			mBatteryCapacity = parametersMap.find("MaxStepsAfterWinner")->second;
		}
		else{
			cout << "Simulation constructor could not find MaxStepsAfterWinner in parameters from configuration file." << endl;
		}

		mPositionInCompetition = -1;
	}

	bool isSimulationRunning();
	bool makeSimulationStep();
	int getNumberOfSteps();
	const AbstractSensor * getSensor();
	void setPositionInCompetition(int position);
	void resetMaxStepsAccordingToWinner();
	void setSimulationScore(int winnerNumberOfSteps);
	const House* getHouse();
	const int getSimulationScore();

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
	map<string, int> mConfiguration;
	int mPositionInCompetition;
	bool mIsBackInDocking;
	bool mCrashedIntoWall;
};
#endif // SIMULATION_H