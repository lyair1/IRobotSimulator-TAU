#ifndef AlgorithmLoaderH
#define AlgorithmLoaderH

#include <iostream>
#include <functional>
#include <memory>
#include <list>
#include <cassert>
#include "Direction.h"
#include "AbstractAlgorithm.h"
#ifndef _WIN32
#include "MakeUnique.cpp"
#endif



class AlgorithmLoader
{
public:
	AlgorithmLoader();
	~AlgorithmLoader();
	AlgorithmLoader(AlgorithmLoader const &) = delete;	// don't implement - it's a singleton!
	void operator = (AlgorithmLoader const &) = delete;	// don't implement - it's a singleton!

	static AlgorithmLoader& getInstance()
	{ 
		return instance;
	}


	friend class AlgorithmRegistration;

	//members: 
	list <std::string> mAlgorithmNames;
	list <std::function<unique_ptr<AbstractAlgorithm>()>> mAlgorithmFactories;
	list < void* > mHandleList;
	
	
	
	//functions: 
	string getAlgorithmErrorMessage();
	void registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory);
	void setNameForLastAlgorithm(const std::string& algorithmName);
	bool loadAlgorithm(const string & algorithmPath_, const string& algorithmName_);
	list<unique_ptr<AbstractAlgorithm>>  getAlgorithms() const;

	const list<std::string>& getAlgorithmNames() const;
	void setRegistrationOn(bool enableRegistration);
	size_t size()const { return mAlgorithmFactories.size();	}
private:
	string mErrorMessage;
	bool mEnableRegistration;
	static AlgorithmLoader instance;
	
};



#endif