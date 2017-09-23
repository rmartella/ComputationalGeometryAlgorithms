/*
 * Punto.h
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */

#include "stddef.h"
#include <iostream>

#ifndef SRC_MODEL_POINT_H_
#define SRC_MODEL_POINT_H_

class Point {
public:
	Point();
	Point(float x, float y);
	virtual ~Point();
	float getX();
	void setX(float x);
	float getY();
	void setY(float y);
private:
	float x, y;
};

#endif /* SRC_MODEL_POINT_H_ */
