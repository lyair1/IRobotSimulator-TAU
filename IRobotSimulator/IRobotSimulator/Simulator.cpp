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
#include <dlfcn.h>
#endif

namespace fs = ::boost::filesystem;
using namespace std;

Simulator::Simulator(bool scoreFormulaReceived, string scoreFormulaPath, int numThreads, string housesPath, string algorithmsPath, string configFilePath, bool isVideo) :
	mHousesPath(housesPath),
	mAlgorithmsPath(algorithmsPath),
	mConfigFilePath(configFilePath),
	mScoreFormulaPath(scoreFormulaPath),
	mHousesErrorMessages(""),
	mAlgorithmErrorMessages(""),
	mSimulationErrorMessages(""),
	mScoreHandle(NULL),
	mConfiguration(NULL),
	mIsAnySimulationScoreBad(false),
	mNumThreads(numThreads),
	mScoreFormulaReceived(scoreFormulaReceived),
	mIsVideo(isVideo)
{
	mHouseList.clear();
	mCalculateScore = NULL;

}

Simulator::~Simulator(){
}

void Simulator::initSimulator()
{
	mConfiguration = new ConfigReader(mConfigFilePath);
	handleConfigFileErrors();// check if config file can be loaded and if values are missing
	handleScore(); // The score formula is the second argument to be checked on startup (after config file).
	readAllHouses();
	loadAllAlgorithms();
	
	for (HouseList::iterator listHouseIter = mHouseList.begin(); listHouseIter != mHouseList.end(); ++listHouseIter)
	{
		House*  house = (*listHouseIter);
		house->mAlgorithmList = AlgorithmLoader::getInstance().getAlgorithms(); // this function creates NEW algorithms using the factory every time it's called
	}
	runSimulation();
	// Print error list
	if (getHousesErrorMessages().length() > 0 || getAlgorithmErrorMessages().length() > 0 || getScoreErrorMessage().length() > 0 || getSimulationsErrorMessage().length() > 0 )
	{
		cout << "\nErrors:\n" << getHousesErrorMessages() << getAlgorithmErrorMessages() << getScoreErrorMessage() << getSimulationsErrorMessage();
	}
	cleanResources();

}

void Simulator::handleConfigFileErrors()
{
	if (!mConfiguration->isAllParamteresLegalInConfigFile)
	{
		cout << mConfiguration->getMessageForBadParamsInConfigFile();
	}
	if (!mConfiguration->isAllParamteresExistInConfigFile())
	{
		cout << mConfiguration->getMessageForMissingParamsInConfigFile();
	}
	if (!mConfiguration->isAllParamteresLegalInConfigFile || !mConfiguration->isAllParamteresExistInConfigFile())
	{
		delete mConfiguration;
		exit(0);
	}
}

void Simulator::handleScore()
{
	
	if (!mScoreFormulaReceived)//In case score_formula argument is not provided in the command line, the default score formula shall be used
	{
		mCalculateScore = calculateSimulationScore;
		return;
	}
	const string defaultScoreFormulaFileName = "score_formula.so";
	fs::path targetDir(mScoreFormulaPath);

	if (!fs::exists(targetDir) || !fs::is_directory(targetDir) || fs::is_empty(targetDir))
	{
		cout << USAGE;// Show usage and return if score file doesn't exists in path
		cout << "cannot find score_formula.so file in " << fs::complete(targetDir) << endl;
		exit(0);
	}
	fs::path scoreFile(mScoreFormulaPath + defaultScoreFormulaFileName);
	if (!fs::exists(scoreFile))
	{
		cout << USAGE;// Show usage and return if score file doesn't exists in path
		cout << "cannot find score_formula.so file in " << fs::complete(targetDir) << endl;
		exit(0);
	}

#ifndef _WIN32
	char* error;
	void *mScoreHandle = dlopen(scoreFile.string().c_str(), RTLD_NOW);// Opening the .so file:
	if (mScoreHandle == NULL)
	{
		cout << "score_formula.so exists in " << fs::complete(targetDir) << " but cannot be opened or is not a valid .so" << endl;
		exit(0);
	}
	dlerror();// clear the current errors
	// calc_score is the instance creator method
	void* p = dlsym(mScoreHandle, "calc_score");
	if((error =  dlerror()) != NULL)
	{
		cout << "score_formula.so is a valid .so but it does not have a valid score formula" << endl;
		cout << error << endl;
		dlclose(mScoreHandle);
		exit(0);
	}
	scoreCreator calc_score = reinterpret_cast<scoreCreator>(reinterpret_cast<long>(p));
	if (calc_score == nullptr)
	{
		cout << "score_formula.so is a valid .so but it does not have a valid score formula" << endl;
		dlclose(mScoreHandle);
		exit(0);
	}
	mCalculateScore = calc_score;
	return ;
#endif
	mCalculateScore = calculateSimulationScore; 
	return;
}

