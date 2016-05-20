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
#include <thread>
#ifndef _WIN32
#include "MakeUnique.cpp"
#endif
namespace fs = ::boost::filesystem;
using namespace std;

void Simulator::runSimulation(HouseList houses_list, size_t numThreads)
{
	mNumThreads = numThreads;
	if (DEBUG)
	{
		cout << endl << endl << "Parameters read:" << endl;
		cout << mConfiguration->toString() << endl;
		cout << "Num of threads: " << mNumThreads << endl;
	}
	
	// Set the houses list
	mHouseList = houses_list;

	//The simulator should execute every algorithm on every house:
	executeAllAlgoOnAllHouses();

	printScores();
}

bool compareAvg(const pair< double, string > a, const pair< double, string > b)
{
	return a.first > b.first;
}

void Simulator::printScores()
{
	cout << getSupparatorLine();
	cout << getHeaderPrintLine();
	cout << getSupparatorLine();
	int i = 0;
	//sort algorithms by average: 
	list <pair< double, string >> algorithmsByAvg;
	for (list<string>::iterator it = mAlgorithmNames->begin(); it != mAlgorithmNames->end(); ++it)
	{
		double averageResult = 0;
		string algorithmLine = getAlgoPrintLine(i, (*it), averageResult);
		algorithmsByAvg.push_back(make_pair(averageResult, algorithmLine));
		i++;
	}
	algorithmsByAvg.sort(compareAvg);

	for (auto item : algorithmsByAvg)
	{
		cout << item.second;
		cout << getSupparatorLine();
	}
}


//bool compareLexical(const House* first, const House* second)
//{
//	return first->getHouseFileName() < second->getHouseFileName();
//}

string Simulator::getHeaderPrintLine()
{
	string line = "|";
	for (int j = 0; j < 13; j++)
	{
		line += " ";
	}
	line += "|";

	//if (DEBUG)
	//{
	//	cout << "BEFORE sorting by house name : " << endl;
	//	for (HouseList::iterator it = mHouseList->begin(); it != mHouseList->end(); ++it)
	//	{
	//		cout << (*it)->getHouseFileName() << " ";
	//	}
	//	cout << endl;

	//}
	//mHouseList->sort(compareLexical);
	//if (DEBUG)
	//{
	//	cout << "AFTER sorting by house name : " << endl;
	//	for (HouseList::iterator it = mHouseList->begin(); it != mHouseList->end(); ++it)
	//	{
	//		cout << (*it)->getHouseFileName() << " ";
	//	}
	//	cout << endl;

	//}
	for (HouseList::iterator it = mHouseList.begin(); it != mHouseList.end(); ++it)
	{
		House* house = (*it);
		string fileName = house->getHouseFileName();
		if (fileName.length() > 10)
		{
			line += fileName.substr(0, 10);
		}
		else
		{
			line += fileName;
			for (int j = 0; j < (int)(10 - house->getHouseFileName().length()); j++)
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

string Simulator::getAlgoPrintLine(int ind, string algoName, double & averageResult)
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
	for (HouseList::iterator it = mHouseList.begin(); it != mHouseList.end(); ++it)
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

	double avg = counter / (double)mHouseList.size();
	averageResult = avg;
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
	for (int i = 0; i < (int)(15 + (mHouseList.size() + 1) * 11); i++)
	{
		line += "-";
	}

	return line + "\n";
}

void Simulator::cleanResources()
{
	// Clean Houses
	for (HouseList::iterator listHouseIter = mHouseList.begin(); listHouseIter != mHouseList.end(); ++listHouseIter)
	{
		(*listHouseIter)->cleanResources();
		delete *listHouseIter;
	}
	
	delete mAlgorithmNames;
}

int Simulator::countHousesInPath(string houses_path)
{
	int count = 0;
	fs::path targetDir(houses_path);
	fs::directory_iterator it(targetDir), eod;
	if (!fs::exists(targetDir) || !fs::is_directory(targetDir) || fs::is_empty(targetDir))
	{
		return 0;
	}

	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (fs::is_regular_file(p) && p.extension() == HOUSE_EXT)
		{
			count++;
		}
	}
	return count;
}

HouseList Simulator::readAllHouses(string houses_path)
{
	HouseList housesList;
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
				housesList.push_back(house);
			}
		}
	}

	return housesList;
}

