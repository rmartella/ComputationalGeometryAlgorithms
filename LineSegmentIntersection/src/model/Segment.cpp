/*
 * Segment.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Segment.h"
#include "../Util/UtilityIntersection.h"
#include <cstdlib>

using namespace std;

Segment::Segment() {
	// TODO Auto-generated constructor stub

}

Segment::~Segment() {
	// TODO Auto-generated destructor stub
}

Segment::Segment(Point p1, Point p2) {
	this->p1 = p1;
	this->p2 = p2;
}
Point Segment::getPoint1() {
	return this->p1;
}
Point Segment::getPoint2() {
	return this->p2;
}
void Segment::setPoint1(Point p1) {
	this->p1 = p1;
}
void Segment::setPoint2(Point p2) {
	this->p2 = p2;
}

bool Segment::isEqual(Segment segment) {
	return this->p1.isEqual(segment.getPoint1())
			&& this->p2.isEqual(segment.getPoint2());
}

bool Segment::isLess(Segment segment, Point point) {
	UtilInt::UtilityIntersection * inter =
			(UtilInt::UtilityIntersection *) malloc(
					size_t(sizeof(UtilInt::UtilityIntersection)));
	Point * pre1 = (Point *) malloc(size_t(sizeof(Point)));
	Point * pre2 = (Point *) malloc(size_t(sizeof(Point)));
	bool resul = false;
	pre1->setX(0);
	pre2->setX(5);
	pre1->setY(point.getY());
	pre2->setY(point.getY());
	Segment * sre = new Segment(*pre1, *pre2);
	Point * pointIntThis = inter->getIntersection(*this, *sre);
	Point * pointIntSeg = inter->getIntersection(segment, *sre);
	if (pointIntThis->getX() <= pointIntSeg->getX())
		resul = true;
	free(pre1);
	free(pre2);
	free(sre);
	free(pointIntThis);
	free(pointIntSeg);
	return resul;
}

bool Segment::isGreater(Segment segment, Point point) {
	UtilInt::UtilityIntersection * inter =
			(UtilInt::UtilityIntersection *) malloc(
					size_t(sizeof(UtilInt::UtilityIntersection)));
	Point * pre1 = (Point *) malloc(size_t(sizeof(Point)));
	Point * pre2 = (Point *) malloc(size_t(sizeof(Point)));
	bool resul = false;
	pre1->setX(0);
	pre2->setX(5);
	pre1->setY(point.getY());
	pre2->setY(point.getY());
	Segment * sre = new Segment(*pre1, *pre2);
	Point * pointIntThis = inter->getIntersection(*this, *sre);
	Point * pointIntSeg = inter->getIntersection(segment, *sre);
	if (pointIntThis->getX() >= pointIntSeg->getX())
		resul = true;
	free(pre1);
	free(pre2);
	free(sre);
	free(pointIntThis);
	free(pointIntSeg);
	return resul;
}