void Simulator::runSimulation()
{
	
	if (DEBUG)
	{
		cout << endl << endl << "Parameters read:" << endl;
		cout << mConfiguration->toString() << endl;
		cout << "Num of threads: " << mNumThreads << endl;
	}

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
	for (string & algoName : mAlgorithmNames)
	{
		double averageResult = 0;
		string algorithmLine = getAlgoPrintLine(i, algoName, averageResult);
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
		for (list<int>::iterator algoIt = house->algorithmScores.begin(); algoIt != house->algorithmScores.end(); ++algoIt)
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
	if (mConfiguration != NULL)
	{
		delete mConfiguration;
		mConfiguration = NULL;
	}
#ifndef _WIN32
	if (mScoreHandle != NULL)
	{
		dlclose(mScoreHandle);
	}
#endif

	
}


void Simulator::readAllHouses()
{
	int count = 0;
	/*in case the folder for the house files is bad for some reason or leads to a directory
	with no house files, or is missing and there are no house files in the working directory :
	*/
	fs::path targetDir(mHousesPath);
	if (!fs::exists(targetDir) || !fs::is_directory(targetDir) || fs::is_empty(targetDir))
	{
		cout << USAGE;
		cout << "cannot find house files in " << fs::complete(targetDir) << endl;
		exit(0);// Print message and return if all houses in path are invalid
	}
	fs::directory_iterator it(targetDir), eod;
	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (fs::is_regular_file(p) && p.extension() == HOUSE_EXT)
		{
			count++; // a house file was found
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
				mHouseList.push_back(house);
			}
		}
	}
	if (count == 0)
	{
		cout << USAGE;
		cout << "cannot find house files in " << fs::complete(targetDir) << endl;
		exit(0);// Print message and return if all houses in path are invalid
	}
	// Check if all houses are invalid
	if (mHouseList.empty())
	{
		std::cout << "All house files in target folder " << fs::complete(targetDir) << " cannot be opened or are invalid:\n" << getHousesErrorMessages();
		exit(0);
	}
}

void Simulator::loadAllAlgorithms()
{

	fs::path targetDir(mAlgorithmsPath);
	//check if directory doesn't exist or path is not a directory or directory is empty
	if (!fs::exists( targetDir ) || ! fs::is_directory(targetDir) || fs::is_empty(targetDir)) 
	{
		cout << USAGE;
		cout << "cannot find algorithm files in " << fs::complete(mAlgorithmsPath) << endl; 
		exit(0);
	}
#ifndef _WIN32
	fs::directory_iterator it(targetDir), eod;
	AlgorithmLoader::getInstance().setRegistrationOn(true);

	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (fs::is_regular_file(p) && p.extension() == ".so" )
		{
			if (p.string().length()>=4 && p.string().at(p.string().length()-4) == '_') // change to _.so
			{
				if (DEBUG)
				{
					cout << "scan  _.so file :" << p.string() << "\n";
				}

				AlgorithmLoader::getInstance().loadAlgorithm(p.string(), p.stem().string());
			}
		}
	}

	size_t numOfAlgosLoaded = AlgorithmLoader::getInstance().size();
	if (numOfAlgosLoaded == 0)
	{
		cout << USAGE;
		cout << "cannot find algorithm files in " << fs::complete(targetDir) << endl; 
		exit(0);
	}
	AlgorithmLoader::getInstance().setRegistrationOn(false);
	mAlgorithmErrorMessages = AlgorithmLoader::getInstance().getAlgorithmErrorMessage();
	if (DEBUG)
	{
		cout << "Algorithms loading errors:: "<< mAlgorithmErrorMessages<< endl; 
	}

#endif
	mAlgorithms = AlgorithmLoader::getInstance().getAlgorithms();
	mAlgorithmNames = AlgorithmLoader::getInstance().getAlgorithmNames();
	if (mAlgorithms.empty())
	{
		cout << "All algorithm files in target folder " << fs::complete(targetDir) << " cannot be opened or are invalid: \n" << getAlgorithmErrorMessages();
		exit(0);
	}
	return;
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


string Simulator::getSimulationsErrorMessage() const
{
	return mSimulationErrorMessages;
}

