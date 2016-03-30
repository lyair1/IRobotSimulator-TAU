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
const string _pathPrefix = "./";
const string _seperator = "/";
const string _config = "-config";

int main(int argc, char* argv[])
{
	if (argc > 3){
		// Inform the user of how to use the program:
		//std::cout << "Usage is -config <config_file_location> -house_path<houses_path_location>\n"; 
		std::cout << "Usage is -config <config_file_location>\n"; 
		std::cin.get();
		exit(0);
	}

	string config_file_path = _pathPrefix + _defaultConfigFileName;
	string houses_path = _pathPrefix;
	for (int i = 1; i < argc-1; i++){ //skip program name -> i=1
		string arg = argv[i];
		if (arg.compare(_config) == 0) {
			// We know the next argument *should* be the filename:
			config_file_path = _pathPrefix + argv[i + 1] + _seperator + _defaultConfigFileName;
		}
		/*
		else if (strcmp(argv[i], "-house_path") == 0) {
			houses_path = argv[i + 1];
		}*/
	}

	// Temp for ex_1 - create default config file if config file doesn't exists in path
	if (!isFileExists(config_file_path))
	{
		config_file_path = _pathPrefix + _defaultConfigFileName;
		if (!isFileExists(config_file_path))
		{
			if (DEBUG){
				writeConfigFile(config_file_path);
			}
			else{
				std::cout << "Can't find config file in path \n";

				return 0;
			}
		}
	}

	//TODO: update config_file_path = INI_CONFIG_PATH;
	//TODO in LINUX: check if a *.house file exists in houses_path. if NOT:
	createExampleHouse(houses_path);
	//TODO: make sure this works for path that end with '/' and without it. example: /usr/targil1/simufiles/	OR  	/usr/targil1/simufiles

	srand ((unsigned int)time(NULL)); // this is for the seed to be initialized only once, for the random algorithm
	//set the new_handler for handling cases where "new" failed to allocate memory
	std::set_new_handler(outOfMemHandler);

    Simulator simul;
	simul.runSimulation(config_file_path, houses_path);

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
	fout << "2 Bedrooms + Kitchen Isle" << endl;

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
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}


// function to call if operator new can't allocate enough memory or error arises
void outOfMemHandler()
{
	std::cerr << "Unable to satisfy request for memory\n";
	exit(1);
}
