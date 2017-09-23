/*
 * Segment.h
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Point.h"

#ifndef SRC_MODEL_SEGMENTO_H_
#define SRC_MODEL_SEGMENTO_H_

class Segment {
public:
	Segment();
	Segment(Point p1, Point p2);
	Point getPoint1();
	Point getPoint2();
	void setPoint1(Point p1);
	void setPoint2(Point p2);
	virtual ~Segment();
	bool isEqual(Segment segment);
	bool isLess(Segment segment, Point point);
	bool isGreater(Segment segment, Point point);
private:
	Point p1;
	Point p2;
};

#endif /* SRC_MODEL_SEGMENTO_H_ */
