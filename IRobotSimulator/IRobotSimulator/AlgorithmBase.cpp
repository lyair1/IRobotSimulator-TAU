/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "AlgorithmBase.h"
#include "configReader.h"
#include <queue>
#include "Path.h"

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
	SensorInformation info = mSensor->sense();
	//if there's still dust - stay in current location:
	Direction chosenDirection = Direction::Stay;
	
	chosenDirection = getNextStep(info, prevStep);

	Point next_point = mLocation;
	next_point.move(chosenDirection);
	addRobotToMatrix(next_point);
	mNotWallSet.erase(mLocation);
	if (info.dirtLevel == 0)
	{
		addCleanToMatrix(mLocation);
	}
	else
	{
		addDirtToMatrix(mLocation, info.dirtLevel);
	}

	updateBattery();

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
			else if (isNotWall(currP))
			{
				pointChar = CHAR_NOT_WALL;
			}
			else if (isDirt(currP))
			{
				pointChar = to_string(mDirtsMap[currP]);
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
	for (size_t j = 0; j < mMatrixSize*2; j++)
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
	mDirtsMap.insert({p,dirt});
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
		for (auto& neighbor : neighbors)
		{
			if (isUnknownPoint(neighbor))
				continue; // don't add neighbor to queue - outside of house's bounderis or is a wall
			if (parent.find(neighbor) == parent.end()) //don't add neighbor to queue if it was already visited...
			{
				parent[neighbor] = point;
				Q.push(neighbor);
			}
		}
	}

	//reconstruct path from robot to docking
	Point point = p2;
	vector<Point> shortestPath;
	shortestPath.clear();
	while (point != p1)
	{
		shortestPath.push_back(point);
		point = parent[point];
	}
	shortestPath.push_back(p1);
	std::reverse(shortestPath.begin(), shortestPath.end());

	return Path(p1, p2, shortestPath, shortestPath.size() - 1);
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

	return Path(path1.origin, path2.dest, AB, AB.size()-1);
}

Point AlgorithmBase::findLeftUpperCorner()
{
	Point point = *next(mWallsSet.begin(), 0);
	for (auto p : mWallsSet) {
		if (p._x < point._x && p._y < point._y)
		{
			point = p;
		}
	}

	return point;
}

Point AlgorithmBase::findLeftBottomCorner()
{
	Point point = *next(mWallsSet.begin(), 0);
	for (auto p : mWallsSet) {
		if (p._x < point._x && p._y > point._y)
		{
			point = p;
		}
	}

	return point;
}

Point AlgorithmBase::findRightUpperCorner()
{
	Point point = *next(mWallsSet.begin(), 0);
	for (auto p : mWallsSet) {
		if (p._x > point._x && p._y < point._y)
		{
			point = p;
		}
	}

	return point;
}

Point AlgorithmBase::findRightBottomCorner()
{
	Point point = *next(mWallsSet.begin(), 0);
	for (auto p : mWallsSet) {
		if (p._x > point._x && p._y > point._y)
		{
			point = p;
		}
	}

	return point;
}

bool AlgorithmBase::horLineBetweenPoints(Point p1, Point p2)
{
	if (p1._y != p2._y)
	{
		return false;
	}

	int x_start = p1._x;
	int x_end = p2._x;
	if (p2._x < p1._x)
	{
		x_start = p2._x;
		x_end = p1._x;
	}

	for (int i = x_start; i < x_end; i++)
	{
		if (!isWall(Point(i, p1._y)))
		{
			return false;
		}
	}

	return true;
}

bool AlgorithmBase::verrLineBetweenPoints(Point p1, Point p2)
{
	if (p1._x != p2._x)
	{
		return false;
	}

	int y_start = p1._y;
	int y_end = p2._y;
	if (p2._y < p1._y)
	{
		y_start = p2._y;
		y_end = p1._y;
	}

	for (int i = y_start; i < y_end; i++)
	{
		if (!isWall(Point(p1._x, i)))
		{
			return false;
		}
	}

	return true;
}

bool AlgorithmBase::isHouseMapped()
{
	// Smallest house:
	// WWW
	// WDW
	// WWW
	if (mWallsSet.size() < 8)
	{
		return false;
	}

	Point leftUp = findLeftUpperCorner();
	Point rightUp = findRightUpperCorner();

	if (leftUp._x == rightUp._x)
	{
		return false;
	}

	Point leftBottom = findLeftBottomCorner();

	if (leftUp._y == leftBottom._y)
	{
		return false;
	}
	Point rightBottom = findRightBottomCorner();

	if (rightUp._y == rightBottom._y)
	{
		return false;
	}

	if (leftBottom._x == rightBottom._x)
	{
		return false;
	}

	return horLineBetweenPoints(leftBottom, rightBottom) 
		&& horLineBetweenPoints(leftUp, rightUp) 
		&& verrLineBetweenPoints(leftUp, leftBottom) 
		&& verrLineBetweenPoints(rightUp, rightBottom);
}

bool AlgorithmBase::isHouseClean()
{
	if (!isHouseMapped())
	{
		return false;
	}

	Point leftUp = findLeftUpperCorner();
	Point rightBottom = findRightBottomCorner();

	for (int i = leftUp._x; i < rightBottom._x; i++)
	{
		for (int j = leftUp._y; j < rightBottom._y; j++)
		{
			if (!isCleaned(Point(i,j)))
			{
				return false;
			}
		}
	}

	return true;
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

bool AlgorithmBase::isCleaned(Point p)
{
	return (mCleanedSet.find(p) != mCleanedSet.end());
}

void AlgorithmBase::updateBattery()
{
	if (mLocation == mDockingLocation)
	{
		mBatteryLeft += mConfiguration.BatteryRechargeRate;
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
Path AlgorithmBase::findClosestNotWall()
{
	Point point = *next(mNotWallSet.begin(), 0);
	Path path = getShortestPathBetween2Points(mLocation, point);
	for (auto &p : mNotWallSet)
	{
		Path currPath = getShortestPathBetween2Points(mLocation, p);
		if (path.length > currPath.length)
		{
			path = currPath;
		}
	}

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


