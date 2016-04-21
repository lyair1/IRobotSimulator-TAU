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
using namespace std;

//the simulator class is responsible for running every algorithm on every house, and keeping track of the algorithms preformance according to time
	typedef std::list<House*> HouseList;
	typedef std::list<AbstractAlgorithm*> AlgorithmList;
	typedef std::list<Simulation*> SimulationList;
	typedef std::list<list<string>*> StringMatrix;

class Simulator
{
public:
	Simulator()
	{
		mAlgorithmList = new AlgorithmList();
		mConfiguration = NULL;
		housesErrorMessages = "";
		mAlgorithmNames = new list<string>;
	}
	~Simulator(){
	}
	void runSimulation(ConfigReader *configuration, HouseList* houses_list, string algorithms_path);
	void cleanResources();
	int countHousesInPath(string houses_path);
	HouseList* readAllHouses(string houses_path);
	string housesErrorMessages;
private:
	HouseList *mHouseList;
	AlgorithmList  *mAlgorithmList;
	ConfigReader* mConfiguration;
	list<string>* mAlgorithmNames;
	void loadAllAlgorithms(string algorithms_path);
	void executeAllAlgoOnAllHouses();
	void printScores();
	string getSupparatorLine();
	string getAlgoPrintLine(int ind, string algoName);
	string Simulator::getHeaderPrintLine();
};

#endif //SIMULATOR_H