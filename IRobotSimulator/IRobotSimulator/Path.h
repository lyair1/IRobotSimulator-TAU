#ifndef __PATH__H_
#define __PATH__H_
#include "Point.h"
#include "Direction.h"
#include <vector>

using namespace std;
class Path {
public:
	Point origin, dest;
	int length;
	vector<Point> path;
	Path(){};
	Path(Point origin_, Point dest_, vector<Point> path_, int length_) : origin(origin_), dest(dest_), path(path_), length(length_) {}
};

#endif //__PATH__H_