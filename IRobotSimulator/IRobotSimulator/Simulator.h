/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <list>
#include "Simulation.h"
#include "configReader.h"
#include "AlgorithmLoader.h"
#include "score_formula.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif
#include <atomic>
#include <mutex>
using namespace std;

//the simulator class is responsible for running every algorithm on every house, and keeping track of the algorithms preformance according to time
	typedef std::vector<House*> HouseList;
	typedef std::list<Simulation*> SimulationList;
	typedef std::list<AlgorithmLoader*> LoadersList;
	typedef std::list<list<string>*> StringMatrix;
	typedef int(*scoreCreator)(const map<string, int>&);

//this function  is not related to a certin instance of the class simulator(static?)
int calculateSimulationScore(const map<string, int>& score_params);

class Simulator
{
public:
	Simulator(ConfigReader *configuration, scoreCreator _calculateScore) :
		mHouseList(NULL),
		mHousesErrorMessages(""),
		mAlgorithmErrorMessages(""),
		mIsAnySimulationScoreBad(false)
	{
		mConfiguration = configuration;
		mAlgorithmNames = new list<string>;
		calculateScore = _calculateScore;

	}
	~Simulator(){
		#ifndef _WIN32
		for (LoadersList::iterator it = algoLoaders->begin(); it != algoLoaders->end(); ++it)
		{
			AlgorithmLoader *loader = (*it);
			if (loader->handle != NULL)
			{
				dlclose(loader->handle);
			}
			delete loader;
		}

		delete algoLoaders;
		#endif
	}
	void runSimulation(HouseList houses_list, size_t numThreads);
	void cleanResources();
	int countHousesInPath(string houses_path);
	HouseList readAllHouses(string houses_path);
	AlgorithmList *loadAllAlgorithms(string algorithms_path, bool firstTime);
	string getAlgorithmErrorMessages() const;
	string getHousesErrorMessages() const;
	string getScoreErrorMessage() const;
	void runSimuationOnHouse();
private:

	//members:
	LoadersList *algoLoaders;
	HouseList mHouseList;
	string mHousesErrorMessages;
	string mAlgorithmErrorMessages;
	ConfigReader* mConfiguration;
	list<string>* mAlgorithmNames;
	bool mIsAnySimulationScoreBad;
	size_t mNumThreads;
	atomic_size_t houseIndex{0};
	mutex print_lock;

	//functions:
	void executeAllAlgoOnAllHouses();
	void printScores();
	string getSupparatorLine();
	string getAlgoPrintLine(int ind, string algoName, double & averageResult);
	string getHeaderPrintLine();
	scoreCreator calculateScore;
	struct less_than_key
	{
		inline bool operator() (const AlgorithmLoader *struct1, const AlgorithmLoader *struct2)
		{
			return (struct1->fileName < struct2->fileName);
		}
	};
};

#endif //SIMULATOR_H