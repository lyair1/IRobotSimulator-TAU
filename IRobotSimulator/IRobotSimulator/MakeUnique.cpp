#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
using namespace std;

// =========================================================================//
//									USAGE EXAMPLES							//
// =========================================================================//
// =================================1=======================================//
//std::unique_ptr<std::string> ptr2 = make_unique<std::string>("Hello");
//cout << *ptr2 << endl;
// =================================2=======================================//
//const int strArrSize = 2; // note that this is NOT constexpr, it can be a variable
//std::unique_ptr<std::string[]> strArr = make_unique<std::string[]>(strArrSize);
//strArr[0] = "Hello";
//strArr[1] = "Bye";
//for (int i = 0; i<strArrSize; ++i) {
//	cout << strArr[i] << endl;
//}
// =================================3=======================================//
//const int strArrSize2 = 3; // note that this is NOT constexpr, it can be a variable
//std::unique_ptr<std::string[]> strArr2(new string[strArrSize2]{"Mo", "Joe", "Do"});
//you can initialize the array in the same line, but not with make_unique...
//for (int i = 0; i<strArrSize2; ++i) {
//	cout << strArr2[i] << endl;
//}


// these does NOT work
//============================================
// std::unique_ptr<std::string> ptr1 = make_unique<std::string[2]>("Hello", "Bye");
// std::unique_ptr<std::string> ptr1 = make_unique<std::string[]>("Hello", "Bye");
// std::unique_ptr<std::string> ptr1 = make_unique<std::string[]>();

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
	for (auto& el : vec)
	{
		os << el << ' ';
	}
	return os;
}

#ifdef __cpp_lib_make_unique
//    std::cout << "we have make_unique" << std::endl;
#else

template<class T> struct _Unique_if {
	typedef unique_ptr<T> _Single_object;
};

template<class T> struct _Unique_if<T[]> {
	typedef unique_ptr<T[]> _Unknown_bound;
};

template<class T, size_t N> struct _Unique_if<T[N]> {
	typedef void _Known_bound;
};

template<class T, class... Args>
typename _Unique_if<T>::_Single_object
make_unique(Args&&... args) {
	return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
typename _Unique_if<T>::_Unknown_bound
make_unique(size_t n) {
	typedef typename remove_extent<T>::type U;
	return unique_ptr<T>(new U[n]());
}

template<class T, class... Args>
typename _Unique_if<T>::_Known_bound
make_unique(Args&&...) = delete;

#endif