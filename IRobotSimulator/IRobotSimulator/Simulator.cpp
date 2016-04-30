/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "Simulator.h"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp> 
#include <iomanip>
#include "200945657_C_.h"
#include "200945657_B_.h"
#include "200945657_A_.h"

namespace fs = ::boost::filesystem;
using namespace std;

void Simulator::runSimulation(HouseList* houses_list, AlgorithmList* algorithmsList)
{
	if (DEBUG)
	{
		cout << endl << endl << "Parameters read:" << endl;
		cout << mConfiguration->toString() << endl;
	}
	
	// Set the houses list
	mHouseList = houses_list;

	//The simulator should dynamically load libraries for all the algorithms:
	mAlgorithmList = algorithmsList;

	//The simulator should execute every algorithm on every house:
	executeAllAlgoOnAllHouses();

	printScores();
}

void Simulator::printScores()
{
	cout << getSupparatorLine();
	cout << getHeaderPrintLine();
	cout << getSupparatorLine();
	int i = 0;
	for (list<string>::iterator it = mAlgorithmNames->begin(); it != mAlgorithmNames->end(); ++it)
	{
		cout << getAlgoPrintLine(i, (*it));
		cout << getSupparatorLine();
		i++;
	}
}

string Simulator::getHeaderPrintLine()
{
	string line = "|";
	for (int j = 0; j < 13; j++)
	{
		line += " ";
	}
	line += "|";

	for (HouseList::iterator it = mHouseList->begin(); it != mHouseList->end(); ++it)
	{
		House* house = (*it);
		string fileName = house->houseFileName;
		if (fileName.length() > 10)
		{
			line += fileName.substr(0, 10);
		}
		else
		{
			line += fileName;
			for (int j = 0; j < (int)(10 - house->houseFileName.length()); j++)
			{
				line += " ";
			}
		}

		line += "|";
	}

	line += "AVG";
	for (int j = 0; j < (10 - 3); j++)
	{
		line += " ";
	}
	line += "|";

	return line + "\n";
}

string Simulator::getAlgoPrintLine(int ind, string algoName)
{
	string line = "|";
	for (int j = 0; j < 13; j++)
	{
		if (j < (int)algoName.length())
		{
			line += algoName[j];
			continue;
		}
		line += " ";
	}
	line += "|";


	int counter = 0;
	for (HouseList::iterator it = mHouseList->begin(); it != mHouseList->end(); ++it)
	{
		int i = 0;
		House* house = (*it);
		for (list<int>::iterator algoIt = house->algorithmScores->begin(); algoIt != house->algorithmScores->end(); ++algoIt)
		{
			int score(*algoIt);
			if (i == ind)
			{
				counter += score;
				string numStr = to_string(score);
				for (int j = 0; j < (int)(10 - numStr.length()); j++)
				{
					line += " ";
				}
				line += numStr + "|";
			}
			i++;
		}
	}

	double avg = counter / (double)mHouseList->size();
	string numStr = to_string(avg);
	string numStr2Dec = "";
	for (int j = 0; j < (int)numStr.length(); j++)
	{
		numStr2Dec += numStr[j];
		if (numStr[j] == '.')
		{
			numStr2Dec += numStr[j+1];
			numStr2Dec += numStr[j+2];
			break;
		}
	}

	for (int j = 0; j < (int)(10 - numStr2Dec.length()); j++)
	{
		line += " ";
	}
	line += numStr2Dec + "|";

	return line + "\n";
}

string Simulator::getSupparatorLine()
{
	string line = "";
	for (int i = 0; i < (int)(15 + (mHouseList->size() + 1) * 11); i++)
	{
		line += "-";
	}

	return line + "\n";
}

void Simulator::cleanResources()
{
	// Clean Houses
	for (HouseList::iterator listHouseIter = mHouseList->begin(); listHouseIter != mHouseList->end(); ++listHouseIter)
	{
		(*listHouseIter)->cleanResources();
		delete *listHouseIter;
	}

	// Clean algorithms
	for (AlgorithmList::iterator listAlgorithmIter = mAlgorithmList->begin(); listAlgorithmIter != mAlgorithmList->end(); ++listAlgorithmIter)
	{
		delete *listAlgorithmIter; // this calls the destructor which will call cleanResources. 
	}
	delete mAlgorithmList;
	
	delete mAlgorithmNames;

}

