/*
 * Main.cpp
 *
 *  Created on: 31/03/2016
 *      Author: rey
 */
#include <math.h>
#include <iostream>

typedef struct _Point {
	float x, y;
	_Point(float x, float y) {
		this->x = x;
		this->y = y;
	}
	_Point sub(_Point p) {
		return _Point(this->x - p.x, this->y - p.y);
	}
	float dot(_Point p) {
		return x * p.x + y * p.y;
	}
	float comCross(_Point p) {
		return this->x * p.y - this->y * p.x;
	}
	float module() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}
} Point;

int main(int argc, char ** argv) {

	Point p1(2.0, 2.0);
	Point p2(3.0, 1.0);
	Point p3(5.0, 2.0);
	Point p4(4.0, 1.0);

	Point u1 = p2.sub(p1);
	Point u2 = p3.sub(p4);

	float com = u1.comCross(u2);
	std::cout << "com:" << com << std::endl;

	/*float aco = u1.dot(u2) / (u1.module() * u2.module());
	float theta = acosf(aco);

	std::cout << "theta:" << theta << std::endl;*/

	return 1;
}
