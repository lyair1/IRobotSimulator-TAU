#ifndef AlgorithmLoaderH
#define AlgorithmLoaderH

#include "Direction.h"
#include "AlgorithmRegister.h"

class AlgorithmLoader
{
	bool isAlgoValid = false;
	string mAlgoFileName;
	string mAlgoSimpleName;
	string mErrorMessage;
public:
	void* handle = NULL;
	string filePath;
	string fileName;
	AlgorithmLoader() = delete;
#ifndef _WIN32
	AlgorithmLoader(string algorithmPath_, string algorithmName_);
	~AlgorithmLoader();
#else
	AlgorithmLoader(AbstractAlgorithm* algo_, const char* algoName_);
	~AlgorithmLoader() {}
#endif
	bool isValid() const { return isAlgoValid; }
	string getFileName() const { return mAlgoFileName; }
	string GetAlgorithmName() const { return mAlgoSimpleName; }
	string getErrorLine() const { return mErrorMessage; }
};


#endif