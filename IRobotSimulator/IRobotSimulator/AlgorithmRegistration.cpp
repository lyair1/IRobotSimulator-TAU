#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistration.h"
#include "AlgorithmLoader.h"

#ifdef _WIN32
#include "200945657_A_.h"
#endif

AlgorithmRegistration::AlgorithmRegistration(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory)
{
#ifndef _WIN32
	cout << "algorithm registered" << endl;
	AlgorithmLoader::getInstance().registerAlgorithm(algorithmFactory);
	
#else
	AbstractAlgorithm* algo_ = new _200945657_A();;
	const char* algoName_ = "Nir";
	cout << "algorithm registered" << endl;
	AlgorithmLoader::getInstance( algo_, algoName_).registerAlgorithm(algorithmFactory);
#endif
}
