/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef HOUSE_H
#define HOUSE_H
#define DEBUG 1
#include <iostream>
#include <string>
#include <fstream>
#include "SensorInformation.h"
using namespace std;
#define HOUSE_EXT ".house"


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

	House() : _matrix(NULL) {}
	~House(){
		if (_matrix != NULL)	{
			delete[] _matrix;
		}
	}
	string getName()const;
	int getMaxSteps()const;
	int getR() const;
	int getC()const;
	SensorInformation getLocationInfo(std::pair<const int, const int> location)const;
	string isLegalHouse();
	void printHouse()const;
	string fillHouseInfo(string filePath);
	bool isDirtCollected(pair<int, int> location);
	char getLocationValue(pair<int, int> location);
	bool isCleanHouse();
	pair <int, int> getDockingLocation();
	int getDustInHouse();
	void cleanResources();
	string housePath;
private:
	string _name;
	int _maxSteps;
	int _R;
	int _C;
	string* _matrix;
	int _mDustInHouse;
	pair <int, int> _mDockingLocation;
	void setR(int R);
	void setC(int C);
	void setName(string name);
	void setMaxSteps(int maxSteps);
	string initDockingLocation();
	void initDustInHouse();
};

#endif //HOUSE_H