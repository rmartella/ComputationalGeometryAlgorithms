/*
 * DistanceMin.h
 *
 *  Created on: 12/04/2015
 *      Author: rey
 */
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef DISTANCEMIN_H_
#define DISTANCEMIN_H_

namespace DistanceMin {

typedef struct _punto {
	float x, y;
} Punto;

typedef struct _minimo {
	Punto p1, p2;
	float d;
} Minimo;

typedef struct _intervalo {
	int i, j;
} Intervalo;

class DistanceMin {
public:
	DistanceMin();
	Minimo getNearestPair(Punto * listPointsOrderX, Punto * listPointsOrderY,
			int lenghtPoints);
	Minimo bruteForce(Punto * listPoints, int lenghtPoints);
	void split(Punto * listPointsOrderX, Punto * listPointsOrderY,
			int lenghtPoints, Punto ** lx, Punto ** ly, int * tamL, Punto ** rx,
			Punto ** ry, int * tamR);
	void filter(Punto * listPoints, int lenghtPoints, float d, float xPart,
			float yPartMin, float yPartMax, Punto ** strip,
			int * lenghtPointsStrip);
	Minimo test(int p, Punto * strip, int lenghtPointsStrip);
	virtual ~DistanceMin();
};

} /* namespace UtilInt */

#endif /* DISTANCEMIN_H_ */
