/*
 * Result.h
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */
#include "Segment.h"

#ifndef SRC_MODEL_RESULT_H_
#define SRC_MODEL_RESULT_H_

namespace Result {

class Result {
public:
	Result();
	Result(Segment segmento1, Segment segmento);
	virtual ~Result();
	Segment getSegement1();
	void setSegement1(Segment segmento);
	Segment getSegement2();
	void setSegement2(Segment segmento);
private:
	Segment segmento1;
	Segment segmento2;
};

} /* namespace UtilInt */

#endif /* SRC_MODEL_RESULT_H_ */
