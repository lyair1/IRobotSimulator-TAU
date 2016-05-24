#include "Direction.h"
#include "MakeUnique.cpp"
#include "AlgorithmRegistration.h"
#include "Simulator.h"
#ifdef _WIN32
#include "200945657_A_.h"
#endif
AlgorithmRegistration::AlgorithmRegistration(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory)
{
#ifndef _WIN32
	AlgorithmLoader::getInstance().registerAlgorithm(algorithmFactory);
#else
	AbstractAlgorithm* algo_ = new _200945657_A();;
	const char* algoName_ = "Nir";
	AlgorithmLoader::getInstance( algo_, algoName_).registerAlgorithm(algorithmFactory);
#endif
}
