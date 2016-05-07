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

void createExampleHouse(const string filePath);
void writeConfigFile(const string iniPath);
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
			score_formula_path = _pathPrefix + argv[i + 1] + _seperator;
			// Show usage and return if score file doesn't exists in path
			if (!isFileExists(score_formula_path))
			{
				cout << _usage;
				cout << "cannot find score_formula.so file in '" << score_formula_path.substr(2) << "'" << endl;
				exit(0);
			}
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
		cout << "cannot find config.ini file in '" << config_file_path << "'" << endl;
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
		std::cout << "All houses files in target folder '" << houses_path << "' cannot be opened or are invalid:\n" << simul.getHousesErrorMessages();
		exit(0);
	}

	AlgorithmList* algo_list = simul.loadAllAlgorithms(algorithms_path);

	if (algo_list == nullptr)
	{
		if (simul.getAlgorithmErrorMessages().length() > 0)
		{
			cout << "All algorithm files in target folder '" << algorithms_path << "' cannot be opened or are invalid: \n" << simul.getAlgorithmErrorMessages();
		}

		exit(0);
	}

	// Check if all algorithms are invalid
	if (houses_list->empty())
	{
		cout << "All algorithm files in target folder '" << algorithms_path << "' cannot be opened or are invalid: \n" << simul.getAlgorithmErrorMessages();
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

	// Only on windows
	#if defined (_WIN32)
		system("pause");
	#endif

	return 0;
}


void createExampleHouse(const string filePath)
{
	if (DEBUG)
	{
		cout << "Creating default house file" << endl;
	}
  
	ofstream fout(filePath + _defaultHosuseFileName);
	fout << "Simple1" << endl;
	fout << 100 << endl;

/*
  fout << 8 << endl;
  fout << 10 << endl;
  fout << "WWWWWWWWWW" << endl;
  fout << "W22  DW59W" << endl;
  fout << "W  W 1119W" << endl;
  fout << "W WWW3WW W" << endl;
  fout << "W6   3W  W" << endl;
  fout << "W78W  W  W" << endl;
  fout << "W99W  W  W" << endl;
  fout << "WWWWWWWWWW" << endl;
*/  

  fout << 19 << endl;
  fout << 80 << endl;
  //                 1         2         3         4         5         6         7        
  //       01234567890123456789012345678901234567890123456789012345678901234567890123456789
  fout << "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" << endl; // 0
  fout << "W  99   D              1234321                                                 W" << endl; // 1
  fout << "W  99      WWWWWWW     1234321                     W                       1   W" << endl; // 2
  fout << "W              W                                   W   555                 2   W" << endl; // 3
  fout << "W              W                                   W   555                 3   W" << endl; // 4
  fout << "W              W           WWWWWWWWWWWWWWWWWWWWWWWWW                       4   W" << endl; // 5
  fout << "W              W                                                           5   W" << endl; // 6
  fout << "W              W                                                           6   W" << endl; // 7
  fout << "W                          WWWWWWWWWWWWWWWWWWWWWW  WWWWWWW                 7   W" << endl; // 8
  fout << "W         1         2         3         4         5W 999 W  6         7        W" << endl; // 9
  fout << "W              W           444                     W 999 W                 9   W" << endl; // 10
  fout << "W              W           444                     W 999 W                 8   W" << endl; // 11
  fout << "W              W                                   W     W                 7   W" << endl; // 12
  fout << "W              W                                   WW   WW                 6   W" << endl; // 13
  fout << "W              W                                    W   W                  5   W" << endl; // 14
  fout << "W              W                                                           4   W" << endl; // 15
  fout << "W              W                                                           3   W" << endl; // 16
  fout << "W              W                                                               W" << endl; // 17
  fout << "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" << endl; // 18
 
  fout.close();
}

void writeConfigFile(const string iniPath)
{
	//write info into configuration file:
  ofstream fout(iniPath.c_str());
  fout << "BatteryConsumptionRate  =    1" << endl;
  fout << "MaxSteps   = 1200" << endl;
  fout << "MaxStepsAfterWinner=200" << endl;
  fout << "BatteryCapacity=400" << endl;
  fout << "BatteryRechargeRate=20" << endl;
  fout.close();
	//print the information from the configuration file:
  ifstream fin(iniPath.c_str());
  string line;
  cout << "Original config ini file: " << endl;
  while (getline(fin, line))
  {
    cout << line << endl;
  }
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
