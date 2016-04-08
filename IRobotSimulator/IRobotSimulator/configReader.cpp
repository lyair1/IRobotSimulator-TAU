/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "configReader.h"

  void ConfigReader::loadFromFile(const string& iniPath)
  {
    this->parameters.clear();
    ifstream fin(iniPath.c_str());
    string line;
    while (getline(fin, line))
    {
      this->processLine(line);
    }
	
  }
  
  string ConfigReader::toString()
  {
    string out;
    for (map<string, int>::const_iterator itr = this->parameters.begin();
         itr != this->parameters.end();
         ++itr)
    {
      if (itr !=  this->parameters.begin())
      {
        out += "\n";
      }
      out += "parameters[" + itr->first + "] = " + to_string((long double)itr->second);
    }
    return out;
  }

   std::vector<std::string> ConfigReader::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
  }

   std::string ConfigReader::trim(std::string& str)
  {
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
  }

  void ConfigReader::processLine(const string& line)
  {
    vector<string> tokens = split(line, '=');
    if (tokens.size() != 2)
    {
      return;
    }
    this->parameters[trim(tokens[0])] = stoi(tokens[1]);
  }



int ConfigReader::getParameter(const string parameter){
	 auto pos = this->parameters.find(parameter);
	 if(pos != parameters.end()){
		 return pos->second;
	 }
	 cout << "Error in ConfigReader::getParameter, parameter " << parameter<<" was not found in configuration file"<<endl;
	 //(all resources are released and program terminates) 
	 return -1;

}


map<string, int>* ConfigReader::getParametersMap(){
	return &parameters;
}


bool ConfigReader ::isLegalConfigFile()
{
	if (getParameter("BatteryConsumptionRate") == -1	||
		getParameter("BatteryRechargeRate") == -1		||
		getParameter("BatteryCapacity") == -1			||
		getParameter("MaxStepsAfterWinner") == -1)
	{
		return false;
	}

	return true;
}