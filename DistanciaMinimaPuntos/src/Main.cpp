#include "DistanceMin.h"
#include "Utility.h"

/*
 * DistanciaMaxima.cpp
 *
 *  Created on: 01/04/2015
 *      Author: rey
 */

int main(int argc, char** argv) {
	/* Se obtienen el vector con los puntos*/
	std::vector<DistanceMin::Punto> puntosConvert = parserPointsFormFile(
			"coordenadas.txt");

	DistanceMin::Punto * puntosEspacio = (DistanceMin::Punto *) malloc(
			sizeof(DistanceMin::Punto) * puntosConvert.size());

	/* Como el algoritmo trabaja con apuntadores y la apartura del archivo te regresa un vector de puntos
	 * Se crea el apuntador al arreglo de puntos
	 */
	for (unsigned int i = 0; i < puntosConvert.size(); i++) {
		(puntosEspacio + i)->x = puntosConvert[i].x;
		(puntosEspacio + i)->y = puntosConvert[i].y;
	}

	DistanceMin::Punto * ordenX = (DistanceMin::Punto *) malloc(
			sizeof(DistanceMin::Punto));
	DistanceMin::Punto * ordenY = (DistanceMin::Punto *) malloc(
			sizeof(DistanceMin::Punto));

	/*Se ordenan respecto X*/
	quicksort(puntosEspacio, 0, puntosConvert.size() - 1, 2);

	/*Se guarda en un arreglo las puntos ordenados en X*/
	for (unsigned int i = 0; i < puntosConvert.size(); i++) {
		ordenX = (DistanceMin::Punto *) realloc(ordenX,
				sizeof(DistanceMin::Punto) * (i + 1));
		*(ordenX + i) = puntosEspacio[i];
	}
	/*Se ordenan los puntos respecto a Y*/
	quicksort(puntosEspacio, 0, puntosConvert.size() - 1, 1);

	/*Se guarda en un arreglo las puntos ordenados en Y*/
	for (unsigned int i = 0; i < puntosConvert.size(); i++) {
		ordenY = (DistanceMin::Punto *) realloc(ordenY,
				sizeof(DistanceMin::Punto) * (i + 1));
		*(ordenY + i) = puntosEspacio[i];
	}

	DistanceMin::DistanceMin nearestPair;
	/*Se calcula la pareja de puntos mas cercanos*/
	DistanceMin::Minimo minimo = nearestPair.getNearestPair(ordenX, ordenY,
			puntosConvert.size());
	printf("Minimo d %f\n", minimo.d);
	printf("Minimo P1(%f,%f)\n", minimo.p1.x, minimo.p1.y);
	printf("Minimo P2(%f,%f)\n", minimo.p2.x, minimo.p2.y);

}
