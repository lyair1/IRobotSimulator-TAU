/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "AlgorithmBase.h"
#include "configReader.h"
#include <queue>
#include "Path.h"
#include <stack>
#include <vector>
#include <algorithm>

// setSensor is called once when the Algorithm is initialized 
void AlgorithmBase::setSensor(const AbstractSensor& sensor)
{
	mLastDirection = -1;
	mPrevLastDirection = -1;
	delete mMoves;
	mMoves = new list <Direction>();
	mSensor = &sensor;
	mMatrixSize = DEFAULT_MATRIX_SIZE;
	mMatrix = new string[DEFAULT_MATRIX_SIZE*2];
	mStepsTillFinish = -1;
	mWallsSet.clear();
	mNotWallSet.clear();
	mCleanedSet.clear();
	mDirtsMap.clear();
	mMoatizationShortestPaths.clear();
}

// setConfiguration is called once when the Algorithm is initialized - see below 
void AlgorithmBase::setConfiguration(map<string, int> config)
{
	mConfiguration = Configuration(config[BATTERY_CONSUMPTION_RATE], config[MAX_STEPS_AFTER_WINNER], config[BATTERY_CAPACITY], config[BATTERY_RECHARGE_RATE]);
	mBatteryLeft = mConfiguration.BatteryCapacity;

	addRobotToMatrix(Point(0, 0));
	addDockingToMatrix(Point(0, 0));
}

// step is called by the simulation for each time unit 
/*	the algorithm MUST take into account the actual
	steps that were taken, provided as a parameter to 'step', not relying on the recommended
	steps returned from the calls to 'step' to be the actual steps taken*/
Direction AlgorithmBase::step(Direction prevStep){

	if (myPrevStep != prevStep)
	{
		myPrevPoint.move(prevStep);
		mLocation = myPrevPoint;
		fakeStepsCount++;
	}

	SensorInformation info = mSensor->sense();
	//if there's still dust - stay in current location:
	Direction chosenDirection = Direction::Stay;
	
	// Safe clean memoization
	cleanMemoizationFromWrongPaths();
	
	mNotWallSet.erase(mLocation);
	chosenDirection = getNextStep(info, prevStep);

	Point next_point = mLocation;
	next_point.move(chosenDirection);
	addRobotToMatrix(next_point);
	if (info.dirtLevel == 0)
	{
		addCleanToMatrix(mLocation);
	}
	else
	{
		addDirtToMatrix(mLocation, info.dirtLevel);
	}

	updateBattery();

	if (_ALGORITHM_DEBUG_)
	{
		createHouseMatrix();
		printHouseMatrix();
#if defined (_WIN32)
		//system("pause");
#endif
	}

	if (chosenDirection != Direction::Stay)
	{
		lastDirection = chosenDirection;
	}

	myPrevPoint = mLocation;
	myPrevStep = chosenDirection;

	stepsCount++;
	fakeStatistics = fakeStepsCount*100 / stepsCount;
	
	return chosenDirection;
}

// this method is called by the simulation either when there is a winner or 
// when steps == MaxSteps - MaxStepsAfterWinner 
// parameter stepsTillFinishing == MaxStepsAfterWinner 
void AlgorithmBase::aboutToFinish(int stepsTillFinishing){
	mAboutToFinish = true;
	if (mBatteryLeft > stepsTillFinishing)
	{
		mBatteryLeft = stepsTillFinishing;
	}
}

void AlgorithmBase::cleanResources(){
	delete mMoves;
}

Direction AlgorithmBase::oppositeDirection(Direction direction_)
{
	switch (direction_)
	{
	case Direction::East: return Direction::West;
	case Direction::West: return Direction::East;
	case Direction::North: return Direction::South;
	case Direction::South: return Direction::North;
	case Direction::Stay: return Direction::Stay;
	}
	return Direction::Stay;
}

