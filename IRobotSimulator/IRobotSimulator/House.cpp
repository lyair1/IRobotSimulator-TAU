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
	mHousePath = filePath;
	mHouseFileName = fileName;
	ifstream fin(filePath);
	string name;
	getline(fin, name);
	mHouseName = name;
	int maxSteps;
	fin >> maxSteps;
	if (maxSteps < 0)
	{
		return mHouseFileName + ".house" + ": line number 2 in house file shall be a positive number, found: " + to_string(maxSteps) + "\n";
	}
	mMaxSteps = maxSteps; 

	fin >> mHouseRow;
	if (mHouseRow <= 0)
	{
		return mHouseFileName + ".house" + ": line number 3 in house file shall be a positive number, found: " + to_string(mHouseRow) + "\n";
	}
	fin >> mHouseCol;
	if (mHouseCol <= 0)
	{
		return mHouseFileName + ".house" + ": line number 4 in house file shall be a positive number, found: " + to_string(mHouseCol) + "\n";
	}

	this->mHouseMatrix = new string[mHouseRow];
	std::getline(fin, this->mHouseMatrix[0]);
	for (int i = 0; i < mHouseRow; ++i)
	{
		// Check if stream is over before the rows count
		if (fin.eof())
		{
			for (int j = 0; j < mHouseCol; ++j)
			{
				mHouseMatrix[i] += " ";
			}

			continue;
		}
		std::getline(fin, this->mHouseMatrix[i]);

		// Check if stream isn't over but this is an empty line
		if (mHouseMatrix[i].length() == 0)
		{
			for (int j = 0; j < mHouseCol; ++j)
			{
				mHouseMatrix[i] += " ";
			}

			continue;
		}

		// Check if a row is shorter than expected
		while ((int)mHouseMatrix[i].length() < mHouseCol)
		{
			mHouseMatrix[i] += " ";
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
	fillHouseInfo(otherHouse.mHousePath, otherHouse.mHouseFileName);
}


void House::printHouse() const
{
	cout << "Printing house from instance into standard output" << endl;
	cout << "House name: " << getName() << endl;
	cout << "House maxSteps: " << getMaxSteps() << endl;
	for (int i = 0; i < mHouseRow; ++i)
	{
		for (int j = 0; j < mHouseCol; ++j)
		{
			cout << mHouseMatrix[i][j];
		}
		cout << endl;
	}
}

//TODO: complete this with fill rows and cols if matrix is not in size C*R
string House::isLegalHouse(){
	// Make sure that that house surounded by walls
	for (int i = 0; i < mHouseRow; ++i){
		mHouseMatrix[i][0] = 'W';
		mHouseMatrix[i][mHouseCol - 1] = 'W';
	}
	for (int i = 0; i < mHouseCol; ++i){
		mHouseMatrix[mHouseRow - 1][i] = 'W';
		mHouseMatrix[0][i] = 'W';
	}

	// Overwrite any unknown chars to ' '
	for (int i = 1; i < mHouseRow - 1; ++i)
	{
		for (int j = 1; j < mHouseCol - 1; ++j)
		{
			if (mHouseMatrix[i][j] != 'D' && mHouseMatrix[i][j] != 'W' && (mHouseMatrix[i][j] < '0' || mHouseMatrix[i][j] > '9'))
			{
				mHouseMatrix[i][j] = ' ';
			}
		}
	}

	initDustInHouse();

	return initDockingLocation();
}

string House::getName() const {
	return mHouseName;
}

int House::getMaxSteps() const {
	return mMaxSteps;
}

void House::cleanResources() const{
	//delete mHouseMatrix;
	// Clean algorithms
	for (AlgorithmList::iterator listAlgorithmIter = mAlgorithmList->begin(); listAlgorithmIter != mAlgorithmList->end(); ++listAlgorithmIter)
	{
		delete *listAlgorithmIter; // this calls the destructor which will call cleanResources. 
	}
}


// 
SensorInformation House::getLocationInfo(std::pair<const int, const int> location) const {
	SensorInformation locationInfo;
	if (mHouseMatrix[location.second][location.first] >= '1' &&
		mHouseMatrix[location.second][location.first] <= '9')
	{
		locationInfo.dirtLevel = mHouseMatrix[location.second][location.first] - '0'; // convert char to int
	}
	else //char which is neither 'D'/' '/'W'/'0'-'9' is considered as ' '. 
	{
		locationInfo.dirtLevel = 0;
	}

	locationInfo.isWall[0] = mHouseMatrix[location.second][location.first + 1] == 'W' ? true : false;
	locationInfo.isWall[1] = mHouseMatrix[location.second][location.first - 1] == 'W' ? true : false;
	locationInfo.isWall[2] = mHouseMatrix[location.second + 1][location.first] == 'W' ? true : false;
	locationInfo.isWall[3] = mHouseMatrix[location.second - 1][location.first] == 'W' ? true : false;
	return locationInfo;
}



string House::initDockingLocation()
{
	bool didFindDocking = false;
	for (int row = 0; row < mHouseRow; ++row) {
		for (int col = 0; col < mHouseCol; ++col) {
			if (mHouseMatrix[row][col] == 'D') {
				if (didFindDocking == true)
				{
					return mHouseFileName + ".house" + ": too many docking stations (more than one D in house)\n";
				}

				didFindDocking = true;
				mDockingLocation.first = col;
				mDockingLocation.second = row; // col == X, row == Y
			}
		}
	}

	if (didFindDocking)
	{
		return "";
	}

	return mHouseFileName + ".house" + ": missing docking station (no D in house)\n";
}

pair <int, int> House::getDockingLocation() const{
	return mDockingLocation;
}

bool House::isDirtCollected(pair<int, int> location){
	//clean the dust if it exists:
	if (mHouseMatrix[location.second][location.first] >= '1' &&
		mHouseMatrix[location.second][location.first] <= '9')
	{
		mHouseMatrix[location.second][location.first] --;
		mDustInHouse--;

		return true;
	}
	return false;
}

char House::getLocationValue(pair<int, int> location) const{
	return mHouseMatrix[location.second][location.first];
}

bool House::isCleanHouse() const{
	return (mDustInHouse == 0);
}

void House::initDustInHouse(){
	mDustInHouse = 0;
	for (int row = 0; row < mHouseRow; ++row)
	{
		for (int col = 0; col < mHouseCol; ++col)
		{
			if (mHouseMatrix[row][col] >= '1' &&
				mHouseMatrix[row][col] <= '9')
			{
				mDustInHouse += (mHouseMatrix[row][col] - '0');
			}
		}
	}
}

int House::getDustInHouse() const{
	return mDustInHouse;
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
	return mHouseFileName;
}
string House::getHousePath() const
{
	return mHousePath;
}