/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

#define BATTERY_CONSUMPTION_RATE "BatteryConsumptionRate"
#define BATTERY_RECHARGE_RATE "BatteryRechargeRate"
#define BATTERY_CAPACITY "BatteryCapacity"
#define MAX_STEPS_AFTER_WINNER "MaxStepsAfterWinner"
const string PARAMETER_ARRAY[4] = { BATTERY_CONSUMPTION_RATE, BATTERY_RECHARGE_RATE, BATTERY_CAPACITY, MAX_STEPS_AFTER_WINNER };
#define CONFIG_DEBUG 0

class ConfigReader
{
public:
	ConfigReader() : isAllParamteresLegalInConfigFile(false), messgaeOfBadParams(""){}
  ConfigReader(const string& iniPath)
  {
	  isAllParamteresLegalInConfigFile = this->loadFromFile(iniPath, messgaeOfBadParams);
  }
  int getParameter(const string parameter);
  bool isParameterExist(const string parameter);
  bool loadFromFile(const string& iniPath, string & result);
  string toString();
  string getMessageForMissingParamsInConfigFile();
  map<string, int>* getParametersMap();
  bool isAllParamteresExistInConfigFile();
  bool isAllParamteresLegalInConfigFile;
  string getMessageForBadParamsInConfigFile() const;
private:
	/* map<string, int> parameters contains the following: 
	-	MaxSteps:
		An execution of an algorithm on a house will run for at most MaxSteps time units.
		If the algorithm hasn't terminated during this amount of time, we stop running it.

	-	MaxStepsAfterWinner:
		If we run a simulation of several algorithms on a specific house, when the first algorithm finishes,
		we let all other algorithms continue for additional MaxStepsAfterWinner time units.
		After that time we stop running all algorithms which haven't finished yet. Anyhow, we don't run any algorithm more than MaxSteps time units.

	-	BatteryCapacity:
		assuming the battery is fully changed, this is the number of time units a fully charged battery may hold.

	-	BatteryConsumptionRate:
		this is the rate at which the battery is consumed during each time unit the robot is not in its docking station.
		The current battery load is an integer between zero (empty battery) and BatteryCapacity (full-charged).
		Each time unit the robot is not in its docking station, the current battery load is reduced by this number,
		per each time unit the robot is not at the docking station.

	-	BatteryRechargeRate:
		this is the rate at which the battery is charged.
		Each time unit the robot is at the docking station, the current battery load increases by the amount RechargeRate.
	*/
  string messgaeOfBadParams;
  map<string, int> parameters;
  static std::vector<std::string> split(const std::string &s, char delim);
  static std::string trim(std::string& str);
  string processLine(const string& line);
  const int numParameters = 4;
  

  
};

#endif