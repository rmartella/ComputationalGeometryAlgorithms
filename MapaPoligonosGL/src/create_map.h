#include "camposPotenciales.h"
#include <stdio.h>
#include <math.h>

#define scale 1.75

void getParallelLinesToDirection(Vertex vertex1, Vertex vertex2, float ratio,
		Vertex * vertexOutputL11, Vertex * vertexOutputL12,
		Vertex * vertexOutputL21, Vertex * vertexOutputL22);
bool isIntersect(Vertex point1Segment1, Vertex point2Segment1,
		Vertex point1Segment2, Vertex point2Segment2);
bool testToPathDirect(Vertex inicio, Vertex fin, int num_polygons,
		Polygon * polygons, Polygon * grownPolygons);
void addNodesInitEndToMap(Vertex inicio, Vertex fin, int num_polygons,
		Polygon * polygons, Polygon * grownPolygons, bool ** adyacencias,
		float ratio);

Polygon * grownPoligons(int num_polygons, float radioRobot, Polygon *polygons,
		Polygon *grownPolygons) {

	grownPolygons = (Polygon *) realloc(grownPolygons,
			sizeof(Polygon) * (num_polygons + 1));

	coord * centroides = getCentroid(num_polygons, polygons);
	for (int i = 1; i < num_polygons; i++) {
		Polygon * newPolygon = (Polygon *) malloc(sizeof(Polygon));
		newPolygon->num_vertex = polygons[i].num_vertex;
		for (int j = 1; j <= polygons[i].num_vertex; j++) {
			newPolygon->vertex[j].x = scale
					* (polygons[i].vertex[j].x - centroides[i].xc)
					+ centroides[i].xc;
			newPolygon->vertex[j].y = scale
					* (polygons[i].vertex[j].y - centroides[i].yc)
					+ centroides[i].yc;

		}
		grownPolygons[i] = *newPolygon;

	}
	return grownPolygons;
}

/*bool isPolygonIntersect(Polygon polygon1, Polygon polygon2) {
 for (int i = 1; i <= polygon1.num_vertex; i++) {
 for (int j = 1; j <= polygon2.num_vertex; j++) {

 }
 polygon1.vertex[i].x = polygon2.vertex[i]
 }
 }*/

/*void getMapTopologic(int num_polygons, Polygon * polygons,
 Polygon * grownPolygons, float ratio, Vertex * cabeceras,
 int ** adyacencias) {*/
bool ** getMapTopologic(int num_polygons, Polygon * polygons,
		Polygon * grownPolygons, float ratio) {

	bool ** adyacencias = (bool **) malloc(
			(num_polygons + 2) * NUM_MAX_VERTEX * sizeof(bool *));

	for (int i = 0; i < (num_polygons + 2) * NUM_MAX_VERTEX; i++) {
		(adyacencias[i] = (bool *) malloc(
				(num_polygons + 2) * NUM_MAX_VERTEX * sizeof(bool)));
	}

	/*bool adyacencias[(num_polygons - 4) * polygons[0].num_vertex][(num_polygons
	 - 4) * polygons[0].num_vertex];*/

	for (int i = 0; i < (num_polygons + 2) * NUM_MAX_VERTEX; i++) {
		for (int j = 0; j < (num_polygons + 2) * NUM_MAX_VERTEX; j++)
			adyacencias[i][j] = 0;
	}

	for (int h = 1; h < num_polygons; h++) {
		Polygon polygon1 = grownPolygons[h];
		for (int i = h; i < num_polygons; i++) {
			Polygon polygon2 = grownPolygons[i];
			for (int j = 1; j <= polygon1.num_vertex; j++) {
				Vertex vertex1 = polygon1.vertex[j];
				for (int k = 1; k <= polygon2.num_vertex; k++) {
					Vertex vertex2 = polygon2.vertex[k];
					if (vertex1.x == vertex2.x && vertex1.y == vertex2.y) {
						continue;
					}
					Vertex * vertexOutputL11 = (Vertex *) malloc(
							sizeof(Vertex));
					Vertex * vertexOutputL12 = (Vertex *) malloc(
							sizeof(Vertex));
					Vertex * vertexOutputL21 = (Vertex *) malloc(
							sizeof(Vertex));
					Vertex * vertexOutputL22 = (Vertex *) malloc(
							sizeof(Vertex));
					getParallelLinesToDirection(vertex1, vertex2, ratio,
							vertexOutputL11, vertexOutputL12, vertexOutputL21,
							vertexOutputL22);
					bool isIntersectBool = false;
					for (int l = 1; l < num_polygons; l++) {
						Polygon polygonTest = polygons[l];
						for (int m = 1; m <= polygonTest.num_vertex; m++) {
							Vertex verticeTest1 = polygonTest.vertex[m];
							Vertex verticeTest2;
							if (m < polygonTest.num_vertex)
								verticeTest2 = polygonTest.vertex[m + 1];
							else
								verticeTest2 = polygonTest.vertex[1];
							isIntersectBool = isIntersect(*vertexOutputL11,
									*vertexOutputL12, verticeTest1,
									verticeTest2);
							if (isIntersectBool)
								break;
							else {
								isIntersectBool = isIntersect(*vertexOutputL21,
										*vertexOutputL22, verticeTest1,
										verticeTest2);
							}
							if (isIntersectBool)
								break;
						}
						if (isIntersectBool) {
							break;
						}
					}
					if (!isIntersectBool) {
						/*printf("%f,%f\n%f,%f\n", polygons[h].vertex[j].x,
						 polygons[h].vertex[j].y,
						 polygons[i].vertex[k].x,
						 polygons[i].vertex[k].y);*/
						adyacencias[(h - 0) * NUM_MAX_VERTEX + j - 1][(i - 0)
								* NUM_MAX_VERTEX + k - 1] = 1;
						adyacencias[(i - 0) * NUM_MAX_VERTEX + k - 1][(h - 0)
								* NUM_MAX_VERTEX + j - 1] = 1;
						/*printf("%f,%f\n%f,%f\n", polygon1.vertex[j].x,
						 polygon1.vertex[j].y, polygon2.vertex[k].x,
						 polygon2.vertex[k].y);*/
					}
					free(vertexOutputL11);
					free(vertexOutputL12);
					free(vertexOutputL21);
					free(vertexOutputL22);
				}
			}
		}
	}
	return adyacencias;
}

