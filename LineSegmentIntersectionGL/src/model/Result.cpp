/*
 * Result.cpp
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */

#include "Result.h"

namespace Result {

Result::Result() {
	// TODO Auto-generated constructor stub

}

Result::Result(Segment segmento1, Segment segmento2) {
	this->segmento1 = segmento1;
	this->segmento2 = segmento2;
}

Result::~Result() {
	// TODO Auto-generated destructor stub
}

Segment Result::getSegement1() {
	return this->segmento1;
}
void Result::setSegement1(Segment segmento) {
	this->segmento1 = segmento;
}
Segment Result::getSegement2() {
	return this->segmento2;
}
void Result::setSegement2(Segment segmento) {
	this->segmento2 = segmento;
}

} /* namespace UtilInt */
