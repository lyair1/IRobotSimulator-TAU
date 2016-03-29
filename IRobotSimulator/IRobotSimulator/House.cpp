/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "House.h"

//TODO: make sure number of rows is R
bool House::fillHouseInfo(string filePath)
{
	if (DEBUG)
	{
		cout << "Reading house from file path: " << filePath << "into class House" << endl;
	}
  
  ifstream fin(filePath);
  string name;
  getline(fin, name);
  this->setName(name);
  string desc;
  std::getline(fin, desc);
  this->setDescription(desc);
  int r,c;
  fin >> r;
  fin >> c;
  this->setC(c);
  this->setR(r);
  fin.ignore(); //skip newline and go the begining of matrix
  this->matrix = new string[this->getR()];
  for (int i =0; i < this->getR(); ++i)
  {
	  // Check if stream is over before the rows count
	  if (fin.eof())
	  {
		  cout << "Input error, not enough rows for house description" << endl;

		  return false;
	  }
	  std::getline(fin, this->matrix[i]);

	  // Check if stream isn't over but this is an empty line
	  if (matrix[i].length() == 0)
	  {
		  cout << "Input error, not enough rows for house description" << endl;

		  return false;
	  }

	  // Check if a row is shorter than expected
	  if ((int)matrix[i].length() != C)
	  {
		  cout << "Input error, not enough columns for house description" << endl;

		  return false;
	  }
  }

  return isLegalHouse();
}

void House:: printHouse() const
{
  cout << "Printing house from instance into standard output" << endl;
  cout << "House name: " << getName() << endl;
  cout << "House description: " << getDescription() << endl;
  for (int i = 0; i < R; ++i)
  {
    for (int j =0; j < C ; ++j)
    {
      cout << matrix[i][j];
    }
    cout << endl;
  }
}

//TODO: comlete this with fill rows and cols if matrix is not in size C*R
bool House::isLegalHouse(){
	// Make sure that that house souranded by walls
	for (int i = 0; i < R; ++i){
		matrix[i][0] = 'W';
		matrix[i][C-1] = 'W';
	}
	for (int i = 0; i < C; ++i){
		matrix[R-1][i] = 'W';
		matrix[0][i] = 'W';
	}

	// Overwrite any unknown chars to ' '
	for (int i = 0; i < R; ++i)
	{
		for (int j = 0; j < C; ++j)
		{
			if (matrix[i][j] != 'W' && matrix[i][j] != 'D' && (matrix[i][j] < '0' || matrix[i][j] > '9'))
			{
				matrix[i][j] = ' ';
			}
		}
	}

	initDustInHouse();

	return initDockingLocation();
}

string House::getName() const {
	return name;
}

string House::getDescription() const {
	return desc;
}

int House::getR() const {
	return R;
}

int House::getC() const {
	return C;	
}

void House::cleanResources(){
	
}


// 
SensorInformation House::getLocationInfo(std::pair<const int,const int> location) const {
	SensorInformation locationInfo; 
	if (matrix[location.second][location.first] >= '1' &&
		matrix[location.second][location.first] <= '9')
	{
		locationInfo.dirtLevel = matrix[location.second][location.first] - '0'; // convert char to int
	}
	else //char which is neither 'D'/' '/'W'/'0'-'9' is considered as ' '. 
	{
		locationInfo.dirtLevel = 0;
	}

	locationInfo.isWall[0] = matrix[location.second][location.first + 1] == 'W' ? true : false;
	locationInfo.isWall[1] = matrix[location.second][location.first - 1] == 'W' ? true : false;
	locationInfo.isWall[2] = matrix[location.second + 1][location.first] == 'W' ? true : false;
	locationInfo.isWall[3] = matrix[location.second - 1][location.first] == 'W' ? true : false;
	return locationInfo;
}

void House::setR(int r){
	R = r;
}
void House::setC(int c)
{
	C = c;
}
void House::setName(string houseName)
{
	name = houseName;
}

void House::setDescription(string description)
{
	desc = description;
}

bool House:: initDockingLocation()
{
	bool didFindDocking = false;
	for (int row = 0; row < getR(); ++row) {
		for (int col = 0; col < getC(); ++col) {
			if (matrix[row][col] == 'D') {
				if (didFindDocking == true)
				{
					cout << "House:: initDockingLocation() : more than one docking station was found! this house is Illegal" << endl;

					return false;
				}

				didFindDocking = true;
				mDockingLocation.first = col;
				mDockingLocation.second = row; // col == X, row == Y
			}
		}
	}

	if (didFindDocking)
	{
		return true;
	}

	cout << "House:: initDockingLocation() : no docking station was found! this house is Illegal" << endl;

	return false;
}

pair <int, int> House::getDockingLocation(){
	return mDockingLocation;
}

bool House::isDirtCollected(pair<int, int> location){
	//clean the dust if it exists:
	if (matrix[location.second][location.first] >= '1' &&
		matrix[location.second][location.first] <= '9')
	{
		matrix[location.second][location.first] --;
		mDustInHouse--;

		return true;
	}
	return false;
}

char House::getLocationValue(pair<int, int> location)
{
	return matrix[location.second][location.first];
}

bool House::isCleanHouse(){
	return (mDustInHouse == 0);
}

void House::initDustInHouse(){
	mDustInHouse = 0;
	for (int row = 0; row < R; ++row) 
	{
		for (int col = 0; col < C; ++col) 
		{
			if (matrix[row][col] >= '1' &&
				matrix[row][col] <= '9')
			{
				mDustInHouse += (matrix[row][col] - '0');
			}
		}
	}
}

int House::getDustInHouse(){
	return mDustInHouse;
}