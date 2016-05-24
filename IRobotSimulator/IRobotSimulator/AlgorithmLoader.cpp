#include "AlgorithmLoader.h"
#include <iostream>
#include <boost/filesystem.hpp>
#ifndef _WIN32
#include <dlfcn.h>
#endif

// create the static field


#ifndef _WIN32

AlgorithmLoader AlgorithmLoader::instance;
void AlgorithmLoader::registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory)
{
	instance.mAlgorithmFactories.push_back(algorithmFactory);
}
AlgorithmLoader::AlgorithmLoader(): mEnableRegistration(false)
{
}
void AlgorithmLoader:: setRegistrationOn(bool enableRegistration){
	mEnableRegistration = enableRegistration;
}
bool AlgorithmLoader::loadAlgorithm(const string & algorithmPath_, const string& algorithmName_)
{
	size_t size = instance.size();
	boost::filesystem::path path(algorithmPath_);
	string algoFileName = path.filename().generic_string();
	string algoSimpleName = path.stem().generic_string();

	// Opening the .so file:
	void *handle = dlopen(algorithmPath_.c_str(), RTLD_NOW);
	if (handle == NULL)
	{
		mErrorMessage += algoFileName + ": file cannot be loaded or is not a valid .so";
		return false;
	}
	if (instance.size() == size) {
		mErrorMessage += algoFileName + ": valid .so but no algorithm was registered after loading it";
		return false; // no algorithm registered
	}
	mHandleList.push_back(handle);
	setNameForLastAlgorithm(algoSimpleName);
	return true;
}

list<unique_ptr<AbstractAlgorithm>> AlgorithmLoader::getAlgorithms()const {
	list<unique_ptr<AbstractAlgorithm>> algorithms;
	for (auto algorithmFactoryFunc : mAlgorithmFactories) {
		algorithms.push_back(algorithmFactoryFunc());
	}
	return algorithms;
}

AlgorithmLoader::~AlgorithmLoader(){
	mAlgorithmFactories.clear();
	for (void* handle : mHandleList)
		dlclose(handle);
}
void AlgorithmLoader::setNameForLastAlgorithm(const std::string& algorithmName) {
	assert(mAlgorithmFactories.size() - 1 == mAlgorithmNames.size());
	mAlgorithmNames.push_back(algorithmName);
	}

const list<std::string>& AlgorithmLoader::getAlgorithmNames() const
{
	return mAlgorithmNames;
}
string AlgorithmLoader::getAlgorithmErrorMessage()
{
	return mErrorMessage;
}
#else
AlgorithmLoader::AlgorithmLoader(AbstractAlgorithm* algo_, const char* algoName_)
 {
}
AlgorithmLoader::~AlgorithmLoader()
{

}
void AlgorithmLoader::registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory)
{
	mAlgorithmFactories.push_back(algorithmFactory);
}
#endif