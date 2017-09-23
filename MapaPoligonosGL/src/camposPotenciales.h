/*
 * utilities.h
 *
 *  Created on: 04/03/2015
 *      Author: rey
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LARGEST_DISTANCE_SENSORS 5.0 // 5.0 meters
#define MAX_NUM_SENSORS 700
#define MAX_NUM_RAW_SONAR 100
#define MAX_NUM_RAW_INFRARED 100
#define MAX_NUM_POLYGONS 100
#define NUM_MAX_VERTEX 10
#define STRSIZ 300
#define SIZE_LINE 10000
#define NUM_LASERS 682
#define INIT_ANGLE_LASER   (float) (((15.8f*PI*2)/360) - 2.3562f) // see Hokuyo laser manual for this
#define COVER_ANGLE  ((360.0f/1024.0f)*681.0f)

#define Epsilon (float)0.03
#define ETHA (float)0.02
#define D0 (float)0.3
#define DELTA (float)0.04

typedef struct Vertex_ {
	float x;
	float y;
} Vertex;

typedef struct Line_ {
	float m;
	float b;
} Line;

typedef struct Polygon_ {
	char name[STRSIZ];
	char type[STRSIZ];
	int num_vertex;
	Vertex vertex[NUM_MAX_VERTEX];
	Line line[NUM_MAX_VERTEX];
} Polygon;

typedef struct _coord {
	float xc, yc, anglec;
} coord;

float getDeterminante(Vertex vertice1, Vertex vertice2, Vertex vertice3);
float getModule(coord coord1, coord coord2);
float getModule(coord coord);
float getCrossProduct(coord cord1, coord cord2);

float getArea(Polygon polygon) {
	int i;
	float area = 0;
	for (i = 1; i <= polygon.num_vertex; i++) {
		if (i <= polygon.num_vertex - 1) {
			area = area + polygon.vertex[i].x * polygon.vertex[i + 1].y
					- polygon.vertex[i + 1].x * polygon.vertex[i].y;
		} else {
			area = area
					+ polygon.vertex[polygon.num_vertex].x * polygon.vertex[1].y
					- polygon.vertex[1].x
							* polygon.vertex[polygon.num_vertex].y;
		}
	}
	return area / 2;
}

coord * getCentroid(int num_polygons, Polygon * polygons) {
	int i, j;
	float area;
	coord * centroids = (coord *) malloc(num_polygons * sizeof(coord));
	for (i = 1; i < num_polygons; i++) {
		coord centroid = { 0, 0 };
		area = getArea(polygons[i]);
		for (j = 1; j <= polygons[i].num_vertex; j++) {
			if (j <= polygons[i].num_vertex - 1) {
				centroid.xc =
						centroid.xc
								+ (polygons[i].vertex[j].x
										+ polygons[i].vertex[j + 1].x)
										* (polygons[i].vertex[j].x
												* polygons[i].vertex[j + 1].y
												- polygons[i].vertex[j + 1].x
														* polygons[i].vertex[j].y);
				centroid.yc =
						centroid.yc
								+ (polygons[i].vertex[j].y
										+ polygons[i].vertex[j + 1].y)
										* (polygons[i].vertex[j].x
												* polygons[i].vertex[j + 1].y
												- polygons[i].vertex[j + 1].x
														* polygons[i].vertex[j].y);
			} else {
				centroid.xc =
						centroid.xc
								+ (polygons[i].vertex[polygons[i].num_vertex].x
										+ polygons[i].vertex[1].x)
										* (polygons[i].vertex[polygons[i].num_vertex].x
												* polygons[i].vertex[1].y
												- polygons[i].vertex[1].x
														* polygons[i].vertex[polygons[i].num_vertex].y);
				centroid.yc =
						centroid.yc
								+ (polygons[i].vertex[polygons[i].num_vertex].y
										+ polygons[i].vertex[1].y)
										* (polygons[i].vertex[polygons[i].num_vertex].x
												* polygons[i].vertex[1].y
												- polygons[i].vertex[1].x
														* polygons[i].vertex[polygons[i].num_vertex].y);
			}
		}
		centroids[i].xc = centroid.xc / (6 * area);
		centroids[i].yc = centroid.yc / (6 * area);
	}
	return centroids;
}

coord * getGradientRepulsionObject(coord robot, coord obstaclePosition) {
	float module = getModule(obstaclePosition, robot);
	float x, y, scalar;
	coord * gradiente = (coord *) malloc(sizeof(coord));
	if (module <= D0) {
		x = robot.xc - obstaclePosition.xc;
		y = robot.yc - obstaclePosition.yc;
		scalar = 1 / module - 1 / D0;
		scalar = (-ETHA * scalar * (1 / pow(module, 3)));
		x = scalar * x;
		y = scalar * y;
		gradiente->xc = x;
		gradiente->yc = y;
	} else {
		gradiente->xc = 0;
		gradiente->yc = 0;
	}
	return gradiente;
}

coord * getFuerzasRepulsion(coord robot, Polygon * polygons, int num_polygons) {
	coord * centroides = getCentroid(num_polygons, polygons);
	coord * fuerza;
	coord * fuerzaTotal = (coord *) malloc(sizeof(coord));
	fuerzaTotal->xc = 0;
	fuerzaTotal->yc = 0;
	int i;
	for (i = 1; i < num_polygons; i++) {
		fuerza = getGradientRepulsionObject(robot, centroides[i]);
		fuerzaTotal->xc = fuerzaTotal->xc + fuerza->xc;
		fuerzaTotal->yc = fuerzaTotal->yc + fuerza->yc;
		free(fuerza);
	}
	return fuerzaTotal;
}

coord * getFuerzaAtraccion(coord robot, coord destino) {
	float modulo = getModule(robot, destino);
	coord * fuerzaAtraccion = (coord *) malloc(sizeof(coord));
	if (modulo <= D0) {
		fuerzaAtraccion->xc = Epsilon * (robot.xc - destino.xc);
		fuerzaAtraccion->yc = Epsilon * (robot.yc - destino.yc);
	} else {
		fuerzaAtraccion->xc = Epsilon / modulo * (robot.xc - destino.xc);
		fuerzaAtraccion->yc = Epsilon / modulo * (robot.yc - destino.yc);
	}
	return fuerzaAtraccion;
}

coord * getFuerzaTotal(coord robot, Polygon * polygons, int num_polygons,
		coord destino) {
	coord * fuerzaTotal = (coord *) malloc(sizeof(coord));
	coord * fuerzaAtraccion = getFuerzaAtraccion(robot, destino);
	coord * fuerzaRepulsion = getFuerzasRepulsion(robot, polygons,
			num_polygons);
	printf("fuerzaAtraccion P(%f,%f)\n", fuerzaAtraccion->xc,
			fuerzaAtraccion->yc);
	printf("fuerzaRepulsion P(%f,%f)\n", fuerzaRepulsion->xc,
			fuerzaRepulsion->yc);
	fuerzaTotal->xc = fuerzaAtraccion->xc + fuerzaRepulsion->xc;
	fuerzaTotal->yc = fuerzaAtraccion->yc + fuerzaRepulsion->yc;
	free(fuerzaAtraccion);
	free(fuerzaRepulsion);
	return fuerzaTotal;
}

float obtenerGiro(coord robot, coord destino) {
	float alphaFinal = atan((destino.yc - robot.yc) / (destino.xc - robot.xc));
	float giro;
	if (destino.xc < robot.xc && destino.yc >= robot.yc) {
		alphaFinal = 3.1416 + alphaFinal;
	} else if (destino.xc < robot.xc && destino.yc < robot.yc) {
		alphaFinal = 3.1416 + alphaFinal;
	} else if (destino.xc >= robot.xc && destino.yc < robot.yc) {
		alphaFinal = 6.2832 + alphaFinal;
	}
	printf("alphaFinal = %f\n", alphaFinal);
	printf("angR = %f\n", robot.anglec);
	if (alphaFinal >= robot.anglec) {
		if (alphaFinal - robot.anglec > 3.1416) {
			giro = -1 * (6.2832 - alphaFinal + robot.anglec);
		} else {
			giro = alphaFinal - robot.anglec;
		}
	} else if (alphaFinal < robot.anglec) {
		if (robot.anglec - alphaFinal > 3.1416) {
			giro = 6.2832 - robot.anglec + alphaFinal;
		} else {
			giro = -1 * (robot.anglec - alphaFinal);
		}

	}
	return giro;
}

float getModule(coord coord1, coord coord2) {
	return sqrt(pow(coord1.xc - coord2.xc, 2) + pow(coord1.yc - coord2.yc, 2));
}

float getModule(coord coord) {
	return sqrt(pow(coord.xc, 2) + pow(coord.yc, 2));
}

Polygon * getPolygonInClockwiseOrder(int num_polygons, Polygon * polygons) {
	int i, j, k;
	Polygon * polygonsInClockwiseOrder = (Polygon *) malloc(
			num_polygons * sizeof(Polygon));
	for (i = 0; i < num_polygons; i++) {
		j = 0;
		Vertex vertice1;
		Vertex vertice2;
		Vertex vertice3;
		int parallelVertex = 1;
		float determinate;
		do {
			vertice1 = polygons[i].vertex[j];
			vertice1 = polygons[i].vertex[j + 1];
			vertice1 = polygons[i].vertex[j + 1];
			determinate = getDeterminante(vertice1, vertice2, vertice3);
			if (determinate != 0) {
				parallelVertex = 0;
			}
			j++;
		} while (parallelVertex == 0);
		Polygon poligonInClockwiseOrder;
		if (determinate > 0)
			poligonInClockwiseOrder = polygons[i];
		else {
			//poligonInClockwiseOrder.type = polygons[i].type;
			poligonInClockwiseOrder.num_vertex = polygons[i].num_vertex;
			//poligonInClockwiseOrder.name = polygons[i].name;
			//poligonInClockwiseOrder.line = polygons[i].line;
			for (k = 0; k < polygons[i].num_vertex; k++) {
				poligonInClockwiseOrder.vertex[k] =
						polygons[i].vertex[polygons[i].num_vertex - k - 1];
			}
		}
		polygonsInClockwiseOrder[i] = poligonInClockwiseOrder;
	}
	return polygonsInClockwiseOrder;
}

float getDeterminante(Vertex vertice1, Vertex vertice2, Vertex vertice3) {
	float determinante = (vertice2.x - vertice1.x) * (vertice3.y - vertice2.y)
			- (vertice2.y - vertice1.y) * (vertice3.x - vertice2.x);
	return determinante;
}

float getCrossProduct(coord cord1, coord cord2) {
	return cord1.xc * cord2.yc - cord2.xc * cord1.yc;
}
