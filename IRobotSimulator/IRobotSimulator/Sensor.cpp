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

pair <int, int> Sensor::getSensorLocation(){
	return mSensorLocation;
}

void Sensor::moveSensor(Direction direction){
	string dirStr;
	switch (direction){
	case Direction::East:
		mSensorLocation.first++;
		dirStr = "East";
		break;
	case Direction::West:
		mSensorLocation.first--;
		dirStr = "West";
		break;
	case Direction::South:
		mSensorLocation.second++;
		dirStr = "South";
		break;
	case Direction::North:
		mSensorLocation.second--;
		dirStr = "North";
		break;
	case Direction::Stay:
		dirStr = "Stay";
		break;
	default:
		dirStr = "";
		cout << "Sensor::moveSensor: no such Direction exists! " << endl;
	}
	if (DEBUG)
	{
		cout << "step to location: <row,col> = <" << mSensorLocation.second << ", " << mSensorLocation.first << "> (" << dirStr << ")" << endl;
	}
}