void AlgorithmBase::createHouseMatrix()
{
	int negMatrixSize = (-1)*mMatrixSize;
	for (int i = negMatrixSize; i < mMatrixSize; i++)
	{
		mMatrix[i + mMatrixSize] = "";
		for (int j = negMatrixSize; j < mMatrixSize; j++)
		{
			Point currP = Point(mLocation._x + j, mLocation._y + i);
			string pointChar = CHAR_DEFAULT;

			if (isWall(currP))
			{
				pointChar = CHAR_WALL;
			}
			else if (isDirt(currP))
			{
				pointChar = to_string(mDirtsMap[currP]);
			}
			else if (isNotWall(currP))
			{
				pointChar = CHAR_NOT_WALL;
			}
			else if (isCleaned(currP))
			{
				pointChar = CHAR_CLEAN;
			}

			if (mLocation == currP)
			{
				pointChar = CHAR_ROBOT;
			}
			else if (mDockingLocation == currP)
			{
				pointChar = CHAR_DOCKING;
			}

			mMatrix[i + mMatrixSize] += pointChar + " ";
		}
	}
}

void AlgorithmBase::printHouseMatrix()
{
	cout << "**************************************************" << endl;
#if defined (_WIN32)
	system("CLS");
#else
	cout << "\033[2J\033[1;1H";
#endif
	for (int j = 0; j < mMatrixSize*2; j++)
	{
		cout << mMatrix[j] << endl;
	}
	cout << "**************************************************" << endl;
}

void AlgorithmBase::addWallToMatrix(Point p)
{
	eraseFromAllSets(p);
	mWallsSet.insert(p);
}

void AlgorithmBase::addNotWallToMatrix(Point p)
{
	discoverNewNotWall = true;
	eraseFromAllSets(p);
	mNotWallSet.insert(p);
}

void AlgorithmBase::addCleanToMatrix(Point p)
{
	eraseFromAllSets(p);
	mCleanedSet.insert(p);
}

void AlgorithmBase::addDirtToMatrix(Point p, int dirt)
{
	eraseFromAllSets(p);
	mNotWallSet.insert(p); // adding it here so the robot will look for it when there are no other 'NotWalls'
	mDirtsMap.insert({p,dirt});
}

void AlgorithmBase::addShortestPathToMoatization(Path path)
{
	mMoatizationShortestPaths[pair<Point,Point>(path.origin,path.dest)] = path;

	vector<Point> vector = path.path;
	reverse(vector.begin(), vector.end());
	mMoatizationShortestPaths[pair<Point, Point>(path.dest, path.origin)] = Path(path.dest,path.origin,vector,vector.size()-1);
	
}

void AlgorithmBase::addDockingToMatrix(Point p)
{
	mDockingLocation = p;
}

void AlgorithmBase::addRobotToMatrix(Point p)
{
	mLocation = p;
}

void AlgorithmBase::eraseFromAllSets(Point p)
{
	if (isDirt(p))
	{
		mDirtsMap.erase(p);
	}

	if (isWall(p))
	{
		mWallsSet.erase(p);
	}

	if (isNotWall(p))
	{
		mNotWallSet.erase(p);
	}

	if (isCleaned(p))
	{
		mCleanedSet.erase(p);
	}
}

Point AlgorithmBase::getPointFromDirection(Point origin, Direction direction)
{
	Point nextPoint = Point(origin._x, origin._y);
	nextPoint.move(direction);

	return nextPoint;
}

