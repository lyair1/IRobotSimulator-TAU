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
#include <functional>
#include "Encoder.h"


#ifndef _WIN32
#include <dlfcn.h>

#endif
#include <atomic>
using namespace std;

int calculateSimulationScore(const map<string, int>& score_params);
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
	
	Simulator(Simulator const &) = delete;			// don't implement - it's a singleton!
	void operator = (Simulator const &) = delete;	// don't implement - it's a singleton!
	Simulator(bool scoreFormulaReceived, string scoreFormulaPath, int numThreads, string housesPath, string algorithmsPath, string configFilePath, bool isVideo);
	~Simulator();
	void initSimulator();
	static Simulator& getInstance(bool scoreFormulaReceived, string scoreFormulaPath, int numThreads, string housesPath, string algorithmsPath, string configFilePath, bool isVideo)
	{ 
		static Simulator simulator(scoreFormulaReceived, scoreFormulaPath, numThreads, housesPath, algorithmsPath, configFilePath, isVideo);
		return simulator;
	}
	list <unique_ptr<AbstractAlgorithm>> mAlgorithms;
private:

	//members:
	string mHousesPath;
	string mAlgorithmsPath;
	string mConfigFilePath;
	string mScoreFormulaPath;
	HouseList mHouseList;
	string mHousesErrorMessages;
	string mAlgorithmErrorMessages;
	string mSimulationErrorMessages;
	void* mScoreHandle;
	ConfigReader* mConfiguration;
	list<string> mAlgorithmNames;
	list<string> mHouseNames;
	bool mIsAnySimulationScoreBad;
	int mNumThreads;
	bool mScoreFormulaReceived;
	bool mIsVideo;
	atomic_size_t mHouseIndex{0};
	scoreCreator mCalculateScore;

	

	//functions:
	void runSimulation();
	void runSimuationOnHouse();
	void readAllHouses();
	void loadAllAlgorithms();
	void executeAllAlgoOnAllHouses();
	void printScores();

	string getSupparatorLine();
	string getAlgoPrintLine(int ind, string algoName, double & averageResult);
	string getHeaderPrintLine();
	
	string getAlgorithmErrorMessages() const;
	string getHousesErrorMessages() const;
	string getScoreErrorMessage() const;
	string getSimulationsErrorMessage() const;

	void handleConfigFileErrors();
	void handleScore();

	void cleanResources();

};

#endif //SIMULATOR_H