AlgorithmList *Simulator:: loadAllAlgorithms(string algorithms_path, bool firstTime)
{
#ifndef _WIN32
	vector<AlgorithmLoader*> allAlgos;
	fs::path targetDir(algorithms_path);
	//check if directory doesn't exist or path is not a directory or directory is empty
	if (!fs::exists( targetDir ) || ! fs::is_directory(targetDir) || fs::is_empty(targetDir)) 
	{
		cout << USAGE;
		cout << "cannot find algorithm files in '" << algorithms_path << "'"<<endl; 
		exit(0);
	}
	fs::directory_iterator it(targetDir), eod;
#ifdef _WIN32
	int i = 0;
#endif
	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (fs::is_regular_file(p) && p.extension() == ".so" && p.string().at(p.string().length()-4) == '_') // change to _.so
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
		cout << USAGE;
		cout << "cannot find algorithm files in '" << algorithms_path << "'"<<endl; 
		exit(0);
	}

	algoLoaders = new LoadersList();

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
			if (firstTime)
			{
				mAlgorithmErrorMessages += algo->getErrorLine();	
			}
			dlclose(algo->handle);
			delete algo;
		}
	}

	if (algoLoaders->empty())
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
		if (firstTime)
		{
			mAlgorithmNames->push_back(loader->fileName);	
		}
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
	if (firstTime)
	{
		mAlgorithmNames->push_back("algo1");
	}
	//don't set the sensor yet.
	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
	algoList->push_back(algoNaive);

//	_200945657_B* algoNaive2 = new _200945657_B();
//	algoNaive->setConfiguration(*mConfiguration->getParametersMap());
//	if (firstTime)
//	{
//		mAlgorithmNames->push_back("algo2");
//	}
//	//don't set the sensor yet.
//	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
//	algoList->push_back(algoNaive2);
//
//	_200945657_C* algoNaive3 = new _200945657_C();
//	algoNaive->setConfiguration(*mConfiguration->getParametersMap());
//	if (firstTime)
//	{
//		mAlgorithmNames->push_back("algo3");
//	}
//	//don't set the sensor yet.
//	//the sensor of the algorithm is related to the house which it is running on, and is set in simulatiom constructor
//	algoList->push_back(algoNaive3);

	return algoList;
#endif
}
string Simulator::getAlgorithmErrorMessages() const
{
	return mAlgorithmErrorMessages;
}

string Simulator::getHousesErrorMessages() const
{
	return mHousesErrorMessages;
}

string Simulator::getScoreErrorMessage() const
{
	if (mIsAnySimulationScoreBad)
	{
		return "Score formula could not calculate some scores, see -1 in the results table \n";
	}
	else
	{
		return "";
	}

}

void Simulator::executeAllAlgoOnAllHouses()
{
	// ===> create the threads as vector of pointers to threads (unique_ptr)
	vector<unique_ptr<thread>> threads(mNumThreads);
	for (auto& thread_ptr : threads) {
		// ===> actually create the threads and run them
		thread_ptr = make_unique<thread>(&Simulator::runSimuationOnHouse, this); // create and run the thread
	}

	// ===> join all the threads to finish nicely (i.e. without crashing / terminating threads)
	for (auto& thread_ptr : threads) {
		thread_ptr->join();
	}
}