Path AlgorithmBase::getShortestPathBetween2Points(Point p1, Point p2)
{
	//return breadth_first(p1, p2);

	if (isInMoatization(p1,p2))
	{
		return mMoatizationShortestPaths[pair<Point, Point>(p1, p2)];
	}

	queue<Point> Q;
	map<Point, Point> parent;
	Q.push(p1);
	parent[p1] = p1;
	while (!Q.empty())
	{
		Point point = Q.front();
		Q.pop();
		if (point == p2)
		{
			break;
		}
		//add neighbors to queue
		vector<Point> neighbors;
		neighbors.push_back(Point(point._x - 1, point._y));
		neighbors.push_back(Point(point._x, point._y - 1));
		neighbors.push_back(Point(point._x + 1, point._y));
		neighbors.push_back(Point(point._x, point._y + 1));
		
		vector<Point> notWalls;
		vector<Point> clean;

		for (auto& neighbor : neighbors)
		{
			if (isUnknownPoint(neighbor) || isWall(neighbor))
				continue; // don't add neighbor to queue - outside of house's bounderis or is a wall
			if (parent.find(neighbor) == parent.end()) //don't add neighbor to queue if it was already visited...
			{
				parent[neighbor] = point;
				if (isNotWall(neighbor))
				{
					notWalls.push_back(neighbor);
				}
				else
				{
					clean.push_back(neighbor);
				}
			}
		}

		for (auto &p : clean)
		{
			Q.push(p);
		}

		for (auto &p : notWalls)
		{
			Q.push(p);
		}
	}

	//reconstruct path from robot to docking
	Point point = p2;
	vector<Point> shortestPath;
	shortestPath.clear();
	while (point != p1)
	{
		shortestPath.push_back(point);
		addShortestPathToMoatization(Path(point, p2, shortestPath, shortestPath.size() -1));
		point = parent[point];
	}
	shortestPath.push_back(p1);
	reverse(shortestPath.begin(), shortestPath.end());

	Path shortPath = Path(p1, p2, shortestPath, shortestPath.size() - 1);
	addShortestPathToMoatization(shortPath);

	return shortPath;
}

Path AlgorithmBase::getShortestPathToDocking(Point p1)
{
	return getShortestPathBetween2Points(p1, mDockingLocation);
}

Path AlgorithmBase::connect2Paths(Path path1, Path path2)
{
 	vector<Point> AB;
	AB.reserve(path1.path.size() + path2.path.size()); // preallocate memory
	AB.insert(AB.end(), path1.path.begin(), path1.path.end());
	AB.insert(AB.end(), path2.path.begin(), path2.path.end());

	int size = AB.size() - 1;
	if (path1.length > 60000 || path2.length > 60000)
	{
		size = 60000;
	}

	return Path(path1.origin, path2.dest, AB, size);
}

bool AlgorithmBase::isHouseMapped()
{
	if (mNotWallSet.size() == 0 && mMoatizationShortestPaths.size() > 0) // if there is no more 'N' and we already started it means that the house is mapped
	{
		return true;
	}

	return false;
}

bool AlgorithmBase::isHouseClean()
{
	if (!isHouseMapped())
	{
		return false;
	}

	return mDirtsMap.size() == 0 && mNotWallSet.size() == 0;
}

bool AlgorithmBase::isUnknownPoint(Point p)
{
	return !isNotWall(p) && !isWall(p) && !isDirt(p) && !isCleaned(p);
}

bool AlgorithmBase::isWall(Point p)
{
	return (mWallsSet.find(p) != mWallsSet.end());
}

bool AlgorithmBase::isNotWall(Point p)
{
	return (mNotWallSet.find(p) != mNotWallSet.end());
}

bool AlgorithmBase::isDirt(Point p)
{
	return (mDirtsMap.find(p) != mDirtsMap.end());
}

bool AlgorithmBase::isInMoatization(Point p1, Point p2)
{
	return (mMoatizationShortestPaths.find(pair<Point,Point>(p1,p2)) != mMoatizationShortestPaths.end());
}

bool AlgorithmBase::isCleaned(Point p)
{
	return (mCleanedSet.find(p) != mCleanedSet.end());
}

void AlgorithmBase::updateBattery()
{
	if (mLocation == mDockingLocation)
	{
		mBatteryLeft += mConfiguration.BatteryRechargeRate;
		if (mBatteryLeft > mConfiguration.BatteryCapacity)
		{
			mBatteryLeft = mConfiguration.BatteryCapacity;
		}
	}
	else
	{
		mBatteryLeft -= mConfiguration.BatteryConsumptionRate;
	}

	if (mStepsTillFinish > -1 && mBatteryLeft > mStepsTillFinish)
	{
		mBatteryLeft = mStepsTillFinish;
	}
}

