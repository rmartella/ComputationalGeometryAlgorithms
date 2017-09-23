/*
 * Definition.h
 *
 *  Created on: 06/09/2015
 *      Author: rey
 */

#ifndef DEFINITION_H_
#define DEFINITION_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <vector>

namespace justin {

enum TypePolygon {
	WALL, OBSTACLE
};

typedef struct _Vertex {
	float x, y, z;
} Vertex;

typedef struct _Polygon {

	std::vector<Vertex> * vertex;
	TypePolygon * type;

} Polygon;

typedef struct _Triangle {
	Vertex v1, v2, v3;
} Triangle;

}

#endif /* DEFINITION_H_ */
