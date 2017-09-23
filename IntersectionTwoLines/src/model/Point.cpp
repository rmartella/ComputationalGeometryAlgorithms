/*
 * Punto.cpp
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */

#include "Point.h"

using namespace std;

Point::Point() {
}

Point::Point(float x, float y) {
	this->x = x;
	this->y = y;
}

float Point::getX() {
	return this->x;
}

float Point::getY() {
	return this->y;
}

void Point::setX(float x) {
	this->x = x;
}

void Point::setY(float y) {
	this->y = y;
}

Point::~Point() {
	// TODO Auto-generated destructor stub
}

