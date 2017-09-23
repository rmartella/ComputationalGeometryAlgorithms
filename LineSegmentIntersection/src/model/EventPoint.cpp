/*
 * EventPoint.cpp
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */

#include "EventPoint.h"

EventPoint::EventPoint() {
	// TODO Auto-generated constructor stub

}

EventPoint::EventPoint(Point pointEvent, Segment segment, TypeEventPoint type) {
	this->segment = segment;
	this->type = type;
	this->pointEvent = pointEvent;
}

void EventPoint::setSegment(Segment segment) {
	this->segment = segment;

}

Segment EventPoint::getSegment() {
	return this->segment;
}

void EventPoint::setType(TypeEventPoint type) {
	this->type = type;
}

TypeEventPoint EventPoint::getTypeEventPoint() {
	return this->type;
}

Point EventPoint::getPointEvent() {
	return this->pointEvent;
}

void EventPoint::setPointEvent(Point pointEvent) {
	this->pointEvent = pointEvent;
}

bool EventPoint::isEqual(Point pointEvent) {
	return this->pointEvent.getY() == pointEvent.getY() ?
			(this->pointEvent.getX() == pointEvent.getX() ? true : false) :
			false;
}

bool EventPoint::isLess(Point pointEvent) {
	return this->pointEvent.getY() <= pointEvent.getY() ?
			(this->pointEvent.getY() == pointEvent.getY() ?
					(this->pointEvent.getX() > pointEvent.getX() ? true : false) :
					true) :
			false;
}

bool EventPoint::isGreater(Point pointEvent) {
	return this->pointEvent.getY() >= pointEvent.getY() ?
			(this->pointEvent.getY() == pointEvent.getY() ?
					(this->pointEvent.getX() < pointEvent.getX() ? true : false) :
					true) :
			false;
}

Segment EventPoint::getSegmentInt(){
	return this->segemntInt;
}
void EventPoint::setSegmentInt(Segment segment){
	this->segemntInt = segment;
}

EventPoint::~EventPoint() {
// TODO Auto-generated destructor stub
}

