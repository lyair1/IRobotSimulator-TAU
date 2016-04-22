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
	Simulator(ConfigReader *configuration)
	{
		mConfiguration = configuration;
		mAlgorithmList = new AlgorithmList();
		housesErrorMessages = "";
		algorithmErrorMessages = "";
		mAlgorithmNames = new list<string>;
	}
	~Simulator(){
	}
	void runSimulation(HouseList* houses_list, AlgorithmList* algorithmsList);
	void cleanResources();
	int countHousesInPath(string houses_path);
	HouseList* readAllHouses(string houses_path);
	AlgorithmList *loadAllAlgorithms(string algorithms_path);
	string housesErrorMessages;
	string algorithmErrorMessages;
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
};

#endif //SIMULATOR_H