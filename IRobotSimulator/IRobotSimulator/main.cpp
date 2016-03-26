#include <iostream>
#include <string>
#include <windows.h>
#include <time.h>
#include "Simulator.h"

void createExampleHouse(const string filePath);
void writeConfigFile(const string iniPath);

const string _defaultConfigFileName = "config.ini";
const string _defaultHosuseFileName = "default_generated_house.house";
const string _pathPrefix = "./";

int main(int argc, char* argv[])
{
	if (argc > 5){
		// Inform the user of how to use the program:
		std::cout << "Usage is -config <config_file_location> -house_path<houses_path_location>\n"; 
		std::cin.get();
		exit(0);
	}

	string config_file_path = _pathPrefix + _defaultConfigFileName, houses_path = _pathPrefix;
	for (int i = 1; i < argc; i++){ //skip program name -> i=1
		if (i + 1 != argc){
			// Check that we haven't finished parsing already
			if (strcmp(argv[i],"-config")==0) {
				// We know the next argument *should* be the filename:
				config_file_path = argv[i + 1];
			}
			else if (strcmp(argv[i], "-house_path") == 0) {
				houses_path = argv[i + 1];
			}
		} 
	}
	//TODO in LINUX: check if file exists in config_file_path. if NOT:
	writeConfigFile(config_file_path);
	//TODO: update config_file_path = INI_CONFIG_PATH;
	//TODO in LINUX: check if a *.house file exists in houses_path. if NOT:
	createExampleHouse(houses_path);
	//TODO: make sure this works for path that end with '/' and without it. example: /usr/targil1/simufiles/	OR  	/usr/targil1/simufiles
	srand ((unsigned int)time(NULL));
    Simulator simul;
	simul.runSimulation(config_file_path, houses_path);

	system("pause");
	return 0;
}


void createExampleHouse(const string filePath)
{
  cout << "Creating default house file" << endl;
  ofstream fout(filePath + _defaultHosuseFileName);
  fout << "Simple1" << endl;
  fout << "2 Bedrooms + Kitchen Isle" << endl;
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
