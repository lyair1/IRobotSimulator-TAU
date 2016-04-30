/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "House.h"



string House::fillHouseInfo(string filePath, string fileName)
{

	if (DEBUG)
	{
		cout << "Reading house from file path: " << filePath << " into class House" << endl;
	}
	_housePath = filePath;
	_houseFileName = fileName;
	ifstream fin(filePath);
	string name;
	getline(fin, name);
	_name = name;
	int maxSteps;
	fin >> maxSteps;
	if (maxSteps < 0)
	{
		return _housePath + ": line number 2 in house file shall be a positive number, found: " + to_string(maxSteps) + "\n";
	}
	_maxSteps = maxSteps; 

	fin >> _R;
	if (_R <= 0)
	{
		return _housePath + ": line number 3 in house file shall be a positive number, found: " + to_string(_R) + "\n";
	}
	fin >> _C;
	if (_C <= 0)
	{
		return _housePath + ": line number 4 in house file shall be a positive number, found: " + to_string(_C) + "\n";
	}

	//fin.ignore(); //skip newline and go the begining of matrix
	
	this->_matrix = new string[_R];
	std::getline(fin, this->_matrix[0]);
	for (int i = 0; i < _R; ++i)
	{
		// Check if stream is over before the rows count
		if (fin.eof())
		{
			for (int j = 0; j < _C; ++j)
			{
				_matrix[i] += " ";
			}
			continue;
		}
		std::getline(fin, this->_matrix[i]);

		// Check if stream isn't over but this is an empty line
		if (_matrix[i].length() == 0)
		{
			for (int j = 0; j < _C; ++j)
			{
				_matrix[i] += " ";
			}
			continue;
		}

		// Check if a row is shorter than expected
		while ((int)_matrix[i].length() < _C)
		{
			_matrix[i] += " ";
		}
	}

	return isLegalHouse();
}

//copy constructor for house:
House::House(const House & otherHouse)
{
	if (DEBUG)
	{
		cout << "copy constructor for house" << endl;
	}
	fillHouseInfo(otherHouse._housePath, otherHouse._houseFileName);
}


void House::printHouse() const
{
	cout << "Printing house from instance into standard output" << endl;
	cout << "House name: " << getName() << endl;
	cout << "House maxSteps: " << getMaxSteps() << endl;
	for (int i = 0; i < _R; ++i)
	{
		for (int j = 0; j < _C; ++j)
		{
			cout << _matrix[i][j];
		}
		cout << endl;
	}
}

//TODO: complete this with fill rows and cols if matrix is not in size C*R
string House::isLegalHouse(){
	// Make sure that that house surounded by walls
	for (int i = 0; i < _R; ++i){
		_matrix[i][0] = 'W';
		_matrix[i][_C - 1] = 'W';
	}
	for (int i = 0; i < _C; ++i){
		_matrix[_R - 1][i] = 'W';
		_matrix[0][i] = 'W';
	}

	// Overwrite any unknown chars to ' '
	for (int i = 1; i < _R - 1; ++i)
	{
		for (int j = 1; j < _C - 1; ++j)
		{
			if (_matrix[i][j] != 'D' && _matrix[i][j] != 'W' && (_matrix[i][j] < '0' || _matrix[i][j] > '9'))
			{
				_matrix[i][j] = ' ';
			}
		}
	}

	initDustInHouse();

	return initDockingLocation();
}

string House::getName() const {
	return _name;
}

int House::getMaxSteps() const {
	return _maxSteps;
}

int House::getR() const {
	return _R;
}

int House::getC() const {
	return _C;
}

void House::cleanResources() const{
	//delete _matrix;
}


// 
SensorInformation House::getLocationInfo(std::pair<const int, const int> location) const {
	SensorInformation locationInfo;
	if (_matrix[location.second][location.first] >= '1' &&
		_matrix[location.second][location.first] <= '9')
	{
		locationInfo.dirtLevel = _matrix[location.second][location.first] - '0'; // convert char to int
	}
	else //char which is neither 'D'/' '/'W'/'0'-'9' is considered as ' '. 
	{
		locationInfo.dirtLevel = 0;
	}

	locationInfo.isWall[0] = _matrix[location.second][location.first + 1] == 'W' ? true : false;
	locationInfo.isWall[1] = _matrix[location.second][location.first - 1] == 'W' ? true : false;
	locationInfo.isWall[2] = _matrix[location.second + 1][location.first] == 'W' ? true : false;
	locationInfo.isWall[3] = _matrix[location.second - 1][location.first] == 'W' ? true : false;
	return locationInfo;
}



string House::initDockingLocation()
{
	bool didFindDocking = false;
	for (int row = 0; row < _R; ++row) {
		for (int col = 0; col < _C; ++col) {
			if (_matrix[row][col] == 'D') {
				if (didFindDocking == true)
				{
					return _housePath + ": too many docking stations/n";
				}

				didFindDocking = true;
				_dockingLocation.first = col;
				_dockingLocation.second = row; // col == X, row == Y
			}
		}
	}

	if (didFindDocking)
	{
		return "";
	}

	return _housePath + ": missing docking station/n";
}

pair <int, int> House::getDockingLocation() const{
	return _dockingLocation;
}

bool House::isDirtCollected(pair<int, int> location){
	//clean the dust if it exists:
	if (_matrix[location.second][location.first] >= '1' &&
		_matrix[location.second][location.first] <= '9')
	{
		_matrix[location.second][location.first] --;
		_dustInHouse--;

		return true;
	}
	return false;
}

char House::getLocationValue(pair<int, int> location) const{
	return _matrix[location.second][location.first];
}

bool House::isCleanHouse() const{
	return (_dustInHouse == 0);
}

void House::initDustInHouse(){
	_dustInHouse = 0;
	for (int row = 0; row < _R; ++row)
	{
		for (int col = 0; col < _C; ++col)
		{
			if (_matrix[row][col] >= '1' &&
				_matrix[row][col] <= '9')
			{
				_dustInHouse += (_matrix[row][col] - '0');
			}
		}
	}
}

int House::getDustInHouse() const{
	return _dustInHouse;
}

string getDirectionString(Direction direction)
{
	switch (direction){
	case Direction::East:
		return "East";
	case Direction::West:
		return "West";
	case Direction::North:
		return "North";
	case Direction::South:
		return "South";
	case Direction::Stay:
		return "Stay";
	default:
		return "";
	}
}


string House::getHouseFileName() const
{
	return _houseFileName;
}
string House::getHousePath() const
{
	return _housePath;
}