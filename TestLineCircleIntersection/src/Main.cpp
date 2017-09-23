/*
 * Main.cpp
 *
 *  Created on: 03/03/2016
 *      Author: rey
 */

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

using namespace biorobotics;

float computeDistanceEuclideanPointToLine(Eigen::Vector3d point,
		Segment segment) {
	Eigen::Vector3d u;
	Eigen::Vector3d p0;
	u(0, 0) = segment.v2.x - segment.v1.x;
	u(1, 0) = segment.v2.y - segment.v1.y;
	u(2, 0) = 0.0;
	p0(0, 0) = segment.v1.x;
	p0(1, 0) = segment.v1.y;
	p0(2, 0) = 0.0;
	Eigen::Vector3d ba = point - p0;
	return ba.cross(u).norm() / u.norm();
}

bool isInRangeProyection(Vertex2 vertex, Segment segment) {
	float xinf, xsup, yinf, ysup;
	if (segment.v1.x < segment.v2.x) {
		xinf = segment.v1.x;
		xsup = segment.v2.x;
	}
	if (segment.v1.y < segment.v2.y) {
		yinf = segment.v1.y;
		ysup = segment.v2.y;
	}
	if (vertex.x >= xinf && vertex.x <= xsup && vertex.y >= yinf
			&& vertex.y <= ysup)
		return true;
	else
		return false;
}

bool SegmentCircleIntersect(Circle circle, Segment segment) {
	bool result;
	float dx = segment.v2.x - segment.v1.x;
	float dy = segment.v2.y - segment.v1.y;
	float h = circle.center.x;
	float k = circle.center.x;
	float r = circle.ratio;
	float x0 = segment.v1.x;
	float y0 = segment.v1.y;

	float a = pow(dx, 2) + pow(dy, 2);
	float b = 2 * dx * (x0 - h) + 2 * dy * (y0 - k);
	float c = pow(x0 - (float) h, 2) + pow(y0 - (float) k, 2) - pow(r, 2);

	float disc = pow(b, 2) - 4 * a * c;

	if (disc == 0 || disc > 0) {
		float t1 = (-b + sqrt(disc)) / (2 * a);
		float t2 = (-b - sqrt(disc)) / (2 * a);

		float x1int = dx * t1 + x0;
		float y1int = dy * t1 + y0;
		float x2int = dx * t2 + x0;
		float y2int = dy * t2 + y0;

		if (disc == 0) {
			std::cout << "Tangent" << std::endl;
			result = isInRangeProyection(Vertex2(x1int, y1int), segment);
		} else if (disc > 0) {
			std::cout << "Intersection" << std::endl;
			result = isInRangeProyection(Vertex2(x1int, y1int), segment);
			if (!result)
				result = isInRangeProyection(Vertex2(x2int, y2int), segment);
		}
	} else if (disc < 0) {
		std::cout << "Not intersect" << std::endl;
	}
	return result;
}

int main(int argc, char ** argv) {
	Circle c(Vertex2(1.517f, 1.517f), 0.3);
	Segment s(Vertex2(4.14552021f, 2.92201996f),
			Vertex2(3.80464005f, 2.78146005f));

	bool test = SegmentCircleIntersect(c, s);
	std::cout << "test=" << test << std::endl;
	return 1;
}

