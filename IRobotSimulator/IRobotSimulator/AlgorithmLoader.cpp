#include "AlgorithmLoader.h"
#include <iostream>
#include <boost/filesystem.hpp>
#ifndef _WIN32
#include <dlfcn.h>
#else
#include "200945657_C_.h"
#include "200945657_B_.h"
#include "200945657_A_.h"
#endif

#define _LOADER_DEBUG_ 0
// create the static field


AlgorithmLoader AlgorithmLoader::instance;


void AlgorithmLoader::registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory)
{
	if (mEnableRegistration)
	{
		instance.mAlgorithmFactories.push_back(algorithmFactory);
	}
}


AlgorithmLoader::AlgorithmLoader(): mEnableRegistration(false)
{
}


AlgorithmLoader::~AlgorithmLoader()
{
	if (_LOADER_DEBUG_)
	{
		cout << "AlgorithmLoader() destructor dlclose every handle " << endl;
	}
	mAlgorithmFactories.clear();
#ifndef _WIN32	//don't call dlclose on windows
	for (void* handle : mHandleList)
		dlclose(handle);
#endif
}


list<unique_ptr<AbstractAlgorithm>> AlgorithmLoader::getAlgorithms()const {
	list<unique_ptr<AbstractAlgorithm>> algorithms;
	if (_LOADER_DEBUG_)
	{
		cout<< "in function getAlgorithms() " << endl;
	}
#ifndef _WIN32
	for (auto algorithmFactoryFunc : mAlgorithmFactories) {
		algorithms.push_back(algorithmFactoryFunc());
	}
#else
	unique_ptr<AbstractAlgorithm> algoA = make_unique<_200945657_A>();
	algorithms.push_back(move(algoA));
	AlgorithmLoader::getInstance().setNameForLastAlgorithm("algoA");
	unique_ptr<AbstractAlgorithm> algoB = make_unique<_200945657_A>();
	algorithms.push_back(move(algoB));
	AlgorithmLoader::getInstance().setNameForLastAlgorithm("algoB");
	unique_ptr<AbstractAlgorithm> algoC = make_unique<_200945657_A>();
	algorithms.push_back(move(algoC));
	AlgorithmLoader::getInstance().setNameForLastAlgorithm("algoC");
	
#endif

	return algorithms;
}

void AlgorithmLoader::setNameForLastAlgorithm(const std::string& algorithmName) {
#ifndef _WIN32
	assert(mAlgorithmFactories.size() - 1 == mAlgorithmNames.size());
#endif
	mAlgorithmNames.push_back(algorithmName);
}


void AlgorithmLoader:: setRegistrationOn(bool enableRegistration){
	mEnableRegistration = enableRegistration;
}

const list<std::string>& AlgorithmLoader::getAlgorithmNames() const
{
	return mAlgorithmNames;
}

string AlgorithmLoader::getAlgorithmErrorMessage()
{
	return mErrorMessage;
}



#ifndef _WIN32

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


#endif



