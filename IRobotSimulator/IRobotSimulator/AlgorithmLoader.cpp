#include "AlgorithmLoader.h"
#include <iostream>
#include <boost/filesystem.hpp>

#ifndef _WIN32
#include <dlfcn.h>
#endif


map<string, instanceCreator> globalFactory;

#ifndef _WIN32
AlgorithmLoader::AlgorithmLoader(string algorithmPath_, string algorithmName_)
{
	filePath = algorithmPath_;
	fileName = algorithmName_;
	boost::filesystem::path path(algorithmPath_);
	algoFileName = path.filename().generic_string();
	algoSimpleName = path.stem().generic_string();

	// Opening the .so file:
	handle = dlopen(algorithmPath_.c_str(), RTLD_NOW);
	if (handle == NULL)
	{
		errorMessage = algoFileName + ": file cannot be loaded or is not a valid .so";
		return;
	}

	// getAbstractAlgorithmPointer is the instance creator method
	void* p = dlsym(handle, "getAbstractAlgorithmPointer");

	// Safe casting
	instanceCreator function1 = reinterpret_cast<instanceCreator>(reinterpret_cast<long>(p));

	if (function1 == nullptr) {
		errorMessage = algoFileName + ": valid .so file but no algorithm was registered after loading it";
		return;
	}

	globalFactory[algoSimpleName] = function1;

	isAlgoValid = true;
}

AlgorithmLoader::~AlgorithmLoader()
{
	if (handle != NULL)
	{
		dlclose(handle);
	}
}

#else
// loading in Windows without shared objects - only for tests!
AlgorithmLoader::AlgorithmLoader(AbstractAlgorithm* algo_, const char* algoName_)
{
	isAlgoValid = true;

}
#endif