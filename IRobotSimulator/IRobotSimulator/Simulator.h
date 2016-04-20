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

class Simulator
{
public:
	Simulator()
	{
		mHouseList = new HouseList();
		mAlgorithmList = new AlgorithmList();
		mConfiguration = NULL;
		
	}
	~Simulator(){
		cleanResources();
	}
	void runSimulation(ConfigReader *configuration, string houses_path, string algorithms_path);
	void cleanResources();
	
private:
	HouseList *mHouseList;
	AlgorithmList  *mAlgorithmList;
	ConfigReader* mConfiguration;
	void readAllHouses(string houses_path);
	void loadAllAlgorithms(string algorithms_path);
	void executeAllAlgoOnAllHouses();
};

#endif //SIMULATOR_H