void Simulator::executeAllAlgoOnAllHouses()
{
	int actualThreads = (int)mHouseList.size() < mNumThreads ? mHouseList.size() : mNumThreads;
	if (actualThreads == 1) // no point in having a thread run and wait for it in "join", simply run the simulation.
	{
		runSimuationOnHouse();
		return;
	}
	vector<unique_ptr<thread>> threads(actualThreads); // create the threads as vector of pointers to threads (unique_ptr)
	for (auto& thread_ptr: threads) 
	{
		thread_ptr = make_unique<thread>(&Simulator::runSimuationOnHouse, this); // actually create the threads and run them
	}

	// join all the threads to finish nicely (i.e. without crashing / terminating threads)
	for (auto& thread_ptr : threads)
	{
		thread_ptr->join();
	}
}

void Simulator::runSimuationOnHouse()
{
	while (mHouseIndex < mHouseList.size())
	{
		House*  house = mHouseList[mHouseIndex];
		++mHouseIndex;
		if (DEBUG)
		{
			cout << "running from thread #" << this_thread::get_id() << endl;
		}

		//initialize a list that holds information about all simulations on current house:
		list<unique_ptr<Simulation >> simulationListPerHouse;
		list<unique_ptr<AbstractAlgorithm>> algorithms = AlgorithmLoader::getInstance().getAlgorithms();
		list<string> algorithmsNames = AlgorithmLoader::getInstance().getAlgorithmNames();
		//insert all initialized simulations on the current house into the list simulationListPerHouse:
		list<string>::iterator algoName = algorithmsNames.begin();
		for (unique_ptr<AbstractAlgorithm>& algo : algorithms)
		{
			House* tempHouse = new House();
			tempHouse->fillHouseInfo(house->getHousePath(), house->getHouseFileName());
			simulationListPerHouse.push_back(make_unique<Simulation>(*algo, *algoName, tempHouse, mConfiguration->getParametersMap(), mIsVideo));
			algoName++;
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
			for (unique_ptr<Simulation >& simul : simulationListPerHouse)
			{
				if (simul->isSimulationRunning())
				{
					isAnyAlgorithmStillRunning = true;
					bool isSuccess = simul->makeSimulationStep();
					if (mIsVideo)
					{
#ifndef _WIN32
						simul->montageStep();
#endif
					}
						if (isSuccess)
					{
						if (isFirstWinner)// this is the first algorithm that finished running successfully!
						{
							isFirstWinner = false;
							winnerNumberOfSteps = simul->getNumberOfSteps();
							for (unique_ptr<Simulation >& iter2 : simulationListPerHouse) 
							{
								if (iter2 != simul)//update steps counter for all OTHER algorithms BUT for the first winner 
								{
									iter2->resetMaxStepsAccordingToWinner();
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
		for (unique_ptr<Simulation> & iter3 :simulationListPerHouse){

			if (iter3->getCrashedIntoWall())
			{
				iter3->setSimulationScore(0);
			}
			else
			{
				if (iter3->getIsOutOfBattery())//if an algorithm is out of battery then it's this_num_steps will equal to simulation steps
				{
					iter3->setStepsCounter(simulationStepsCounter);
				}
				const map<string, int> score_params =
				{
					{ "winner_num_steps", winnerNumberOfSteps },
					{ "simulation_steps", simulationStepsCounter },
					{ "actual_position_in_competition", iter3->getActualPositionInCompetition() },
					{ "this_num_steps", iter3->getStepsCounter() },
					{ "sum_dirt_in_house", iter3->getInitialDustSumInHouse() },
					{ "dirt_collected", iter3->getDirtCollected() },
					{ "is_back_in_docking", iter3->getIsBackInDocking() }
				};
				iter3->setSimulationScore(mCalculateScore(score_params));
			}
			
			house->algorithmScores.push_back(iter3->getSimulationScore());
			if (iter3->getSimulationScore() == -1)
			{
				mIsAnySimulationScoreBad = true;
			}
		}

		// Clean simulationListPerHouse
		for (unique_ptr<Simulation> & iter4: simulationListPerHouse){
			if (DEBUG)
			{
				iter4->printSimulationStepsHistory();
			}
			mSimulationErrorMessages += iter4->getSimulationErrors();
			
			string simulationDir = "simulations/" + iter4->getAlgorithmName() + "_" + iter4->getHouseFileName() + "/";
			string imagesExpression = simulationDir + "image%5d.jpg";
			Encoder::encode(imagesExpression, iter4->getAlgorithmName() + "_" + iter4->getHouseFileName() + ".mpg");

			iter4->cleanResources();
		}
	}
}

// this is not a member function of simulation class!!! 
int calculateSimulationScore(const map<string, int>& score_params){
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
	catch (...)
	{
		if (SCORE_DEBUG)
		{
			cout << "calculateSimulationScore couldn't find one of the parameters of the score_params " << endl;
			cout << "use simulationStepsCounter" << simulationStepsCounter << endl;;
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