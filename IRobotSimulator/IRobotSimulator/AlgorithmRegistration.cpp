#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistration.h"
#include "AlgorithmLoader.h"

#ifdef _WIN32
#include "200945657_A_.h"
#endif

AlgorithmRegistration::AlgorithmRegistration(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory)
{
	cout << "algorithm registered" << endl;
	AlgorithmLoader::getInstance().registerAlgorithm(algorithmFactory);
}