void Simulator::runSimuationOnHouse()
{
	while (houseIndex < mHouseList.size())
	{
		House*  house = mHouseList[houseIndex];
		++houseIndex;
		if (DEBUG)
		{
			cout << "running from thread #" << this_thread::get_id() << endl;
		}

		Simulation* pSimulation;
		//initialize a list that holds information about all simulations on current house:
		SimulationList* simulationListPerHouse;
		simulationListPerHouse = new SimulationList();
		//insert all initialized simulations on the current house into the list simulationListPerHouse:
		for (AlgorithmList::const_iterator listAlgorithmIter = house->mAlgorithmList->begin(); listAlgorithmIter != house->mAlgorithmList->end(); ++listAlgorithmIter)
		{
			House* tempHouse = new House();
			tempHouse->fillHouseInfo(house->getHousePath(), house->getHouseFileName());
			pSimulation = new Simulation((*listAlgorithmIter), tempHouse, mConfiguration->getParametersMap());
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
						simul->setActualPositionInCompetition(actualPositionInCopmetition);
						numberOfWinnersInPosition += 1;
					}
				}
				else if (simul->getPositionInCompetition() == -1) // this algorithm finished running but not successfully
				{
					simul->setActualPositionInCompetition(actualPositionInCopmetition);
					simul->setPositionInCompetition(actualPositionInCopmetition);
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

		//all algorithms stopped - terminate simulation on this house and calculate scores
		for (SimulationList::iterator iter3 = simulationListPerHouse->begin(); iter3 != simulationListPerHouse->end(); ++iter3){

			if ((*iter3)->getCrashedIntoWall())
			{
				(*iter3)->setSimulationScore(0);
			}
			else
			{
				if ((*iter3)->getIsOutOfBattery())//if an algorithm is out of battery then it's this_num_steps will equal to simulation steps
				{
					(*iter3)->setStepsCounter(simulationStepsCounter);
				}
				const map<string, int> score_params =
				{
					{ "winner_num_steps", winnerNumberOfSteps },
					{ "simulation_steps", simulationStepsCounter },
					{ "actual_position_in_competition", (*iter3)->getActualPositionInCompetition() },
					{ "this_num_steps", (*iter3)->getStepsCounter() },
					{ "sum_dirt_in_house", (*iter3)->getInitialDustSumInHouse() },
					{ "dirt_collected", (*iter3)->getDirtCollected() },
					{ "is_back_in_docking", (*iter3)->getIsBackInDocking() }
				};
				(*iter3)->setSimulationScore(calculateScore(score_params));
			}
			
			house->algorithmScores->push_back((*iter3)->getSimulationScore());
			if ((*iter3)->getSimulationScore() == -1)
			{
				mIsAnySimulationScoreBad = true;
			}
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

// this is not a member function of simulation class!!! 
int Simulator::calculateSimulationScore(const map<string, int>& score_params){
	int winnerNumberOfSteps;
	int simulationStepsCounter;
	int positionInCompetition;
	int actualPositionInCompetition;
	int stepsCounter;
	int initialDustSumInHouse;
	int dirtCollected;
	bool isBackInDocking;
	try
	{
		// this is not a member function of simulation class!!! 
		winnerNumberOfSteps = score_params.at("winner_num_steps");
		simulationStepsCounter = score_params.at("simulation_steps");
		actualPositionInCompetition = score_params.at("actual_position_in_competition");
		stepsCounter = score_params.at("this_num_steps");
		initialDustSumInHouse = score_params.at("sum_dirt_in_house");
		dirtCollected = score_params.at("dirt_collected");
		isBackInDocking = (score_params.at("is_back_in_docking") != 0);
	}
	catch (out_of_range & e)
	{
		if (SCORE_DEBUG)
		{
			cout << "calculateSimulationScore couldn't find one of the parameters of the score_params " << e.what() << endl;
		}
		return -1;
	}
	if (initialDustSumInHouse == dirtCollected  && isBackInDocking)
	{
		positionInCompetition = actualPositionInCompetition < 4 ? actualPositionInCompetition : 4;

	}
	else
	{
		positionInCompetition = 10;
	}

	int score = 2000;
	score -= (positionInCompetition - 1) * 50;
	score += (winnerNumberOfSteps - stepsCounter) * 10;
	score -= (initialDustSumInHouse - dirtCollected) * 3;
	score += (isBackInDocking ? 50 : -200);
	return score < 0 ? 0 : score;
}