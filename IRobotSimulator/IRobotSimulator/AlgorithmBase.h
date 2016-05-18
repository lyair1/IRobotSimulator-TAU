#ifndef __ALGORITHM_BASE__H_
#define __ALGORITHM_BASE__H_

#define _ALGORITHM_DEBUG_ true

#include <vector>
#include <map>
#include <limits.h>
#include "Configuration.h"
#include <set>
class Path;
using namespace std;

#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "House.h"
#include "Point.h"
#define DEFAULT_MATRIX_SIZE 25
#define CHAR_WALL "W"
#define CHAR_NOT_WALL "N"
#define CHAR_DEFAULT "."
#define CHAR_DOCKING "D"
#define CHAR_ROBOT "R"
#define CHAR_CLEAN " "

class AlgorithmBase : public AbstractAlgorithm
{

public:
	AlgorithmBase():mSensor(NULL), 
					mLocation(-1,-1),
					mLastDirection(-1), 
					mPrevLastDirection(-1), 
					mConfiguration(0,0,0,0), 
					mAboutToFinish(false),
					mBatteryLeft(0),
					mStepsTillFinish(-1),
					mMatrixSize(DEFAULT_MATRIX_SIZE),
					mDockingLocation(-1,-1)
	{
		mMatrix = new string[DEFAULT_MATRIX_SIZE*2];
		mMoves = new list<Direction>();
		mWallsSet.clear();
		mNotWallSet.clear();
		mDirtsMap.clear();
		mCleanedSet.clear();
	}
	~AlgorithmBase()
	{
		cleanResources();
	}
	// setSensor is called once when the Algorithm is initialized 
	void setSensor(const AbstractSensor& sensor) override;

	// setConfiguration is called once when the Algorithm is initialized - see below 
	void setConfiguration(map<string, int> config) override;

	// step is called by the simulation for each time unit 
	Direction step(Direction prevStep) override;

	// getNextStep is being called by step() after performing some default checks
	virtual Direction getNextStep(SensorInformation info, Direction prevStep) = 0;

	// this method is called by the simulation either when there is a winner or 
	// when steps == MaxSteps - MaxStepsAfterWinner 
	// parameter stepsTillFinishing == MaxStepsAfterWinner 
	void aboutToFinish(int stepsTillFinishing) override;

	void cleanResources();

	Direction oppositeDirection(Direction direction_);

protected:
	const AbstractSensor* mSensor; // the only info the algorithm has about the house it's cleaning comes from the sensor itself.
	Point mLocation;
	Point mDockingLocation;
	int mLastDirection;
	int mPrevLastDirection;
	Configuration mConfiguration;
	bool mAboutToFinish;
	list<Direction> *mMoves;
	string* mMatrix;
	int mBatteryLeft;
	int mStepsTillFinish;
	int mMatrixSize;
	set<Point> mWallsSet;
	set<Point> mNotWallSet;
	set<Point> mCleanedSet;
	map<Point, int> mDirtsMap;

	void createHouseMatrix();
	void printHouseMatrix();
	void addWallToMatrix(Point p);
	void addNotWallToMatrix(Point p);
	void addCleanToMatrix(Point p);
	void addDirtToMatrix(Point p, int dirt);
	void addDockingToMatrix(Point p);
	void addRobotToMatrix(Point p);
	void eraseFromAllSets(Point p);
	Point getPointFromDirection(Point origin, Direction direction);

	Path getShortestPathBetween2Points(Point p1, Point p2);
	Path getShortestPathToDocking(Point p1);
	Path connect2Paths(Path path1, Path path2);
	
	Point findLeftUpperCorner();
	Point findLeftBottomCorner();
	Point findRightUpperCorner();
	Point findRightBottomCorner();
	bool horLineBetweenPoints(Point p1, Point p2);
	bool verrLineBetweenPoints(Point p1, Point p2);
	bool isHouseMapped();
	bool isHouseClean();
	bool isUnknownPoint(Point p);
	bool isWall(Point p);
	bool isNotWall(Point p);
	bool isDirt(Point p);
	bool isCleaned(Point p);
	void updateBattery();
	void addInfoFromSensor();

	// When in docking
	Direction getStepFromDocking();

	// When in position
	//bool doesHaveEnoughBatteryToFinish();
	Path findClosestNotWall();
};

#endif