#include "AlgorithmLoader.h"
#include <iostream>
#include <boost/filesystem.hpp>
#ifndef _WIN32
#include <dlfcn.h>
#endif

#define _LOADER_DEBUG_ 1
// create the static field


AlgorithmLoader AlgorithmLoader::instance;
void AlgorithmLoader::registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory)
{
	if ( mEnableRegistration)
	{
		instance.mAlgorithmFactories.push_back(algorithmFactory);
	}
}
AlgorithmLoader::AlgorithmLoader(): mEnableRegistration(false)
{
}
#ifndef _WIN32
void AlgorithmLoader:: setRegistrationOn(bool enableRegistration){
	mEnableRegistration = enableRegistration;
}
bool AlgorithmLoader::loadAlgorithm(const string & algorithmPath_, const string& algorithmName_)
{
	if (_LOADER_DEBUG_)
	{
		cout<< "loading algorihm " << algorithmPath_<<" the size of instance is "<< instance.size()<<endl;
	}
	size_t size = instance.size();
	boost::filesystem::path path(algorithmPath_);
	string algoFileName = path.filename().generic_string();
	string algoSimpleName = path.stem().generic_string();

	// Opening the .so file:
	void *handle = dlopen(algorithmPath_.c_str(), RTLD_NOW);
	if (_LOADER_DEBUG_)
	{
		cout<< "dlopen of path = " << algorithmPath_.c_str()<<" returned handle= "<< handle<<endl;
	}
	if (handle == NULL)
	{
		mErrorMessage += algoFileName + ": file cannot be loaded or is not a valid .so\n";
		if (_LOADER_DEBUG_)
		{
			cout<< "dlopen handle is NULL, algorithm name = " <<algoFileName <<"\n, dlerror()= "<<dlerror()<<"\n. file cannot be loaded or is not a valid .so"<<endl;
		}
		return false;
	}
	if (instance.size() == size) {
		mErrorMessage += algoFileName + ": valid .so but no algorithm was registered after loading it\n";
		if (_LOADER_DEBUG_)
		{
			cout<< "instance.size() = " <<size << "algorithm name = "<<algoFileName<< ".valid .so but no algorithm was registered after loading it"<<endl;
		}
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
	if (_LOADER_DEBUG_)
	{
		cout<< "AlgorithmLoader() destructor dlclose every handle "<<endl;
	}
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

AlgorithmLoader::~AlgorithmLoader()
{
	if (_LOADER_DEBUG_)
	{
		cout << "AlgorithmLoader() destructor dlclose every handle " << endl;
	}
	mAlgorithmFactories.clear();
	//don't call dlclose on windows
}

#endif