/**
	Advanced Topics In Computer Programming - iRobot Project
	Ex 1

	@author Nir Orman ID 201588902
	@author Yair Levi ID 200945657
	@version 1.0 30/1/16
*/
#include <iostream>
#include <string>
#include <ctime>
#include "Simulator.h"


void handleScoreSO(const string &score_formula_path);
bool loadScoreSO(const string &score_formula_path);
bool isFileExists(const string name);
void outOfMemHandler();

const string _defaultConfigFileName = "config.ini";
const string _defaultHosuseFileName = "default_generated_house.house";
const string _defaultScoreFormulaFileName = "score_formula.so";
const string _pathPrefix = "./";
const string _seperator = "/";
const string _paramConfig = "-config";
const string _paramHouse = "-house_path";
const string _paramAlgorithm = "-algorithm_path";
const string _paramScoreFormula = "-score_formula";
const string _paramThreads = "-threads";
const string _usage = USAGE;

int main(int argc, char* argv[])
{
	// Set default paramters:
	string houses_path = _pathPrefix;
	string algorithms_path = _pathPrefix;
	string config_file_path = _pathPrefix + _defaultConfigFileName;
	string score_formula_path = _pathPrefix + _defaultScoreFormulaFileName;
	int num_threads = 1;
	// Get parameters from arg
	for (int i = 1; i < argc-1; i++){ //skip program name -> i=1
		string arg = argv[i];
		if (arg.compare(_paramConfig) == 0) {
			// We know the next argument *should* be the config file dir:
			config_file_path = _pathPrefix + argv[i + 1] + _seperator + _defaultConfigFileName;

			continue;
		}

		if (arg.compare(_paramHouse) == 0) {
			// We know the next argument *should* be the path:
			houses_path = _pathPrefix + argv[i + 1] + _seperator;

			continue;
		}

		if (arg.compare(_paramAlgorithm) == 0) {
			// We know the next argument *should* be the path:
			algorithms_path = _pathPrefix + argv[i + 1] + _seperator;

			continue;
		}

		if (arg.compare(_paramScoreFormula) == 0) {
			// We know the next argument *should* be the path:
			score_formula_path = _pathPrefix + argv[i + 1] + _seperator + _defaultScoreFormulaFileName;
			handleScoreSO(score_formula_path);
			
			continue;
		}
		if (arg.compare(_paramThreads) == 0) {
			// We know the next argument *should* be an integer:
			istringstream in(argv[i + 1]);
			if (in >> num_threads && in.eof())
			{
				continue;
			}
			else
			{
				if (DEBUG)
				{
					cout << "number of threads should be an integer larger than 0!" << endl;
				}
				std::cout << _usage;
				exit(0);
			}
		}

	}

	// Show usage and return if config file doesn't exists in path
	if (!isFileExists(config_file_path))
	{
		cout << _usage;
		cout << "cannot find config.ini file in '" << config_file_path.substr(2) << "'" << endl;
		exit(0);
	}

	// check if config file can be loaded and if values are missing
	ConfigReader *configReader = new ConfigReader(config_file_path);
	if (!configReader->isAllParamteresLegalInConfigFile)
	{
		cout << configReader->getMessageForBadParamsInConfigFile();
	}
	if (!configReader->isAllParamteresExistInConfigFile())
	{
		cout<< configReader->getMessageForMissingParamsInConfigFile();
	}
	if (!configReader->isAllParamteresLegalInConfigFile || !configReader->isAllParamteresExistInConfigFile())
	{
		delete configReader;
		exit(0);
	}


	//set the new_handler for handling cases where "new" failed to allocate memory
	std::set_new_handler(outOfMemHandler);

	Simulator simul = Simulator(configReader);

	// Print usage and return if there are no houses in the path
	if (simul.countHousesInPath(houses_path) == 0)
	{
		if (DEBUG)
		{
			cout << "no houses in path \n";
		}
		std::cout << _usage;
		exit(0);
	}

	// Print message and return if all houses in path are invalid
	HouseList* houses_list = simul.readAllHouses(houses_path);
	// Check if all houses are invalid
	if (houses_list->empty())
	{
		std::cout << "All houses files in target folder '" << houses_path.substr(2) << "' cannot be opened or are invalid:\n" << simul.getHousesErrorMessages();
		exit(0);
	}

	AlgorithmList* algo_list = simul.loadAllAlgorithms(algorithms_path);

	if (algo_list == nullptr)
	{
		if (simul.getAlgorithmErrorMessages().length() > 0)
		{
			cout << "All algorithm files in target folder '" << algorithms_path.substr(2) << "' cannot be opened or are invalid: \n" << simul.getAlgorithmErrorMessages();
		}

		exit(0);
	}

	// Check if all algorithms are invalid
	if (houses_list->empty())
	{
		cout << "All algorithm files in target folder '" << algorithms_path.substr(2) << "' cannot be opened or are invalid: \n" << simul.getAlgorithmErrorMessages();
		exit(0);
	}

	simul.runSimulation(houses_list, algo_list);


	// Print error list
	if (simul.getHousesErrorMessages().length() > 0)
	{
		std::cout << "\nErrors:\n" << simul.getHousesErrorMessages() << simul.getAlgorithmErrorMessages();
	}

	simul.cleanResources();
	
	delete houses_list;
	delete configReader;
	delete algo_list;

	return 0;
}


bool isFileExists(const string name) {
	if (ifstream(name)){
		return true;
	}

	return false;
}


// function to call if operator new can't allocate enough memory or error arises
void outOfMemHandler()
{
	std::cerr << "Unable to satisfy request for memory\n";
	exit(1);
}


void handleScoreSO(const string &score_formula_path)
{
	// Show usage and return if score file doesn't exists in path
	if (!isFileExists(score_formula_path))
	{
		cout << _usage;
		cout << "cannot find score_formula.so file in '" << score_formula_path.substr(2) << "'" << endl;
		exit(0);
	}
	ifstream fin;
	fin.open(score_formula_path.c_str(), ios::in);
	if (!fin || !fin.is_open())
	{
		std::cout << "score_formula.so exists in '" << score_formula_path.substr(2) << "' but cannot be opened or is not a valid.so\n";
		exit(0);
	}
	loadScoreSO(score_formula_path);

}


bool loadScoreSO(const string & score_formula_path)
{
	typedef int (*instanceCreator)();
	// Opening the .so file:
	#ifndef _WIN32
	void *handle = dlopen(score_formula_path.c_str(), RTLD_NOW);
	if (handle == NULL)
	{
		cout << "score_formula.so exists in '" << score_formula_path.substr(2) << "' but cannot be opened or is not a valid .so" << endl;
		return false;
	}

	// getScore is the instance creator method
	void* p = dlsym(handle, "calc_score");
	instanceCreator function1 = reinterpret_cast<instanceCreator>(reinterpret_cast<long>(p));
	if (function1 == nullptr) {
		cout << score_formula_path + ": valid .so file but no score_formula was registered after loading it";
		return false;
	}
	
	
	cout << "nir"<< endl;
	cout << function1();
	//test:
	//const map<string, int> score_params;
	//int score_result = function1(score_params);
	
	return true;
#else
	
	return true;
#endif
}