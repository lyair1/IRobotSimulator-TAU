#include "../include/Simulator.h"

using namespace std;

void Simulator:: runSimulation(char * config_file_path,char* houses_path)
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

void Simulator:: readAllHouses(char* houses_path)
{
	//for (file in houses_path)
	//if fileExtention == ".house"
	//	loadHouse()
	//TODO: in ex2 we need to go over all the files with extension *.house which are in houses_path. 
	House *house = new House();
	string filePath = "simple1.txt";
	house->fillHouseInfo(filePath);
	if (DEBUG){
		house->printHouse();
	}
	mHouseList->push_back(house);
}

void Simulator:: loadAllAlgorithms()
{
	AlgorithmNaive* algoNaive = new AlgorithmNaive();
	algoNaive->setConfiguration(mConfiguration->getParametersMap());
	//don't set the sensor yet.
	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
	mAlgorithmList->push_back(algoNaive);
}


void Simulator::executeAllAlgoOnAllHouses()
{
	int numOfSteps = 0;
	Simulation* pSimulation;
	//initialize a vector that holds information about all simulations:
	
	for (HouseList::iterator listHouseIter = mHouseList->begin(); listHouseIter != mHouseList->end(); listHouseIter++)
	{
		
		vector< Simulation*> simulationVectorPerHouse;
		//insert all initialized simulations on the current house into the vector simulationVectorPerHouse:
		for (AlgorithmList::iterator listAlgorithmIter = mAlgorithmList->begin(); listAlgorithmIter != mAlgorithmList->end(); listAlgorithmIter++)
		{

			pSimulation = new Simulation((*listAlgorithmIter), (*listHouseIter), mConfiguration->getParametersMap());
			(*listAlgorithmIter)->setSensor(*(pSimulation->getSensor()));
			simulationVectorPerHouse.push_back(pSimulation);
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
			for (std::vector<Simulation*>::iterator iter = simulationVectorPerHouse.begin(); iter != simulationVectorPerHouse.end(); ++iter){
				if ((*iter)->isSimulationRunning()){
					isAnyAlgorithmStillRunning = true;
					if ((*iter)->makeSimulationStep()){ 
						if (isFirstWinner)// this is the first algorithm that finished running successfully!
						{
							isFirstWinner = false;
							winnerNumberOfSteps = (*iter)->getNumberOfSteps();
							for (std::vector<Simulation*>::iterator iter2 = simulationVectorPerHouse.begin(); iter2 != simulationVectorPerHouse.end(); ++iter2){
								(*iter2)->resetMaxStepsAccordingToWinner();
							}
						}
						(*iter)->setPositionInCompetition((actualPositionInCopmetition < 4) ? actualPositionInCopmetition : 4);
						numberOfWinnersInPosition += 1;
						//update steps counter for all other algorithm to : 

						
					}
				}
			}
			actualPositionInCopmetition += numberOfWinnersInPosition;
			numberOfWinnersInPosition = 0; // after incrementingPositionInCompetition, initiate numberOfWinnersInPosition in order to count from 0 at next iteration.
			simulationStepsCounter += 1;
			if (simulationStepsCounter >= maxSimulatorStepsPerHouse)
			{
				winnerNumberOfSteps = maxSimulatorStepsPerHouse;
				break; //exit from while loop
			}

		}//all algorithms stopped - terminate simulation and calculate scores
		for (std::vector<Simulation*>::iterator iter3 = simulationVectorPerHouse.begin(); iter3 != simulationVectorPerHouse.end(); ++iter3){
			(*iter3)->setSimulationScore(winnerNumberOfSteps);
			cout << "[" << (*iter3)->getHouse()->getName() << "]" << "\t" << (*iter3)->getSimulationScore() << "\n";
		}
	}


}
