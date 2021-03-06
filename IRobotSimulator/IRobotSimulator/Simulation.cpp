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
	Direction stepInDirection = mAlgorithm->step(mPrevSimulationStep);
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
			if (DEBUG_LOW_LEVEL)
			{
				mHouse->printHouse();
			}	
		}

		/*	update the sensor on the exact location of the robot, so the sensor may answer its queries - 
			what's the dust level at the current position of the robot,
			and is there a wall touching the robot to its east / west / north / south direction ?
		*/
		mSensor->moveSensor(stepInDirection); // simulation doesn't have to move in the direction that the algorithm suggested, but we do it for now.
		mPrevSimulationStep = stepInDirection; // only update the prevStep after it was really done!
		mStepsHistory.push_back(stepInDirection);
		mStepsCounter++;//step counter incremented even if battery was not spent and the step was "Stay".
		
		if (mStepsCounter == mMaxSteps - mMaxStepsAfterWinner)
		{
			resetMaxStepsAccordingToWinner();
		}
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
					cout << "Simulation terminated due to exceeding BatteryCapacity !" << endl;
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
			std::string stepCounterString;
			std::stringstream strstream;
			strstream << mStepsCounter;
			
			mSimulationError += "Algorithm " + mAlgorithmName + " when running on House " + mHouse->getHouseFileName() + " went on a wall in step " + strstream.str() + "\n";
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
	delete (mHouse);
//	delete(mSensor); make_unique should not be deleted
	
	//remove folder: algoName_HouseName
	path simulationDir("simulations/" + mAlgorithmName + "_" + mHouse->getHouseFileName());
	if (mIsVideo && exists(simulationDir))
	{
		remove_all(simulationDir);
	}
}

bool Simulation::isSimulationRunning() const{
	return mIsRunning;
}
int Simulation::getNumberOfSteps()const {
	return mStepsCounter;
}


int Simulation::getPositionInCompetition() const{
	return mPositionInCompetition;
}

int Simulation::getActualPositionInCompetition() const{
	return mActualPositionInCompetition;
}

void Simulation::setPositionInCompetition(int actualPositionInCompetition){
	if (mHouse->isCleanHouse() && mIsBackInDocking)
	{
		mPositionInCompetition = actualPositionInCompetition < 4 ? actualPositionInCompetition : 4;
		return;
	}
	mPositionInCompetition = 10;
	return;
}

void Simulation::setActualPositionInCompetition(int actualPositionInCompetition)
{
	mActualPositionInCompetition = actualPositionInCompetition;
}


void Simulation::setStepsCounter(int stepsCounter){
	mStepsCounter = stepsCounter;
}

//this function is called only when there's a winner 
//or when there's no winner but the number of steps this simulation made is equal to MaxSteps-MaxStepsAfterWinner
//formula is according to stepsLeft = min {maxStepsAfterWinner, maxSteps - stepsCounter }
//this means that: mMaxSteps = minimum { mMaxSteps, stepsCounter + mMaxStepsAfterWinner }
void Simulation::resetMaxStepsAccordingToWinner(){
	mMaxSteps = mMaxSteps < (mStepsCounter + mMaxStepsAfterWinner) ? mMaxSteps : (mStepsCounter + mMaxStepsAfterWinner);
	if (!mIsAboutToFinish)
	{
		mIsAboutToFinish = true;
		int stepsTillFinishing = mMaxSteps - mStepsCounter;
		mAlgorithm->aboutToFinish(stepsTillFinishing); // this function must only be called once!
	}
}

const int Simulation::getSimulationScore() const {
	return mScore;
}

void Simulation::setSimulationScore(int score)
{
	mScore = score;
}


void Simulation::printSimulationStepsHistory(){
	cout << "************************************";
	cout << "House file name: " << mHouse->getHouseFileName();
	cout << "***********************************" << endl;;
	cout << "House name: " << mHouse->getName() << endl;
	cout << "Algorithm name: " << typeid(this->mAlgorithm).name() << endl;
	cout << "Simulation score: " << this->getSimulationScore() << endl;
	cout << "Simulation steps: " << this->mStepsCounter << endl;
	cout << "Battery status: " << this->mBatteryLeft << endl;
	cout << "Dirt Collected : " << this->mDirtCollected << " (out of " << this->mInitialDustSumInHouse << ")" << endl;
	cout << "back in docking : " << this->mIsBackInDocking << endl;
	cout << "crashed into a wall: " << this->mCrashedIntoWall << endl;

	cout << "Steps History: ";
	for (auto step : mStepsHistory)
		cout << getDirectionString(step) << " ";
	cout << endl << endl;
	mHouse->printHouse();
	cout << "**********************************";
	cout << "end of : " << mHouse->getHouseFileName();
	cout << "**********************************" << endl << endl << endl;

}


