/*
 * UtilityIntersection.h
 *
 *  Created on: 28/02/2015
 *      Author: rey
 */

#ifndef SRC_UTILITYINTERSECTION_H_
#define SRC_UTILITYINTERSECTION_H_

#include "model/Segment.h"

namespace UtilInt {

class UtilityIntersection {
public:
	UtilityIntersection();
	virtual ~UtilityIntersection();
	bool isIntersect(Segment segmento1, Segment segmento2);
	Point * getIntersection(Segment segmento1, Segment segmento2);
};

} /* namespace UtilInt */

#endif /* SRC_UTILITYINTERSECTION_H_ */
