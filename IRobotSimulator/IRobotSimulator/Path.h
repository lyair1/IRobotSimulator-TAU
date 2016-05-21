#ifndef __PATH__H_
#define __PATH__H_
#include "Point.h"
#include "Direction.h"
#include <vector>

using namespace std;
class Path {
public:
	Path(){};
	Path(Point origin_, Point dest_, vector<Point> path_, size_t length_) : origin(origin_), dest(dest_), path(path_), length(length_) {}

	Point origin, dest;
	vector<Point> path;
	size_t length;
	bool operator==(const Path& other) const { return (origin == other.origin && dest == other.dest); }
	bool operator!=(const Path& other) const { return (origin != other.origin || dest != other.dest); }
	bool operator<(const Path& other) const { return (true); }
	friend ostream& operator<<(ostream& out, const Path& p) { return out; }
};

#endif //__PATH__H_