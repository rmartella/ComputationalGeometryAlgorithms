/*
 * TriangulateVertex.h
 *
 *  Created on: 06/09/2015
 *      Author: rey
 */

#ifndef TRIANGULATEVERTEX_H_
#define TRIANGULATEVERTEX_H_

#include "definition.h"

namespace justin {

class TriangulateVertex {
public:
	TriangulateVertex(std::vector<justin::Polygon> * polygons);
	std::vector<justin::Triangle> Traingulate();
	virtual ~TriangulateVertex();
private:
	std::vector<justin::Polygon> * polygons;
};

} /* namespace justin */

#endif /* TRIANGULATEVERTEX_H_ */
