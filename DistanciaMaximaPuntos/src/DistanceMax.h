/*
 * DistanciaMinima.h
 *
 *  Created on: 01/04/2015
 *      Author: rey
 */
#include "ConvexHull.h"

#ifndef DISTANCEMAX_H_
#define DISTANCEMAX_H_

namespace DistanceMax {

class DistanceMax {
public:
	DistanceMax();
	DistanceMax(std::vector<ConvexHull::Punto> listPoints);
	virtual ~DistanceMax();
	std::vector<ConvexHull::Punto> getPointsMaxDistance();
private:
	std::vector<ConvexHull::Punto> listPoints;
};

} /* namespace UtilInt */

#endif /* DISTANCIAMINIMA_H_ */
