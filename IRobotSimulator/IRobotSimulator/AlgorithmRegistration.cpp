#include "Direction.h"
#include "MakeUnique.cpp"
#include "AlgorithmRegistration.h"
#include "AlgorithmRegister.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory_)
{
	
}