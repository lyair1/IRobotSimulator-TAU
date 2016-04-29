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
using namespace std;

//the simulator class is responsible for running every algorithm on every house, and keeping track of the algorithms preformance according to time
	typedef std::list<House*> HouseList;
	typedef std::list<AbstractAlgorithm*> AlgorithmList;
	typedef std::list<Simulation*> SimulationList;
	typedef std::list<AlgorithmLoader*> LoadersList;
	typedef std::list<list<string>*> StringMatrix;


class Simulator
{
public:
	Simulator(ConfigReader *configuration):
		mHouseList(NULL),
		mHousesErrorMessages(""),
		mAlgorithmErrorMessages("")
	{
		mConfiguration = configuration;
		mAlgorithmList = new AlgorithmList();
		mAlgorithmNames = new list<string>;
	}
	~Simulator(){
	}
	void runSimulation(HouseList* houses_list, AlgorithmList* algorithmsList);
	void cleanResources(AlgorithmList* algo_list);
	int countHousesInPath(string houses_path);
	HouseList* readAllHouses(string houses_path);
	AlgorithmList *loadAllAlgorithms(string algorithms_path);
	string mHousesErrorMessages;
	string mAlgorithmErrorMessages;
private:
	HouseList *mHouseList;
	AlgorithmList  *mAlgorithmList;
	ConfigReader* mConfiguration;
	list<string>* mAlgorithmNames;
	void executeAllAlgoOnAllHouses();
	void printScores();
	string getSupparatorLine();
	string getAlgoPrintLine(int ind, string algoName);
	string getHeaderPrintLine();
	
	struct less_than_key
	{
		inline bool operator() (const AlgorithmLoader *struct1, const AlgorithmLoader *struct2)
		{
			return (struct1->fileName < struct2->fileName);
		}
	};
};

#endif //SIMULATOR_H