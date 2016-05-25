/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef HOUSE_H
#define HOUSE_H
#define DEBUG 0
#define DEBUG_LOW_LEVEL 0
#include <iostream>
#include <string>
#include <fstream>
#include "SensorInformation.h"
#include "Direction.h"
#include <list>
#include "AbstractAlgorithm.h"
#ifndef _WIN32
#include "MakeUnique.cpp"
#else
#include <memory>
#endif

using namespace std;

typedef std::list<AbstractAlgorithm*> AlgorithmList;

#define HOUSE_EXT ".house"
#define USAGE "Usage: simulator [-config <config path>] [-house_path <house path>] [-algorithm_path <algorithm path>] [-score_formula <score .so path>] [-threads <num threads>]\n"

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
		mHouseName(""),
		mMaxSteps(0),
		mHouseRow(0),
		mHouseCol(0),
		mDustInHouse(0),
		mDockingLocation(-1, -1),
		mHousePath(""),
		mHouseFileName(""),
		mHouseMatrix(NULL)
		
	{
		
	}

	//copy constructor for house:
	House(const House & otherHouse) = delete;

	//copy assignment operator

	House & operator = (const House & otherHouse) = delete;

	~House(){

	}
	string getName()const;
	int getMaxSteps()const;
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
	list<int> algorithmScores;
	string getHouseFileName() const;
	string getHousePath() const;
	list<unique_ptr<AbstractAlgorithm>> mAlgorithmList;
private:
	// members:
	string mHouseName;
	int mMaxSteps;
	int mHouseRow;
	int mHouseCol;
	int mDustInHouse;
	pair <int, int> mDockingLocation;
	string mHousePath;
	string mHouseFileName;
	string* mHouseMatrix;

	//functions:
	string initDockingLocation();
	void initDustInHouse();
	

};
string getDirectionString(Direction direction);
#endif //HOUSE_H