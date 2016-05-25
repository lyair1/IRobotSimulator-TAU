#ifndef __POINT__H_
#define __POINT__H_
#include <functional>

class Point {
public:
	int _x, _y;
	Point(){
		_x = 0;
		_y = 0;
	};
	Point(int x_, int y_) : _x(x_), _y(y_) {}

	void print(ostream& out = cout) const { out << "(" << _x << "," << _y << ")"; }
	void move(Direction d) {
		const static function<void(Point&)> do_move[] = {
			[](Point&p){++p._x; },	// East
			[](Point&p){--p._x; },	// West
			[](Point&p){++p._y; },	// South
			[](Point&p){--p._y; },	// North
			[](Point&p){}			// Stay
		};
		do_move[(int)d](*this);
	}
	bool operator==(const Point& other) const { return (_x == other._x && _y == other._y); }
	bool operator!=(const Point& other) const { return (_x != other._x || _y != other._y); }
	bool operator<(const Point& other) const { return (_x < other._x) || (_x == other._x && _y < other._y); }
	friend ostream& operator<<(ostream& out, const Point& p) { p.print(out); return out; }
};

#endif //__POINT__H_