void getParallelLinesToDirection(Vertex vertex1, Vertex vertex2, float ratio,
		Vertex * vertexOutputL11, Vertex * vertexOutputL12,
		Vertex * vertexOutputL21, Vertex * vertexOutputL22) {
	float delX = vertex2.x - vertex1.x;
	float delY = vertex2.y - vertex1.y;
	if (delX == 0) {
		vertexOutputL11->x = vertex1.x - ratio;
		vertexOutputL11->y = vertex1.y;
		vertexOutputL21->x = vertex1.x + ratio;
		vertexOutputL21->y = vertex1.y;
		vertexOutputL12->x = vertex2.x - ratio;
		vertexOutputL12->y = vertex2.y;
		vertexOutputL22->x = vertex2.x + ratio;
		vertexOutputL22->y = vertex2.y;
	} else if (delY < 0.01) {
		vertexOutputL11->x = vertex1.x;
		vertexOutputL11->y = vertex1.y - ratio;
		vertexOutputL21->x = vertex1.x;
		vertexOutputL21->y = vertex1.y + ratio;
		vertexOutputL12->x = vertex2.x;
		vertexOutputL12->y = vertex2.y - ratio;
		vertexOutputL22->x = vertex2.x;
		vertexOutputL22->y = vertex2.y + ratio;
	} else {
		float inverPendient = -delX / delY;
		float xaux1 = vertex1.x - 10 * ratio;
		float xaux2 = vertex1.x + 10 * ratio;
		float yaux1 = inverPendient * (xaux1 - vertex1.x) + vertex1.y;
		float yaux2 = inverPendient * (xaux2 - vertex1.x) + vertex1.y;
		float a = pow(xaux2 - xaux1, 2) + pow(yaux2 - yaux1, 2);
		float b = 2 * (xaux2 - xaux1) * (xaux1 - vertex1.x)
				+ 2 * (yaux2 - yaux1) * (yaux1 - vertex1.y);
		float c = pow(xaux1 - vertex1.x, 2) + pow(yaux1 - vertex1.y, 2)
				- pow(ratio, 2);
		float t1 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
		float t2 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
		float x1int = (xaux2 - xaux1) * t1 + xaux1;
		float y1int = (yaux2 - yaux1) * t1 + yaux1;
		float x2int = (xaux2 - xaux1) * t2 + xaux1;
		float y2int = (yaux2 - yaux1) * t2 + yaux1;
		vertexOutputL11->x = x1int;
		vertexOutputL11->y = y1int;
		vertexOutputL21->x = x2int;
		vertexOutputL21->y = y2int;

		xaux1 = vertex2.x - 10 * ratio;
		xaux2 = vertex2.x + 10 * ratio;
		yaux1 = inverPendient * (xaux1 - vertex2.x) + vertex2.y;
		yaux2 = inverPendient * (xaux2 - vertex2.x) + vertex2.y;
		a = pow(xaux2 - xaux1, 2) + pow(yaux2 - yaux1, 2);
		b = 2 * (xaux2 - xaux1) * (xaux1 - vertex2.x)
				+ 2 * (yaux2 - yaux1) * (yaux1 - vertex2.y);
		c = pow(xaux1 - vertex2.x, 2) + pow(yaux1 - vertex2.y, 2)
				- pow(ratio, 2);
		t1 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
		t2 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
		x1int = (xaux2 - xaux1) * t1 + xaux1;
		y1int = (yaux2 - yaux1) * t1 + yaux1;
		x2int = (xaux2 - xaux1) * t2 + xaux1;
		y2int = (yaux2 - yaux1) * t2 + yaux1;
		vertexOutputL12->x = x1int;
		vertexOutputL12->y = y1int;
		vertexOutputL22->x = x2int;
		vertexOutputL22->y = y2int;
	}
}
bool isIntersect(Vertex point1Segment1, Vertex point2Segment1,
		Vertex point1Segment2, Vertex point2Segment2) {
//bool isIntersect(Segment segmento1, Segment segmento2) {
	float determinante1 = (point2Segment1.x - point1Segment1.x)
			* (point1Segment2.y - point2Segment1.y)
			- (point2Segment1.y - point1Segment1.y)
					* (point1Segment2.x - point2Segment1.x);
	float determinante2 = (point2Segment1.x - point1Segment1.x)
			* (point2Segment2.y - point2Segment1.y)
			- (point2Segment1.y - point1Segment1.y)
					* (point2Segment2.x - point2Segment1.x);
	if ((determinante1 < 0 && determinante2 > 0)
			|| (determinante1 > 0 && determinante2 < 0)) {
		determinante1 = (point2Segment2.x - point1Segment2.x)
				* (point1Segment1.y - point2Segment2.y)
				- (point2Segment2.y - point1Segment2.y)
						* (point1Segment1.x - point2Segment2.x);
		determinante2 = (point2Segment2.x - point1Segment2.x)
				* (point2Segment1.y - point2Segment2.y)
				- (point2Segment2.y - point1Segment2.y)
						* (point2Segment1.x - point2Segment2.x);

		if ((determinante1 < 0 && determinante2 > 0)
				|| (determinante1 > 0 && determinante2 < 0))
			return true;
		else
			return false;
	} else if (determinante1 == 0 || determinante2 == 0)
		return true;
	else
		return false;
}

