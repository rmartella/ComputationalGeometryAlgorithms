/*
 * UtilityIntersection.cpp
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */

#include "UtilityIntersection.h"
#include <cstdlib>
#include <stdio.h>

using namespace std;

namespace UtilInt {

UtilityIntersection::UtilityIntersection() {
	// TODO Auto-generated constructor stub

}

UtilityIntersection::~UtilityIntersection() {
	// TODO Auto-generated destructor stub
}

bool UtilityIntersection::isIntersect(Segment segmento1, Segment segmento2) {
	float determinante1 = (segmento1.getPoint2().getX()
			- segmento1.getPoint1().getX())
			* (segmento2.getPoint1().getY() - segmento1.getPoint2().getY())
			- (segmento1.getPoint2().getY() - segmento1.getPoint1().getY())
					* (segmento2.getPoint1().getX()
							- segmento1.getPoint2().getX());
	float determinante2 = (segmento1.getPoint2().getX()
			- segmento1.getPoint1().getX())
			* (segmento2.getPoint2().getY() - segmento1.getPoint2().getY())
			- (segmento1.getPoint2().getY() - segmento1.getPoint1().getY())
					* (segmento2.getPoint2().getX()
							- segmento1.getPoint2().getX());
	if ((determinante1 <= 0 && determinante2 >= 0)
			|| (determinante1 >= 0 && determinante2 <= 0)) {
		determinante1 = (segmento2.getPoint2().getX()
				- segmento2.getPoint1().getX())
				* (segmento1.getPoint1().getY() - segmento2.getPoint2().getY())
				- (segmento2.getPoint2().getY() - segmento2.getPoint1().getY())
						* (segmento1.getPoint1().getX()
								- segmento2.getPoint2().getX());
		determinante2 = (segmento2.getPoint2().getX()
				- segmento2.getPoint1().getX())
				* (segmento1.getPoint2().getY() - segmento2.getPoint2().getY())
				- (segmento2.getPoint2().getY() - segmento2.getPoint1().getY())
						* (segmento1.getPoint2().getX()
								- segmento2.getPoint2().getX());
		if ((determinante1 <= 0 && determinante2 >= 0)
				|| (determinante1 >= 0 && determinante2 <= 0))
			return true;
		else
			return false;
	} else
		return false;
}

Point * UtilityIntersection::getIntersection(Segment segmento1,
		Segment segmento2) {
	float A1 = segmento1.getPoint2().getY() - segmento1.getPoint1().getY();
	float B1 = segmento1.getPoint1().getX() - segmento1.getPoint2().getX();
	float C1 = A1 * segmento1.getPoint1().getX()
			+ B1 * segmento1.getPoint1().getY();
	float A2 = segmento2.getPoint2().getY() - segmento2.getPoint1().getY();
	float B2 = segmento2.getPoint1().getX() - segmento2.getPoint2().getX();
	float C2 = A2 * segmento2.getPoint1().getX()
			+ B2 * segmento2.getPoint1().getY();
	double det = A1 * B2 - A2 * B1;
	double x = (B2 * C1 - B1 * C2) / det;
	double y = (A1 * C2 - A2 * C1) / det;
	Point * point = (Point*) malloc(size_t(sizeof(point)));
	point->setX(x);
	point->setY(y);
	return point;
}

} /* namespace UtilInt */
