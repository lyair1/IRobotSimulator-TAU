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
using namespace std;

//the simulator class is responsible for running every algorithm on every house, and keeping track of the algorithms preformance according to time
	typedef std::vector<House*> HouseList;
	typedef std::list<Simulation*> SimulationList;
	typedef std::list<AlgorithmLoader*> LoadersList;
	typedef std::list<list<string>*> StringMatrix;
	typedef int(*scoreCreator)(const map<string, int>&);

//this function  is not related to a certin instance of the class simulator(static?)

class Simulator
{
public:
	
	Simulator(const Simulator& otherSimulator);
	Simulator& operator=(const Simulator&) = delete;
	Simulator(ConfigReader *configuration, scoreCreator _calculateScore);
	~Simulator(){
		#ifndef _WIN32
		for (LoadersList::iterator it = mAlgoLoaders->begin(); it != mAlgoLoaders->end(); ++it)
		{
			AlgorithmLoader *loader = (*it);
			if (loader->handle != NULL)
			{
				dlclose(loader->handle);
			}
			delete loader;
		}

		delete mAlgoLoaders;
		#endif
	}
	void runSimulation(HouseList houses_list, int numThreads);
	void cleanResources();
	int countHousesInPath(string houses_path);
	HouseList readAllHouses(string houses_path);
	AlgorithmList *loadAllAlgorithms(string algorithms_path, bool firstTime);
	string getAlgorithmErrorMessages() const;
	string getHousesErrorMessages() const;
	string getScoreErrorMessage() const;
	void runSimuationOnHouse();
	static int calculateSimulationScore(const map<string, int>& score_params);
private:

	//members:
	LoadersList *mAlgoLoaders;
	HouseList mHouseList;
	string mHousesErrorMessages;
	string mAlgorithmErrorMessages;
	ConfigReader* mConfiguration;
	list<string>* mAlgorithmNames;
	bool mIsAnySimulationScoreBad;
	int mNumThreads;
	atomic_size_t mHouseIndex{0};

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