bool testToPathDirect(Vertex inicio, Vertex fin, int num_polygons,
		Polygon * polygons, Polygon * grownPolygons) {
	bool pathFree = true;

	for (int i = 1; i < num_polygons && pathFree; i++) {
		for (int j = 1; j <= grownPolygons[i].num_vertex && pathFree; j++) {
			if (j < grownPolygons[i].num_vertex) {
				Vertex vertex1 = grownPolygons[i].vertex[j];
				Vertex vertex2 = grownPolygons[i].vertex[j + 1];
				pathFree = !isIntersect(inicio, fin, vertex1, vertex2);
			} else {
				Vertex vertex1 = grownPolygons[i].vertex[j];
				Vertex vertex2 = grownPolygons[i].vertex[1];
				pathFree = !isIntersect(inicio, fin, vertex1, vertex2);
			}
		}
	}
	return pathFree;
}

void addNodesInitEndToMap(Vertex inicio, Vertex fin, int num_polygons,
		Polygon * polygons, Polygon * grownPolygons, bool ** adyacencias,
		float ratio) {
	for (int i = 1; i < num_polygons; i++) {
		for (int j = 1; j <= grownPolygons[i].num_vertex; j++) {
			Vertex vertexTargetBind = grownPolygons[i].vertex[j];
			Vertex * vertexOutputIL11 = (Vertex *) malloc(sizeof(Vertex));
			Vertex * vertexOutputIL12 = (Vertex *) malloc(sizeof(Vertex));
			Vertex * vertexOutputIL21 = (Vertex *) malloc(sizeof(Vertex));
			Vertex * vertexOutputIL22 = (Vertex *) malloc(sizeof(Vertex));
			Vertex * vertexOutputFL11 = (Vertex *) malloc(sizeof(Vertex));
			Vertex * vertexOutputFL12 = (Vertex *) malloc(sizeof(Vertex));
			Vertex * vertexOutputFL21 = (Vertex *) malloc(sizeof(Vertex));
			Vertex * vertexOutputFL22 = (Vertex *) malloc(sizeof(Vertex));
			getParallelLinesToDirection(inicio, vertexTargetBind, 0.03,
					vertexOutputIL11, vertexOutputIL12, vertexOutputIL21,
					vertexOutputIL22);
			getParallelLinesToDirection(fin, vertexTargetBind, 0.03,
					vertexOutputFL11, vertexOutputFL12, vertexOutputFL21,
					vertexOutputFL22);
			bool isIntersectInicioBool = false;
			bool isIntersectFinBool = false;
			for (int k = 1;
					k < num_polygons
							&& (!isIntersectInicioBool || !isIntersectFinBool);
					k++) {
				for (int l = 1;
						l <= grownPolygons[k].num_vertex
								&& (!isIntersectInicioBool
										|| !isIntersectFinBool); l++) {
					/*if (k != i
					 || (k == i && j != l
					 && ((j > 1 && l != j - 1 && l != j + 1)
					 || (j == 1
					 && l
					 < grownPolygons[k].num_vertex)))) {*/
					if (l < grownPolygons[k].num_vertex) {
						if (!isIntersectInicioBool) {
							isIntersectInicioBool = isIntersect(
									*vertexOutputIL11, *vertexOutputIL12,
									polygons[k].vertex[l],
									polygons[k].vertex[l + 1]);
							if (!isIntersectInicioBool)
								isIntersectInicioBool = isIntersect(
										*vertexOutputIL21, *vertexOutputIL22,
										polygons[k].vertex[l],
										polygons[k].vertex[l + 1]);
						}
						if (!isIntersectFinBool) {
							isIntersectFinBool = isIntersect(*vertexOutputFL11,
									*vertexOutputFL12, polygons[k].vertex[l],
									polygons[k].vertex[l + 1]);
							if (!isIntersectFinBool)
								isIntersectFinBool = isIntersect(
										*vertexOutputFL21, *vertexOutputFL22,
										polygons[k].vertex[l],
										polygons[k].vertex[l + 1]);
						}
					} else {
						if (!isIntersectInicioBool) {
							isIntersectInicioBool = isIntersect(
									*vertexOutputIL11, *vertexOutputIL12,
									polygons[k].vertex[l],
									polygons[k].vertex[1]);
							if (!isIntersectInicioBool)
								isIntersectInicioBool = isIntersect(
										*vertexOutputIL21, *vertexOutputIL22,
										polygons[k].vertex[l],
										polygons[k].vertex[1]);
						}
						if (!isIntersectFinBool) {
							isIntersectFinBool = isIntersect(*vertexOutputFL11,
									*vertexOutputFL12, polygons[k].vertex[l],
									polygons[k].vertex[1]);
							if (!isIntersectFinBool)
								isIntersectFinBool = isIntersect(
										*vertexOutputFL21, *vertexOutputFL22,
										polygons[k].vertex[l],
										polygons[k].vertex[1]);
						}
						//}
					}
				}
			}
			if (!isIntersectInicioBool) {
				/*printf("%f,%f\n%f,%f\n", inicio.x, inicio.y, vertexTargetBind.x,
				 vertexTargetBind.y);*/
				adyacencias[(i - 0) * NUM_MAX_VERTEX + j - 1][0] = 1;
				adyacencias[0][(i - 0) * NUM_MAX_VERTEX + j - 1] = 1;
			}
			if (!isIntersectFinBool) {
				/*printf("%f,%f\n%f,%f\n", fin.x, fin.y, vertexTargetBind.x,
				 vertexTargetBind.y);
				 /*printf("%f,%f\n%f,%f\n", fin.x, fin.y,
				 polygons[i].vertex[j].x, polygons[i].vertex[j].y);*/
				adyacencias[(i - 0) * NUM_MAX_VERTEX + j - 1][(num_polygons - 0)
						* NUM_MAX_VERTEX] = 1;
				adyacencias[(num_polygons - 0) * NUM_MAX_VERTEX][(i - 0)
						* NUM_MAX_VERTEX + j - 1] = 1;
			}
			free(vertexOutputIL11);
			free(vertexOutputIL12);
			free(vertexOutputIL21);
			free(vertexOutputIL22);
			free(vertexOutputFL11);
			free(vertexOutputFL12);
			free(vertexOutputFL21);
			free(vertexOutputFL22);
		}
	}
	bool pathFree = testToPathDirect(inicio, fin, num_polygons, polygons,
			grownPolygons);
	if (pathFree) {
		adyacencias[(num_polygons - 0) * NUM_MAX_VERTEX][0] = 1;
		adyacencias[0][(num_polygons - 0) * NUM_MAX_VERTEX] = 1;
	}
}

float getDistanceEuclidean(Vertex vertice1, Vertex vertice2) {
	return sqrt(
			pow(vertice1.x - vertice2.x, 2) + pow(vertice1.y - vertice2.y, 2));
}
