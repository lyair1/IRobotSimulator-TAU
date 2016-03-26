#include "House.h"

//TODO: make sure number of rows is R
void House::fillHouseInfo(string filePath)
{
  cout << "Reading house from file path: %s into class House" <<filePath << endl;
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
    std::getline(fin, this->matrix[i]);
  }
  initDockingLocation();
  initDustInHouse();
}

void House:: printHouse() const
{
  cout << "Printing house from instance into standard output" << endl;
  cout << "House name: " << getName() << endl;
  cout << "House description: " << getDescription() << endl;
  for (int i = 0; i < getR(); ++i)
  {
    for (int j =0; j < getC() ; ++j)
    {
      cout << matrix[i][j];
    }
    cout << endl;
  }
}

//TODO: comlete this with fill rows and cols if matrix is not in size C*R
	bool House::isLegalHouse(){
		this->R;
		this->C;
		this->matrix;
		for (int i = 0; i < this->R; ++i){
			for (int j =0; j < this->C; ++j){

			
			}
		}
		return true;
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


SensorInformation House::getLocationInfo(std::pair<const int,const int> location) const {
	SensorInformation locationInfo; 
	if (matrix[location.second][location.first] > '0' &&
		matrix[location.second][location.first] <= '9')
	{
		locationInfo.dirtLevel = matrix[location.second][location.first] - '0'; // convert char to int
	}
	else if (matrix[location.second][location.first] == ' ' ||
		matrix[location.second][location.first] == 'D' ||
		matrix[location.second][location.first] == 'W')
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

	void House:: initDockingLocation()
	{
		for (int row = 0; row < getR(); ++row) {
			for (int col = 0; col < getC(); ++col) {
				if (matrix[row][col] == 'D') {
					mDockingLocation.first = col;
					mDockingLocation.second = row; // col == X, row == Y
					return;
				}
			}
		}
		cout << "House:: initDockingLocation() : no docking station was found! this house is Illegal" << endl;
		mDockingLocation.first = -1;// no docking location is found...
		mDockingLocation.second = -1; 
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
		for (int row = 0; row < getR(); ++row) 
		{
			for (int col = 0; col < getC(); ++col) 
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