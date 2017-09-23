/*
 * UtilGeometry.h
 *
 *  Created on: 07/09/2015
 *      Author: rey
 */

#ifndef UTILGEOMETRY_H_
#define UTILGEOMETRY_H_

#include "definition.h"

namespace justin {

class UtilGeometry {
public:
	UtilGeometry();
	float getDeterminant(Vertex * vertice1, Vertex * vertice2, Vertex * vertice3);
	virtual ~UtilGeometry();
};

} /* namespace justin */

#endif /* UTILGEOMETRY_H_ */
