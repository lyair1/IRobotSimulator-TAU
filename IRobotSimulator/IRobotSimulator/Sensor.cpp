/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#include "Sensor.h"

SensorInformation Sensor::sense()const
{
	return mSensorHouse->getLocationInfo(mSensorLocation);
}

pair <int, int> Sensor::getSensorLocation() const{
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
	if (DEBUG_LOW_LEVEL)
	{
		cout << "step to location: <row,col> = <" << mSensorLocation.second << ", " << mSensorLocation.first << "> (" << getDirectionString(direction) << ")" << endl;
	}
}