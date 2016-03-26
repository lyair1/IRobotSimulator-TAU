#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <list>
#include "Simulation.h"
#include "configReader.h"
#define INI_CONFIG_PATH "config.ini"


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
		if (mHouseList != NULL)	{
			for (HouseList::iterator listHouseIter = mHouseList->begin(); listHouseIter != mHouseList->end(); listHouseIter++)
			{
				delete *listHouseIter;
			}
			delete mHouseList;
		}
		if (mAlgorithmList != NULL)
		{				
			for (AlgorithmList::iterator listAlgorithmIter = mAlgorithmList->begin(); listAlgorithmIter != mAlgorithmList->end(); listAlgorithmIter++)
			{
				delete *listAlgorithmIter;
			}
			mAlgorithmList;
		}
	}
	void runSimulation(char * config_file_path,char* houses_path);
	
private:
	HouseList *mHouseList;
	AlgorithmList  *mAlgorithmList;
	void readAllHouses(char* houses_path);
	void loadAllAlgorithms();
	void executeAllAlgoOnAllHouses();
	ConfigReader* mConfiguration;
};

#endif //SIMULATOR_H