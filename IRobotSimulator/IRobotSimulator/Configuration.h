#ifndef __CONFIGURATION__H_
#define __CONFIGURATION__H_

class Configuration {
public:
	int BatteryConsumptionRate, MaxStepsAfterWinner, BatteryCapacity, BatteryRechargeRate;
	Configuration(int BatteryConsumptionRate_, int MaxStepsAfterWinner_, int BatteryCapacity_, int BatteryRechargeRate_) :
		BatteryConsumptionRate(BatteryConsumptionRate_),
		MaxStepsAfterWinner(MaxStepsAfterWinner_),
		BatteryCapacity(BatteryCapacity_),
		BatteryRechargeRate(BatteryRechargeRate_){}
};

#endif //__CONFIGURATION__H_