void AlgorithmBase::addInfoFromSensor()
{
	discoverNewNotWall = false;
	SensorInformation info = mSensor->sense();

	Point eDirection = getPointFromDirection(mLocation, Direction::East);
	Point wDirection = getPointFromDirection(mLocation, Direction::West);
	Point sDirection = getPointFromDirection(mLocation, Direction::South);
	Point nDirection = getPointFromDirection(mLocation, Direction::North);

	if (info.isWall[static_cast<int>(Direction::East)])
	{
		addWallToMatrix(eDirection);
	}
	else if (isUnknownPoint(eDirection))
	{
		addNotWallToMatrix(eDirection);
	}

	if (info.isWall[static_cast<int>(Direction::West)])
	{
		addWallToMatrix(wDirection);
	}
	else if (isUnknownPoint(wDirection))
	{
		addNotWallToMatrix(wDirection);
	}

	if (info.isWall[static_cast<int>(Direction::South)])
	{
		addWallToMatrix(sDirection);
	}
	else if (isUnknownPoint(sDirection))
	{
		addNotWallToMatrix(sDirection);
	}

	if (info.isWall[static_cast<int>(Direction::North)])
	{
		addWallToMatrix(nDirection);
	}
	else if (isUnknownPoint(nDirection))
	{
		addNotWallToMatrix(nDirection);
	}

	if (info.dirtLevel == 0)
	{
		addCleanToMatrix(mLocation);
	}
	else
	{
		addDirtToMatrix(mLocation, info.dirtLevel);
	}
}

void AlgorithmBase::cleanMemoizationFromWrongPaths()
{
	Point point = mLocation;
	set<pair<Point, Point>> eraseSet;
	for (auto &path : mMoatizationShortestPaths)
	{
		// Remove element if on a point that cross this path
		vector<Point> vector = path.second.path;
		if (vector.size() > 1)
		{
			if (find(vector.begin() + 1, vector.end() - 1, point) != vector.end()) {
				eraseSet.insert(path.first);
			}
		}	
	}

	for (auto &path : eraseSet)
	{
		mMoatizationShortestPaths.erase(path);
	}
}

void AlgorithmBase::debugPrint(string str)
{
	if (_ALGORITHM_DEBUG_)
	{
		cout << str << endl;
	}
}

Direction AlgorithmBase::getStepFromDocking()
{
	Point wDirection = getPointFromDirection(mLocation, Direction::West);

	if (isUnknownPoint(wDirection))
	{
		return Direction::West;
	}

	Point eDirection = getPointFromDirection(mLocation, Direction::East);
	if (isUnknownPoint(eDirection))
	{
		return Direction::East;
	}

	Point nDirection = getPointFromDirection(mLocation, Direction::North);
	if (isUnknownPoint(nDirection))
	{
		return Direction::North;
	}

	Point sDirection = getPointFromDirection(mLocation, Direction::South);
	if (isUnknownPoint(sDirection))
	{
		return Direction::South;
	}

	// Go to not wall
	if (isNotWall(wDirection))
	{
		return Direction::West;
	}
	if (isNotWall(eDirection))
	{
		return Direction::East;
	}
	if (isNotWall(nDirection))
	{
		return Direction::North;
	}
	if (isNotWall(sDirection))
	{
		return Direction::South;
	}
	
	//TODO find dirt>NotWall>Unknown and go to this direction
	if (!isWall(wDirection))
	{
		return Direction::West;
	}
	if (!isWall(eDirection))
	{
		return Direction::East;
	}
	if (!isWall(nDirection))
	{
		return Direction::North;
	}
	if (!isWall(sDirection))
	{
		return Direction::South;
	}

	return Direction::Stay;
}