int Simulator::countHousesInPath(string houses_path)
{
	int count = 0;
	fs::path targetDir(houses_path);
	fs::directory_iterator it(targetDir), eod;
	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (fs::is_regular_file(p) && p.extension() == HOUSE_EXT)
		{
			count++;
		}
	}

	return count;
}

HouseList* Simulator::readAllHouses(string houses_path)
{
	HouseList *housesList = new HouseList();;
	fs::path targetDir(houses_path);
	fs::directory_iterator it(targetDir), eod;
	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (fs::is_regular_file(p) && p.extension() == HOUSE_EXT)
		{
			House *house = new House();
			string errMsg = house->fillHouseInfo(p.string(), p.stem().string());
			if (errMsg.length() > 0){
				// House is illegal - delete
				delete(house);
				mHousesErrorMessages += errMsg;
			}
			else{
				// Add house to the list
				if (DEBUG){
					house->printHouse();
				}
				housesList->push_back(house);
			}
		}
	}

	return housesList;
}

AlgorithmList *Simulator:: loadAllAlgorithms(string algorithms_path)
{
#ifndef _WIN32
	list<AlgorithmLoader*> allAlgos;
	fs::path targetDir(algorithms_path);
	fs::directory_iterator it(targetDir), eod;
#ifdef _WIN32
	int i = 0;
#endif
	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (fs::is_regular_file(p) && p.extension() == ".so")
		{
			if (DEBUG)
			{
				cout << "scan file: :" << p.string() << "\n";
			}
		#ifndef _WIN32
			allAlgos.push_back(new AlgorithmLoader(p.string(), p.stem().string()));
		#else
			allAlgos.push_back(new AlgorithmLoader(new AlgorithmNaive(), "ALGO" + i));
			i++;
		#endif
		}
	}

	sort(allAlgos.begin(), allAlgos.end(), less_than_key());

	if (allAlgos.empty())
	{
		if (DEBUG)
		{
			cout << "can't find algorithms files\n";
		}
		cout << USAGE;
		return nullptr;
	}

	LoadersList *algoLoaders = new LoadersList();

	for (AlgorithmLoader* algo : allAlgos)
	{
		if (algo->isValid())
		{
			algoLoaders->push_back(algo);
			if (DEBUG)
			{
				cout << "Algorithm is valid\n";
			}
		}
		else
		{
			mAlgorithmErrorMessages += algo->getErrorLine();
			delete algo;
		}
	}

	if (algoLoaders.empty())
	{
		delete algoLoaders;
		if (DEBUG)
		{
			cout << "can't load any algorithm\n";
		}

		return nullptr;
	}

	AlgorithmList *algoList = new AlgorithmList();

	for (LoadersList::iterator it = algoLoaders->begin(); it != algoLoaders->end(); ++it)
	{
		AlgorithmLoader *loader = (*it);
		AbstractAlgorithm* algo = globalFactory[loader->fileName]();
		mAlgorithmNames->push_back(loader->fileName);
		algoList->push_back(algo);
	}

	if (DEBUG)
	{
		cout << "retrun algorithm list\n";
	}

	return algoList;

#else

	AlgorithmList *algoList = new AlgorithmList();

	_200945657_A* algoNaive = new _200945657_A();
	algoNaive->setConfiguration(*mConfiguration->getParametersMap());
	mAlgorithmNames->push_back("algo1");
	//don't set the sensor yet.
	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
	algoList->push_back(algoNaive);

	_200945657_B* algoNaive2 = new _200945657_B();
	algoNaive->setConfiguration(*mConfiguration->getParametersMap());
	mAlgorithmNames->push_back("algo2");
	//don't set the sensor yet.
	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
	algoList->push_back(algoNaive2);

	_200945657_C* algoNaive3 = new _200945657_C();
	algoNaive->setConfiguration(*mConfiguration->getParametersMap());
	mAlgorithmNames->push_back("algo3");
	//don't set the sensor yet.
	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
	algoList->push_back(algoNaive3);

	return algoList;
