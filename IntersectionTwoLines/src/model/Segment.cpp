/*
 * Segment.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Segment.h"

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
