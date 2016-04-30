/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef HOUSE_H
#define HOUSE_H
#define DEBUG 1
#define DEBUG_LOW_LEVEL 0
#include <iostream>
#include <string>
#include <fstream>
#include "SensorInformation.h"
#include "Direction.h"
#include <list>
using namespace std;
#define HOUSE_EXT ".house"
#define USAGE "Usage: simulator [­config <config path>] [­house_path <house path>] [­algorithm_path <algorithm path>]\n"

/*	W – wall
	D – Docking Station(start position and battery recharge)
	number 1 to 9 – dust level
	empty square – no dust
	In this example the house has:
	8 rows(so R = 8)
	10 columns(so C = 10)
	A house description is stored as a file, as following :
	Line 1 : House / description
	Line 2 : Max Steps
	Line 3 : R(number of rows in the house, in our example 8)
	Line 4 : C(number of columns in the house, in our example 10)
	Lines 5 to R + 4 : a matrix representing the house, where :
	W - represents wall,
	D - the docking station, 
	1 - 9 the dust level,
	space / 0 – represents 'no dust'.
*/

class House
{
public:

	House() : 
		_name(""),
		_maxSteps(0),
		_R(0),
		_C(0),
		_dustInHouse(0),
		_dockingLocation(-1, -1),
		_housePath(""),
		_houseFileName(""),
		_matrix(NULL)
		
	{
		algorithmScores = new list<int>();
	}

	//copy constructor for house:
	House(const House & otherHouse);

	//copy assignment operator

	House & operator = (const House & otherHouse)
	{
		if (DEBUG)
		{
			cout << "operator \"=\" for house" << endl;
		}
		House * house = new House();
		house->fillHouseInfo(otherHouse.getHousePath(), otherHouse.getHouseFileName());
		return *house;
	}
	~House(){
		if (_matrix != NULL)	{
			delete[] _matrix;
		}
		delete algorithmScores;
	}
	string getName()const;
	int getMaxSteps()const;
	int getR() const;
	int getC()const;
	SensorInformation getLocationInfo(std::pair<const int, const int> location)const;
	string isLegalHouse();
	void printHouse()const;
	string fillHouseInfo(string filePath, string fileName);
	bool isDirtCollected(pair<int, int> location);
	char getLocationValue(pair<int, int> location) const;
	bool isCleanHouse() const;
	pair <int, int> getDockingLocation() const;
	int getDustInHouse() const;
	void cleanResources()const;
	list<int>* algorithmScores;
	string getHouseFileName() const;
	string getHousePath() const;
private:
	// members:
	string _name;
	int _maxSteps;
	int _R;
	int _C;
	int _dustInHouse;
	pair <int, int> _dockingLocation;
	string _housePath;
	string _houseFileName;
	string* _matrix;

	//functions:
	string initDockingLocation();
	void initDustInHouse();
	

};
string getDirectionString(Direction direction);
#endif //HOUSE_H