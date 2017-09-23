/*
 * UtilGeometry.cpp
 *
 *  Created on: 07/09/2015
 *      Author: rey
 */

#include "UtilGeometry.h"

namespace justin {

UtilGeometry::UtilGeometry() {
	// TODO Auto-generated constructor stub

}

UtilGeometry::~UtilGeometry() {
	// TODO Auto-generated destructor stub
}

float UtilGeometry::getDeterminant(Vertex * vertice1, Vertex * vertice2,
		Vertex * vertice3) {
	float determinant = (vertice2->x - vertice1->x)
			* (vertice3->y - vertice2->y)
			- (vertice2->y - vertice1->y) * (vertice3->x - vertice2->x);
	return determinant;
}

} /* namespace justin */
