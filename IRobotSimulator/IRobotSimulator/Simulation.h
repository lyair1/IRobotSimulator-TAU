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
	Simulation(AbstractAlgorithm& algorithm, string algoName, House* house, map<string, int>* parametersMap, bool isVideo) :
		mStepsCounter(0),
		mScore(0),
		mDirtCollected(0),
		mBattreyConsumptionRate(0),
		mCrashedIntoWall(false),
		mPositionInCompetition(-1),
		mActualPositionInCompetition(-1),
		mIsRunning(true), // TODO: make sure the robot is in a legal location in the house before setting mIsRunning to true
		mIsAboutToFinish(false),
		mPrevSimulationStep(Direction::Stay),
		mAlgorithmName(algoName),
		mSimulationError(""),
		mIsVideo(isVideo)
	{
		mAlgorithm = &algorithm;
		mHouse = house;
		mSensor = make_unique<Sensor>(mHouse->getDockingLocation(), mHouse);
		mAlgorithm->setSensor(*mSensor);
		mAlgorithm->setConfiguration(*parametersMap);
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
	int getPositionInCompetition() const;
	int getActualPositionInCompetition() const;
	void setPositionInCompetition(int actualPositionInCompetition);
	void setActualPositionInCompetition(int actualPositionInCompetition);
	void setStepsCounter(int stepsCounter);
	void resetMaxStepsAccordingToWinner();
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
	string getSimulationErrors() const { return mSimulationError; }
	string getAlgorithmName() const{ return mAlgorithmName; }
	string getHouseFileName() const{ return mHouse->getHouseFileName(); }
	void montageStep() { mHouse->montage(mAlgorithmName, mHouse->getHouseFileName(), mStepsCounter, mSensor->getSensorLocation().second, mSensor->getSensorLocation().first); }
private:
	long mStepsCounter;
	int mScore;
	int mDirtCollected;
	int mBattreyConsumptionRate;
	bool mCrashedIntoWall;
	int mPositionInCompetition;
	int mActualPositionInCompetition;
	bool mIsRunning;
	bool mIsAboutToFinish;
	Direction mPrevSimulationStep;
	string mAlgorithmName;
	string mSimulationError;
	bool mIsVideo;
	AbstractAlgorithm* mAlgorithm;
	House* mHouse;
	unique_ptr<Sensor>  mSensor;
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