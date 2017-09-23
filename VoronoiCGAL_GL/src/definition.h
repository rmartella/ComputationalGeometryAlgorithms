/**
 * Definition.h
 * Fecha de creaci�n: 12/02/2016, 10:21:08
 *
 * 2016 Universidad Nacional Aut�noma de M�xico - UNAM.
 * Instituto de Investigaciones en Matem�ticas Aplicadas y en Sistemas - IIMAS.
 * Laboratorio de Bio-rob�tica.
 * Se�ales Imagenes y Ambientes Virtuales.
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
#include "eigen3/Eigen/Dense"

#define PI 3.141592653589793238462643383280

namespace biorobotics {

enum TypePolygon {
	WALL, OBSTACLE
};

typedef struct _Vertex2 {
	float x, y;
	_Vertex2() {
	}
	_Vertex2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	static _Vertex2 Zero() {
		_Vertex2 vertex;
		vertex.x = 0.0;
		vertex.y = 0.0;
		return vertex;
	}
	_Vertex2 sub(_Vertex2 v) {
		_Vertex2 vertex;
		vertex.x = v.x - x;
		vertex.y = v.y - y;
		return vertex;
	}
	float norm() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}
} Vertex2;

typedef struct _Vertex3 {
	float x, y, z;
	static _Vertex3 Zero() {
		_Vertex3 vertex;
		vertex.x = 0.0;
		vertex.y = 0.0;
		vertex.z = 0.0;
		return vertex;
	}
	_Vertex3 sub(_Vertex3 v) {
		_Vertex3 vertex;
		vertex.x = v.x - x;
		vertex.y = v.y - y;
		vertex.z = v.z - z;
		return vertex;
	}
	float norm() {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
} Vertex3;

typedef struct _Polygon {
	Vertex2 * vertex;
	int num_vertex;
	TypePolygon type;
} Polygon;

/* --- TODO This change to struct vertex verify --- */
typedef struct _Triangle {
	Eigen::Vector3d v1, v2, v3;
} Triangle;

typedef struct _Box {
	Eigen::Vector3d center;
	Eigen::Vector3d boxhalfsize;
} Box;
/* --- END This change to struct vertex verify --- */

typedef struct _Segment {
	Vertex2 v1, v2;
	_Segment() {
	}
	_Segment(Vertex2 v1, Vertex2 v2) {
		this->v1 = v1;
		this->v2 = v2;
	}
} Segment;

typedef struct _Circle {
	Vertex2 center;
	float ratio;
	_Circle() {
	}
	_Circle(Vertex2 center, float ratio) {
		this->center = center;
		this->ratio = ratio;
	}
} Circle;

}

#endif /* DEFINITION_H_ */
