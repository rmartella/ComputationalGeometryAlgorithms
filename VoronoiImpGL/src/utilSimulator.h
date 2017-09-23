/*
 * utilSimulator.h
 *
 *  Created on: 07/09/2015
 *      Author: rey
 */

#ifndef UTILSIMULATOR_H_
#define UTILSIMULATOR_H_

#include "definition.h"

enum TypeMotion {
	STOP, FORWARD, BACKWARD, LEFT, RIGHT
};

namespace biorobotics {

float getDeterminant(Vertex2 vertex1, Vertex2 vertex2, Vertex2 vertex3) {
	float determinant = (vertex2.x - vertex1.x) * (vertex3.y - vertex2.y)
			- (vertex2.y - vertex1.y) * (vertex3.x - vertex2.x);
	return determinant;
}

void getConstantMovement(float xf, float tf, float * a3, float * a2) {
	*a3 = -2 * xf / pow(tf, 3);
	*a2 = 3 * xf / pow(tf, 2);
}

float getNextPosition(float a3, float a2, float t) {
	float xt = a2 * pow(t, 2) + a3 * pow(t, 3);
	return xt;
}

float computeArea(Polygon polygon) {
	int i;
	float area = 0;
	for (i = 0; i < polygon.num_vertex; i++) {
		if (i < polygon.num_vertex - 1) {
			area = area + polygon.vertex[i].x * polygon.vertex[i + 1].y
					- polygon.vertex[i + 1].x * polygon.vertex[i].y;
		} else {
			area = area
					+ polygon.vertex[polygon.num_vertex - 1].x
							* polygon.vertex[0].y
					- polygon.vertex[0].x
							* polygon.vertex[polygon.num_vertex - 1].y;
		}
	}
	return area / 2;
}

std::vector<Vertex2> computeCentroids(std::vector<Polygon> polygons) {
	int i, j;
	float area;
	std::vector<Vertex2> centroids;
	for (i = 0; i < polygons.size(); i++) {
		Vertex2 centroid = Vertex2::Zero();
		area = computeArea(polygons[i]);
		for (j = 0; j < polygons[i].num_vertex; j++) {
			if (j < polygons[i].num_vertex - 1) {
				centroid.x =
						centroid.x
								+ (polygons[i].vertex[j].x
										+ polygons[i].vertex[j + 1].x)
										* (polygons[i].vertex[j].x
												* polygons[i].vertex[j + 1].y
												- polygons[i].vertex[j + 1].x
														* polygons[i].vertex[j].y);
				centroid.y =
						centroid.y
								+ (polygons[i].vertex[j].y
										+ polygons[i].vertex[j + 1].y)
										* (polygons[i].vertex[j].x
												* polygons[i].vertex[j + 1].y
												- polygons[i].vertex[j + 1].x
														* polygons[i].vertex[j].y);
			} else {
				centroid.x =
						centroid.x
								+ (polygons[i].vertex[polygons[i].num_vertex - 1].x
										+ polygons[i].vertex[0].x)
										* (polygons[i].vertex[polygons[i].num_vertex
												- 1].x * polygons[i].vertex[0].y
												- polygons[i].vertex[0].x
														* polygons[i].vertex[polygons[i].num_vertex
																- 1].y);
				centroid.y =
						centroid.y
								+ (polygons[i].vertex[polygons[i].num_vertex - 1].y
										+ polygons[i].vertex[0].y)
										* (polygons[i].vertex[polygons[i].num_vertex
												- 1].x * polygons[i].vertex[0].y
												- polygons[i].vertex[0].x
														* polygons[i].vertex[polygons[i].num_vertex
																- 1].y);
			}
		}
		centroid.x = centroid.x / (6 * area);
		centroid.y = centroid.y / (6 * area);
		centroids.push_back(centroid);
	}
	return centroids;
}

} /* namespace biorobotics */

#endif /* UTILSIMULATOR_H_ */

