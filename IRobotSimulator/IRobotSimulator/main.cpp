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



bool isFileExists(const string name);
void outOfMemHandler();

//absolute path starts with "/"
// relative path does not start with a "/"
//when we need to print an absolute path : if the given path does not start with "/" then it's a relative path we need to complete it 
const string _defaultConfigFileName = "config.ini";

const string _pathPrefix = "./";
const string _seperator = "/";
const string _paramConfig = "-config";
const string _paramHouse = "-house_path";
const string _paramAlgorithm = "-algorithm_path";
const string _paramScoreFormula = "-score_formula";
const string _paramThreads = "-threads";
const string _paramVideo = "-video";
const string _usage = USAGE;

int main(int argc, char* argv[])
{
	// Set default paramters:
	string housesPath = _pathPrefix;
	string algorithmsPath = _pathPrefix;
	string configFilePath = _pathPrefix + _defaultConfigFileName;
	string scoreFormulaPath = "";
	int num_threads = 1;
	bool isVideo = false;
	bool scoreFormulaReceived = false;
	// Get parameters from arg
	for (int i = 1; i < argc; i++){ //skip program name -> i=1
		string arg = argv[i];
		if (arg.compare(_paramConfig) == 0) {
			// We know the next argument *should* be the config file dir:
			if (i + 1 <= argc)
			{
				configFilePath = argv[i + 1] + _seperator + _defaultConfigFileName;
				continue;
			}
		}
		if (arg.compare(_paramHouse) == 0) {
			// We know the next argument *should* be the path:
			if (i + 1 <= argc)
			{
				housesPath = argv[i + 1] + _seperator;
				continue;
			}		
		}

		if (arg.compare(_paramAlgorithm) == 0) {
			// We know the next argument *should* be the path:
			if (i + 1 <= argc)
			{
				algorithmsPath = argv[i + 1] + _seperator;
				continue;
			}			
		}
		if (arg.compare(_paramScoreFormula) == 0) {
			// We know the next argument *should* be the path:
			if (i + 1 <= argc)
			{
				scoreFormulaPath = argv[i + 1] + _seperator;
				scoreFormulaReceived = true;
				continue;
			}
		}
		if (arg.compare(_paramThreads) == 0) {
			if (i + 1 <= argc)
			{
				// We know the next argument *should* be an integer:
				istringstream in(argv[i + 1]);
				if (in >> num_threads && num_threads > 0)
				{
					continue; // it's an integer larger than 0, read next parameters
				}
				else
				{/*If this parameter is missing or is not a valid number (e.g. negative or zero)
				 the number of threads would be 1, without any error message.*/
					num_threads = 1;
					if (DEBUG)
					{
						cout << "number of threads should be a positive integer! default is set to 1" << endl;
					}
				}
			}
		}
		if (arg.compare(_paramVideo) == 0)
		{
			isVideo = true;
		}
	}

	// Show usage and return if config file doesn't exists in path
	if (!isFileExists(configFilePath))
	{
		cout << _usage;
		cout << "cannot find config.ini file in '" << configFilePath << "'" << endl;
		exit(0);
	}

	//set the new_handler for handling cases where "new" failed to allocate memory
	std::set_new_handler(outOfMemHandler);
	Simulator::getInstance(scoreFormulaReceived, scoreFormulaPath, num_threads, housesPath, algorithmsPath, configFilePath, isVideo).initSimulator();

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
