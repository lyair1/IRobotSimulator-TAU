/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "Simulator.h"

using namespace std;

void Simulator::runSimulation(string config_file_path, string houses_path)
{
	mConfiguration = new ConfigReader(config_file_path);
	if (DEBUG)
	{
		cout << endl << endl << "Parameters read:" << endl;
		cout << mConfiguration->toString() << endl;
	}
	
	//The simulator should read all houses descriptions from text files including dust levels:
	readAllHouses(houses_path);

	//The simulator should dynamically load libraries for all the algorithms:
	loadAllAlgorithms();

	//The simulator should execute every algorithm on every house:
	executeAllAlgoOnAllHouses();
}

void Simulator::cleanResources(){


	// Clean Houses
	for (HouseList::iterator listHouseIter = mHouseList->begin(); listHouseIter != mHouseList->end(); listHouseIter++)
	{
		(*listHouseIter)->cleanResources();
		delete *listHouseIter;
	}
	delete mHouseList;

	// Clean algorithms
	for (AlgorithmList::iterator listAlgorithmIter = mAlgorithmList->begin(); listAlgorithmIter != mAlgorithmList->end(); listAlgorithmIter++)
	{
		(*listAlgorithmIter)->cleanResources();
		delete *listAlgorithmIter;
	}
	delete mAlgorithmList;

	// clean configuration
	delete mConfiguration;
}

void Simulator:: readAllHouses(string houses_path)
{
	//TODO: in ex2 we need to go over all the files with extension *.house which are in houses_path. 
	House *house = new House();
	string filePath = "default_generated_house.house";
	if (!house->fillHouseInfo(filePath)){
		// House is illigle - delete
		delete(house);
	}
	else{
		// Add house to the list
		if (DEBUG){
			house->printHouse();
		}
		mHouseList->push_back(house);
	}
}

void Simulator:: loadAllAlgorithms()
{
	AlgorithmNaive* algoNaive = new AlgorithmNaive();
	algoNaive->setConfiguration(*mConfiguration->getParametersMap());
	//don't set the sensor yet.
	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
	mAlgorithmList->push_back(algoNaive);
}

void Simulator::executeAllAlgoOnAllHouses()
{
	Simulation* pSimulation;
	//initialize a list that holds information about all simulations on current house:
	SimulationList* simulationListPerHouse;
	for (HouseList::iterator listHouseIter = mHouseList->begin(); listHouseIter != mHouseList->end(); listHouseIter++)
	{
		simulationListPerHouse = new SimulationList();
		//insert all initialized simulations on the current house into the list simulationListPerHouse:
		for (AlgorithmList::iterator listAlgorithmIter = mAlgorithmList->begin(); listAlgorithmIter != mAlgorithmList->end(); listAlgorithmIter++)
		{
			pSimulation = new Simulation((*listAlgorithmIter), (*listHouseIter), mConfiguration->getParametersMap());
			(*listAlgorithmIter)->setSensor(*(pSimulation->getSensor()));
			simulationListPerHouse->push_back(pSimulation);
		}
		int winnerNumberOfSteps = 0;
		/*	For every house, the simulator should run all algorithms in parallel, in a "round-robin" fashion.
		That is, every time unit we iterate all algorithms and run a single step of the algorithm.
		We stop either when each algorithm has run for MaxSteps time unite,
		or MaxStepsAfterWinner time units after the first robot has finished cleaning the house and returned to its docking station
		(the minimum between the two options).*/
		bool isAnyAlgorithmStillRunning = true;
		int actualPositionInCopmetition = 1;
		int numberOfWinnersInPosition = 0;
		bool isFirstWinner = true;
		int maxSimulatorStepsPerHouse = mConfiguration->getParameter("MaxSteps");
		int simulationStepsCounter = 0;
		while (isAnyAlgorithmStillRunning){ // while none of the algorithms finished cleaning and back in docking
			isAnyAlgorithmStillRunning = false;
			for (SimulationList::iterator iter = simulationListPerHouse->begin(); iter != simulationListPerHouse->end(); ++iter){
				if ((*iter)->isSimulationRunning()){
					isAnyAlgorithmStillRunning = true;
					if ((*iter)->makeSimulationStep()){ 
						if (isFirstWinner)// this is the first algorithm that finished running successfully!
						{
							isFirstWinner = false;
							winnerNumberOfSteps = (*iter)->getNumberOfSteps();
							for (SimulationList::iterator iter2 = simulationListPerHouse->begin(); iter2 != simulationListPerHouse->end(); ++iter2){
								(*iter2)->resetMaxStepsAccordingToWinner();
							}
						}
						(*iter)->setPositionInCompetition((actualPositionInCopmetition < 4) ? actualPositionInCopmetition : 4);
						numberOfWinnersInPosition += 1;
						//update steps counter for all other algorithm too: 
						
					}
				}
			}
			actualPositionInCopmetition += numberOfWinnersInPosition;
			numberOfWinnersInPosition = 0; // after incrementingPositionInCompetition, initiate numberOfWinnersInPosition in order to count from 0 at next iteration.
			simulationStepsCounter += 1;
			
			// Update the winner number of steps to the value of the one algorithm that ran and finished (and didn't win)
			if (winnerNumberOfSteps == 0)
			{
				winnerNumberOfSteps = simulationStepsCounter;
			}

			if (simulationStepsCounter >= maxSimulatorStepsPerHouse)
			{
				winnerNumberOfSteps = maxSimulatorStepsPerHouse;
				break; //exit from while loop
			}
		}
		
		//all algorithms stopped - terminate simulation and calculate scores
		for (SimulationList::iterator iter3 = simulationListPerHouse->begin(); iter3 != simulationListPerHouse->end(); ++iter3){
			(*iter3)->setSimulationScore(winnerNumberOfSteps);
			cout << "[" << (*iter3)->getHouse()->getName() << "]" << "\t" << (*iter3)->getSimulationScore() << "\n";
		}
		// Clean simulationListPerHouse (in next exercise: TODO: write the score into a score matrix before freeing it)
		for (SimulationList::iterator iter4 = simulationListPerHouse->begin(); iter4 != simulationListPerHouse->end(); ++iter4){
			(*iter4)->cleanResources();
			delete *iter4;
		}
		delete simulationListPerHouse;
	}
}
