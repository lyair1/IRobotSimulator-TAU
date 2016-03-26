#ifndef SENSOR_H
#define SENSOR_H
#include <utility> // std::pair
#include "AbstractSensor.h"
#include "House.h"
#include "Direction.h"


/*
	A sensor is attached to the robot and may be used by the algorithm.
	The sensor can supply the following information:
		1.	Is there dust in the spot where the robot is currently positioned?
			And if there is dust, what is the level of the dust on that spot?
		2. Is there a wall touching the robot in one of the possible four directions:
			East, West, North, or South?
			
	Using the sense() method we get information about the walls and dust at the position where the robot is currently located.
	The algorithm, given that information, should decide what to do during the current time unit
	(move East/West/South/North or Stay). ).
	Sensor is managed by the simulator and should be updated by the simulator.
*/

class Sensor: public AbstractSensor{
public:
	Sensor(pair <int, int> DockingLocation, House* house)
	{
		mSensorLocation.first = DockingLocation.first;
		mSensorLocation.second = DockingLocation.second;
		mSensorHouse = house;
	}

	SensorInformation sense()const;
	void moveSensor(Direction direction);
	pair <int, int> getSensorLocation();

private:
	std::pair <int, int> mSensorLocation ;
	House* mSensorHouse;

};

#endif // SENSOR_H