// Make sure there are Not walls before calling this function
Path AlgorithmBase::findClosestNotWall(bool explorer, bool firstDirt, bool crawlWalls)
{
	if (!discoverNewNotWall && lastShortestPath.length > 0)
	{
		// If because in STAY it's ok
		if (lastShortestPath.origin != mLocation)
		{
			lastShortestPath.origin = lastShortestPath.path[1];
			lastShortestPath.path.erase(lastShortestPath.path.begin());
			lastShortestPath.length--;
		}
		
		if (lastShortestPath.length > 0 && lastShortestPath.origin == mLocation)
		{
			return lastShortestPath;
		}
	}

	Path path = getShortestPathBetween2Points(mLocation, mLocation);// create zero path

	Point lastDirectionPoint = getPointFromDirection(mLocation, lastDirection);
	if (explorer) // optimization to go on the same direction if possible.
	{
		if (crawlWalls)
		{
			Point prevPoint = getPointFromDirection(mLocation, oppositeDirection(lastDirection));

			Point ePoint = getPointFromDirection(mLocation, Direction::East);
			if (isWall(getPointFromDirection(prevPoint, Direction::East)) && isNotWall(ePoint))
			{
				Path p = getShortestPathBetween2Points(mLocation, ePoint);
				lastShortestPath = path;
				return  p;
			}

			Point sPoint = getPointFromDirection(mLocation, Direction::South);
			if (isWall(getPointFromDirection(prevPoint, Direction::South)) && isNotWall(sPoint))
			{
				Path p = getShortestPathBetween2Points(mLocation, sPoint);
				lastShortestPath = path;
				return  p;
			}

			Point wPoint = getPointFromDirection(mLocation, Direction::West);
			if (isWall(getPointFromDirection(prevPoint, Direction::West)) && isNotWall(wPoint))
			{
				Path p = getShortestPathBetween2Points(mLocation, wPoint);
				lastShortestPath = path;
				return  p;
			}

			Point nPoint = getPointFromDirection(mLocation, Direction::North);
			if (isWall(getPointFromDirection(prevPoint, Direction::North)) && isNotWall(nPoint))
			{
				Path p = getShortestPathBetween2Points(mLocation, nPoint);
				lastShortestPath = path;
				return  p;
			}
		}

		if (isNotWall(lastDirectionPoint))
		{
			Path p = getShortestPathBetween2Points(mLocation, lastDirectionPoint);
			lastShortestPath = path;
			
			return  p;
		}
	}

	if (!mDirtsMap.empty() && firstDirt)
	{
		for (auto &p : mDirtsMap)
		{
			Path currPath = getShortestPathBetween2Points(mLocation, p.first);
			if (currPath.length == 0)
			{
				continue;
			}
			if (path.length > currPath.length || path.length == 0)
			{
				path = currPath;
			}

			if (currPath.length == 1)
			{
				lastShortestPath = path;
				return path;
			}
		}
	}
	else
	{
		for (auto &p : mNotWallSet)
		{
			Path currPath = getShortestPathBetween2Points(mLocation, p);
			if (currPath.length == 0)
			{
				continue;
			}
			if (path.length >= currPath.length || path.length == 0)
			{
				path = currPath;	
			}

			if (currPath.length == 1)
			{
				lastShortestPath = path;
				return path;
			}
		}
	}

	lastShortestPath = path;
	return path;
}

Direction AlgorithmBase::getDirectionFromPoint(Point origin, Point dest)
{
	if (getPointFromDirection(origin, Direction::North) == dest)
	{
		return Direction::North;
	}

	if (getPointFromDirection(origin, Direction::West) == dest)
	{
		return Direction::West;
	}

	if (getPointFromDirection(origin, Direction::East) == dest)
	{
		return Direction::East;
	}

	if (getPointFromDirection(origin, Direction::South) == dest)
	{
		return Direction::South;
	}

	return Direction::Stay;
}

bool AlgorithmBase::comparePoint(Point p1, Point p2)
{
	if (isNotWall(p1) && isNotWall(p2))
	{
		return true;
	}

	if (isNotWall(p1))
	{
		return false;
	}
	else
	{
		return true;
	}
}

int AlgorithmBase::getPathScore(Path path)
{
	int score = 0;
	for (auto p : path.path)
	{
		if (isNotWall(p))
		{
			score++;
		}
	}

	return score;
}

// Default implementation is a random choose between all of the non-wall options
Direction AlgorithmBase::getNextStep(SensorInformation info, Direction prevStep)
{
	Direction chosenDirection;
	if (mAboutToFinish)
	{
		chosenDirection = mMoves->front();
		mMoves->pop_front();

		return chosenDirection;
	}

	chosenDirection = (Direction)(rand() % 4);
	while (info.isWall[(int)chosenDirection])
	{
		//robot is stepping into a wall - choose a different direction!
		chosenDirection = (Direction)(rand() % 4);
	}
	mMoves->push_front(prevStep);

	return chosenDirection;
}


