/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "Simulation.h"

//return value: boolean representhing if the algorithm finished running SUCCESSFULLY (house is clean and robot is in docking station)
bool Simulation :: makeSimulationStep()
{
	SensorInformation info = mSensor->sense();
	Direction stepInDirection = mAlgorithm->step();
	// in course forum it says if battery > 0 it's enough to make a step, even if BatteryLeft< mBattreyConsumptionRate
	if ((stepInDirection == Direction::Stay ||
		(stepInDirection != Direction::Stay && !info.isWall[(int)stepInDirection])) &&
		mBatteryLeft > 0 && 
		mStepsCounter < mMaxSteps)
	{
		//update the dust-levels as the cleaning process goes on (clean BEFORE moving to next location)
		if (mHouse->isDirtCollected(mSensor->getSensorLocation()))
		{
			mDirtCollected++;
			if (DEBUG)
			{
				mHouse->printHouse();
			}			
		}

		/*	update the sensor on the exact location of the robot, so the sensor may answer its queries - 
			what's the dust level at the current position of the robot,
			and is there a wall touching the robot to its east / west / north / south direction ?
		*/
		mSensor->moveSensor(stepInDirection);
		mStepsCounter++;//step counter incremented even if battery was not spent and the step was "Stay".
		if (mStepsCounter >= mMaxSteps)
		{
			mIsRunning = false;
			if (DEBUG)
			{
				cout << "Simulation terminated due to exceeding MaxSteps !" << endl;
			}
		}
		if (mHouse->getLocationValue(mSensor->getSensorLocation()) != 'D') // battery is spent only when not in docking station
		{ 		
			mBatteryLeft -= mBattreyConsumptionRate;
			if ( mBatteryLeft <= 0)
			{
				mIsRunning = false;
				mIsOutOfBattery = true;
				if (DEBUG)
				{
					cout << " Simulation terminated due to exceeding BatteryCapacity !" << endl;
				}
			}
		}
		else{ // the robot is in docking and battery is recharged 
			if (mBatteryLeft < mBatteryCapacity){
				mBatteryLeft += mBatteryRechargeRate;
				if (mBatteryLeft > mBatteryCapacity)
				{
					mBatteryLeft = mBatteryCapacity;
				}
			}
			if (mHouse->isCleanHouse())
			{
				mIsRunning = false;
				mIsBackInDocking = true;
				if (DEBUG)
				{
					cout << "Simulation successfully finished!  The house is clean, The robot is in the docking station" << endl;
				}
				return true;
			}
		}
		return false;
	}
	else
	{
		/*	If an algorithm makes an invalid step, e.g., walk into a wall,
			the simulator should identify it;
			stop the simulation of this specific algorithm,
			and output that the algorithm made an invalid step.
		*/
		if (stepInDirection != Direction::Stay && info.isWall[(int)stepInDirection])
		{
			mIsRunning = false;
			mCrashedIntoWall = true;
			if (DEBUG)
			{
				cout << " Simulation terminated due to  an invalid step! (crashed into a wall) " << endl;
			}
			return false;
		}
		else{ // this simulation terminated in the previous step - don't print an error again.
			if (DEBUG)
			{
				cout <<"Simulation:: makeSimulationStep() Error - should never get here!"<< endl;
			}
			mIsRunning = false;
			return false;
		}
		
	}

}

void Simulation::cleanResources(){
	delete(mSensor);
}

bool Simulation::isSimulationRunning(){
	return mIsRunning;
}
int Simulation::getNumberOfSteps(){
	return mStepsCounter;
}

const AbstractSensor* Simulation::getSensor(){
	return mSensor;
}

int Simulation::getPositionInCompetition(){
	return mPositionInCompetition;
}

void Simulation::setPositionInCompetition(int actualPositionInCompetition){
	if (actualPositionInCompetition == -1)
	{
		mPositionInCompetition = 10;
		return;
	}
	if (mHouse->isCleanHouse() && mIsBackInDocking)
	{
		mPositionInCompetition = actualPositionInCompetition < 4 ? actualPositionInCompetition : 4;
	}
	
	return;
}
//formula is according to stepsLeft = min {maxStepsAfterWinner, maxSteps - stepsCounter }
//this means that: mMaxSteps = minimum { mMaxSteps, stepsCounter + mMaxStepsAfterWinner }
void Simulation::resetMaxStepsAccordingToWinner(){
	mMaxSteps = mMaxSteps < (mStepsCounter + mMaxStepsAfterWinner) ? mMaxSteps : (mStepsCounter + mMaxStepsAfterWinner);
}

void Simulation::setSimulationScore(int winnerNumberOfSteps, int simulationStepsCounter){
	if (mCrashedIntoWall){
		mScore = 0;

		return;
	}
	
	int score = 2000;
	score -= (mPositionInCompetition - 1) * 50;
	if (mIsOutOfBattery)
	{
		score += (winnerNumberOfSteps - simulationStepsCounter) * 10;
	}
	else
	{
		score += (winnerNumberOfSteps - mStepsCounter) * 10;
	}
		
	score -= (mInitialDustSumInHouse - mDirtCollected) * 3;
	score += (mIsBackInDocking ? 50 : -200);
	mScore = score < 0 ? 0 : score;

	return;
}
const House* Simulation::getHouse(){
	return mHouse;
}

const int Simulation::getSimulationScore()
{
	return mScore;
}

void Simulation::printSimulationScore(string houseName, const int score)
{
	// TODO in Ex2:
	//Save score into matrix of scores and print matrix at the end 
	//cout << "[" << houseName << "]" << "\t" << score << "\n";

	//in Ex 1:
	cout << score << endl;
	if (mCrashedIntoWall) // exercise demands this is the only message that will be printed, after score is printed!
	{
		cout << " Simulation terminated due to  an invalid step! (crashed into a wall) " << endl;
	}
	return;
}