#endif
}

void Simulator::executeAllAlgoOnAllHouses()
{
	Simulation* pSimulation;
	//initialize a list that holds information about all simulations on current house:
	SimulationList* simulationListPerHouse;
	for (HouseList::iterator listHouseIter = mHouseList->begin(); listHouseIter != mHouseList->end(); ++listHouseIter)
	{
		House* house = (*listHouseIter);
		simulationListPerHouse = new SimulationList();
		//insert all initialized simulations on the current house into the list simulationListPerHouse:
		for (AlgorithmList::iterator listAlgorithmIter = mAlgorithmList->begin(); listAlgorithmIter != mAlgorithmList->end(); ++listAlgorithmIter)
		{
			pSimulation = new Simulation((*listAlgorithmIter), house, mConfiguration->getParametersMap());
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
		int maxSimulatorStepsPerHouse = house->getMaxSteps();
		if (maxSimulatorStepsPerHouse < 0)
		{
			return;
		}
		int simulationStepsCounter = 0;
		while (isAnyAlgorithmStillRunning)// while none of the algorithms finished cleaning and back in docking
		{ 
			isAnyAlgorithmStillRunning = false;
			for (SimulationList::iterator iter = simulationListPerHouse->begin(); iter != simulationListPerHouse->end(); ++iter)
			{
				Simulation *simul = (*iter);
				if (simul->isSimulationRunning())
				{
					isAnyAlgorithmStillRunning = true;
					if (simul->makeSimulationStep())
					{ 
						if (isFirstWinner)// this is the first algorithm that finished running successfully!
						{
							isFirstWinner = false;
							winnerNumberOfSteps = simul->getNumberOfSteps();
							for (SimulationList::iterator iter2 = simulationListPerHouse->begin(); iter2 != simulationListPerHouse->end(); ++iter2)
							{
								if ((*iter2) != simul)//update steps counter for all OTHER algorithms BUT for the first winner 
								{
									(*iter2)->resetMaxStepsAccordingToWinner();
								}
							}
						}
						simul->setPositionInCompetition(actualPositionInCopmetition);
						numberOfWinnersInPosition += 1;
					}
				}
				else if (simul->getPositionInCompetition() == -1) // this algorithm finished running but not successfully
				{
					simul->setPositionInCompetition(-1);
				}
			}
			actualPositionInCopmetition += numberOfWinnersInPosition;
			numberOfWinnersInPosition = 0; // after incrementingPositionInCompetition, initiate numberOfWinnersInPosition in order to count from 0 at next iteration.
			if (isAnyAlgorithmStillRunning)
			{
				simulationStepsCounter += 1;
			}
			
			if (simulationStepsCounter >= maxSimulatorStepsPerHouse) // all simulations on the current house exceeded maxSteps
			{
				winnerNumberOfSteps = maxSimulatorStepsPerHouse;
				break; //exit from while loop - go to next house
			}
		}
		// if no algorithm finished successfully:
		if (winnerNumberOfSteps == 0)
		{
			winnerNumberOfSteps = simulationStepsCounter;
		}

		//all algorithms stopped - terminate simulation and calculate scores
		for (SimulationList::iterator iter3 = simulationListPerHouse->begin(); iter3 != simulationListPerHouse->end(); ++iter3){
			(*iter3)->setSimulationScore(winnerNumberOfSteps, simulationStepsCounter);
			(*iter3)->getHouse()->algorithmScores->push_back((*iter3)->getSimulationScore());
		}


		// Clean simulationListPerHouse
		for (SimulationList::iterator iter4 = simulationListPerHouse->begin(); iter4 != simulationListPerHouse->end(); ++iter4){
			if (DEBUG)
			{
				(*iter4)->printSimulationStepsHistory();
			}
			(*iter4)->cleanResources();
			delete *iter4;
		}
		delete simulationListPerHouse;
	}
}


