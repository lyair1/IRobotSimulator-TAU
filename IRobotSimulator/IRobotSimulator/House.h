/*
Author: Nir Orman ID 201588902
Yair Levi ID 200945657
*/
#ifndef HOUSE_H
#define HOUSE_H
#define DEBUG 1
#include <iostream>
#include <string>
#include <fstream>
#include "SensorInformation.h"
using namespace std;


/*	W – wall
	D – Docking Station(start position and battery recharge)
	number 1 to 9 – dust level
	empty square – no dust
	In this example the house has:
	8 rows(so R = 8)
	10 columns(so C = 10)
	A house description is stored as a file, as following :
	Line 1 : House Short Name
	Line 2 : House Long Description
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

	House() : matrix(NULL) {}
	~House(){
		if (matrix != NULL)	{
			delete[] matrix;
		}
	}
	string getName()const;
	string getDescription()const;
	int getR() const;
	int getC()const;
	SensorInformation getLocationInfo(std::pair<const int, const int> location)const;
	bool isLegalHouse();
	void printHouse()const;
	bool fillHouseInfo(string filePath);
	bool isDirtCollected(pair<int, int> location);
	char getLocationValue(pair<int, int> location);
	bool isCleanHouse();
	pair <int, int> getDockingLocation();
	int getDustInHouse();
	void cleanResources();
private:
	string name;
	string desc;
	int R;
	int C;
	string* matrix;
	int mDustInHouse;
	pair <int, int> mDockingLocation;

	void setR(int R);
	void setC(int C);
	void setName(string name);
	void setDescription(string description);
	bool initDockingLocation();
	void initDustInHouse();
};

#endif //HOUSE_H