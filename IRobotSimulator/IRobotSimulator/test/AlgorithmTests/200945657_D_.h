/**
@author Nir Orman ID 201588902
@author Yair Levi ID 200945657
@version 1.0 30/1/16
*/
#ifndef ___200945657_D_
#define ___200945657_D_

#include "AlgorithmBase.h"

using namespace std;

/*	The purpose of the algorithm:
HIT THE WALL.
*/

class _200945657_D : public AlgorithmBase
{
public:
	_200945657_D()
	{
	}
	~_200945657_D()
	{
		// base class will call clenaResources()
	}

	void aboutToFinish(int stepsTillFinishing) override;

	void cleanResources();

	Direction getNextStep(SensorInformation info, Direction prevStep);
private:
};

#endif //ALGO_NAIVE_H