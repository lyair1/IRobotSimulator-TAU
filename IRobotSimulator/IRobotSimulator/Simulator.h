/*
Author: Nir Orman ID 201588902
Yair Levi ID 200945657
*/
#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <list>
#include "Simulation.h"
#include "configReader.h"

//the simulator class is responsible for running every algorithm on every house, and keeping track of the algorithms preformance according to time
	typedef std::list<House*> HouseList;
	typedef std::list<AlgorithmNaive*> AlgorithmList;
class Simulator
{
public:
	Simulator()
	{
		mHouseList = new HouseList();
		mAlgorithmList = new AlgorithmList();
	}
	~Simulator(){
		cleanResources();
	}
	void runSimulation(string config_file_path,string houses_path);
	void cleanResources();
	
private:
	HouseList *mHouseList;
	AlgorithmList  *mAlgorithmList;
	vector<Simulation*> simulationVectorPerHouse;
	void readAllHouses(string houses_path);
	void loadAllAlgorithms();
	void executeAllAlgoOnAllHouses();
	ConfigReader* mConfiguration;
};

#endif //SIMULATOR_H