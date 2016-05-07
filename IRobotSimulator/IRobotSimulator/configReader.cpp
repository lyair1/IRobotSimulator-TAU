/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "configReader.h"

  bool ConfigReader::loadFromFile(const string& iniPath, string & result)
  {
    this->parameters.clear();
	ifstream fin;
	fin.open(iniPath.c_str(), ios::in);
	if (!fin || !fin.is_open())
	{
		std::cout << "config.ini exists in '" << iniPath << "' but cannot be opened\n";
		exit(0);
	}
	int badParams = 0;
	string badParamsStr = "";
	bool firstBad = true;
    string line;
    while (getline(fin, line))
    {
		string parameter = this->processLine(line);
		if (parameter != "")
		{
			badParams++;
			if (firstBad)
			{
				firstBad = false;
			}
			else
			{
				badParamsStr.append(", ");
			}
			badParamsStr.append(parameter);
		}
    }
	if (badParams > 0)
	{
		result.clear();
		result += "config.ini having bad values for " + to_string(badParams) + " parameter(s): " + badParamsStr + "\n";
		return false;
	}
	
	return true;
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

  string ConfigReader::processLine(const string& line)
  {
    vector<string> tokens = split(line, '=');
    if (tokens.size() != 2)
    {
		return ""; // this line is illegal, don't return the parameter name.
    }
	string parameterName = trim(tokens[0]); 
	bool isGoodParam = false;
	for (int i = 0; i < numParameters ; i++)
	{
		if (parameterName == PARAMETER_ARRAY[i])
		{
			isGoodParam = true;
			break;
		}
	}
	if (!isGoodParam)
	{
		return ""; // this parameter is not one of the legal parametes. don't return the parameter name.
	}
	string parameterValue = trim(tokens[1]);
	istringstream in(parameterValue);
	int parameterInt = -1;
	if (in >> parameterInt && in.eof())
	{
		if (parameterInt >= 0)
		{
			this->parameters[parameterName] = parameterInt;
			return ""; // this parameter is inserted into the map with no error.
		}
	}
	//parameter is negative or is not a number
	this->parameters[parameterName] = 0; //enter the parameter into the map anyway (beacuse it's not a missing parameter)
	return parameterName;  // but return its string since it's a bad parameter.
  }

  bool ConfigReader::isParameterExist(const string parameter){
	  auto pos = parameters.find(parameter);
	  if (pos == parameters.end()){
		  return false;
	  }
	  return true;
  }

int ConfigReader::getParameter(const string parameter){
	if (isParameterExist(parameter))
		{
			return this->parameters.find(parameter)->second;
		}
		 return -1;
	}


	map<string, int>* ConfigReader::getParametersMap(){
		return &parameters;
	}


	bool ConfigReader ::isAllParamteresExistInConfigFile()
	{
		for (int i = 0; i < numParameters; i++)
		{
			if (!isParameterExist(PARAMETER_ARRAY[i]))
			{
				return false;
			}
		}
		return true;
	}

	string ConfigReader::getMessageForMissingParamsInConfigFile(){
		int missingParams = 0;
		string missingParamsStr = "";
		bool firstMissing = true;
		for (int i = 0; i < numParameters; i++)
		{
			string param = PARAMETER_ARRAY[i];
			if (!isParameterExist(param))
			{
				missingParams++;
				if (firstMissing)
				{
					firstMissing = false;
				}
				else
				{
					missingParamsStr.append(", ");
				}

				missingParamsStr.append(param);
			}
		}
		
		return "config.ini missing " + to_string(missingParams) + " parameter(s): " + missingParamsStr + "\n";
		
	}

	string ConfigReader::getMessageForBadParamsInConfigFile() const
	{
		return messgaeOfBadParams;
	}