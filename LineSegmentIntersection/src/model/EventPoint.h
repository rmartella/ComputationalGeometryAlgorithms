/*
 * EventPoint.h
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */
#include "Segment.h"
#include "TypeEventPoint.cpp"

#ifndef SRC_MODEL_EVENTPOINT_H_
#define SRC_MODEL_EVENTPOINT_H_

class EventPoint {
public:
	EventPoint();
	EventPoint(Point pointEvent, Segment segment, TypeEventPoint type);
	void setSegment(Segment segment);
	Segment getSegment();
	void setSegmentInt(Segment segment);
	Segment getSegmentInt();
	void setType(TypeEventPoint type);
	TypeEventPoint getTypeEventPoint();
	Point getPointEvent();
	void setPointEvent(Point pointEvent);
	bool isEqual(Point pointEvent);
	bool isLess(Point pointEvent);
	bool isGreater(Point pointEvent);
	virtual ~EventPoint();

private:
	Segment segment;
	TypeEventPoint type;
	Point pointEvent;
	Segment segemntInt;
};

#endif /* SRC_MODEL_EVENTPOINT_H_ */
