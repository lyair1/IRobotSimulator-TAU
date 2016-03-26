#include "Sensor.h"

SensorInformation Sensor::sense()const
{
	return mSensorHouse->getLocationInfo(mSensorLocation);
}

pair <int, int> Sensor::getSensorLocation(){
	return mSensorLocation;
}

void Sensor::moveSensor(Direction direction){
	switch (direction){
	case Direction::East:
		mSensorLocation.first++;
		break;
	case Direction::West:
		mSensorLocation.first--;
		break;
	case Direction::South:
		mSensorLocation.second++;
		break;
	case Direction::North:
		mSensorLocation.second--;
		break;
	case Direction::Stay:
		break;
	default:
		cout << "Sensor::moveSensor: no such Direction exists! " << endl;
	}
	if (DEBUG)
	{
		cout << "move to <" << mSensorLocation.first << "," << mSensorLocation.second << ">" << endl;
	}
}