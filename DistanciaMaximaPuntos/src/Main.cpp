#include "ConvexHull.h"
#include "Utility.h"
#include "DistanceMax.h"

/*
 * DistanciaMaxima.cpp
 *
 *  Created on: 01/04/2015
 *      Author: rey
 */

int main(int argc, char** argv) {
	/* Se obtienen el vector con los puntos*/
	std::vector<ConvexHull::Punto> puntosConvert = parserPointsFormFile(
			"coordenadas.txt");
	if (puntosConvert.size() > 2) {
		ConvexHull::ConvexHull algoConvex = ConvexHull::ConvexHull(
				puntosConvert);
		/* Se calcula el cierre convexo*/
		ConvexHull::ptrPila pila = algoConvex.calConvexHull();
		std::vector<ConvexHull::Punto> pointsConvexHullVector;
		/* Se vacia la pila que contiene el cierre convexo en un vector*/
		while (ConvexHull::ConvexHull::pila_vacia(&pila) != 1) {
			pointsConvexHullVector.push_back(
					ConvexHull::ConvexHull::pop(&pila));
		}

		DistanceMax::DistanceMax algoDistance = DistanceMax::DistanceMax(
				pointsConvexHullVector);
		/* Se obtiene la pareja de puntos mas distantes*/
		std::vector<ConvexHull::Punto> pointsMaxDistance =
				algoDistance.getPointsMaxDistance();

		printf("Puntos mas distantes\n");

		for (unsigned int i = 0; i < pointsMaxDistance.size(); i++)
			printf("P%d(%f,%f)\n", i + 1, pointsMaxDistance[i].x,
					pointsMaxDistance[i].y);
	} else {
		printf("Puntos mas distantes\n");
		if (puntosConvert.size() > 0) {
			if (puntosConvert.size() == 1) {
				printf("P%d(%f,%f)\n", 1, puntosConvert[0].x,
						puntosConvert[0].y);
			}
			if (puntosConvert.size() == 2) {
				printf("P%d(%f,%f)\n", 1, puntosConvert[0].x,
						puntosConvert[0].y);
				printf("P%d(%f,%f)\n", 2, puntosConvert[1].x,
						puntosConvert[1].y);
			}
		}
	}

}
