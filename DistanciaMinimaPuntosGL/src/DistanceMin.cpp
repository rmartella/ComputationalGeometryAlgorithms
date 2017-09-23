/*
 * DistanceMin.cpp
 *
 *  Created on: 12/04/2015
 *      Author: rey
 */

#include "DistanceMin.h"

namespace DistanceMin {

DistanceMin::DistanceMin() {
}

/* Método que obtiene la pareja de puntos mas cercanos, la complejidad es O(nlogn) por que particiona el arreglo en dos
 * Esta función toma los siguientes parametros:
 * listPointsOrderX: Puntos ordenados en X
 * listPointsOrderY: Puntos ordenados en y
 * lenghtPoints: longitud del arreglo
 */
Minimo DistanceMin::getNearestPair(Punto * listPointsOrderX,
		Punto * listPointsOrderY, int lenghtPoints) {
	if (lenghtPoints < 3)
		return bruteForce(listPointsOrderX, lenghtPoints);
	Punto * lx = (Punto *) malloc(sizeof(Punto)), *ly = (Punto *) malloc(
			sizeof(Punto)), *rx = (Punto *) malloc(sizeof(Punto)), *ry =
			(Punto *) malloc(sizeof(Punto)), *strip = (Punto *) malloc(
			sizeof(Punto));
	int * tamL = (int *) malloc(sizeof(int)), *tamR = (int *) malloc(
			sizeof(int)), *tamStrip = (int *) malloc(sizeof(int));
	*tamL = 0, *tamR = 0, *tamStrip = 0;

	/*printf("X [");
	for (int i = 0; i < lenghtPoints; i++) {
		printf("(%f,%f),", listPointsOrderX[i].x, listPointsOrderX[i].y);
	}
	printf("]\n");

	printf("Y [");
	for (int i = 0; i < lenghtPoints; i++) {
		printf("(%f,%f),", listPointsOrderY[i].x, listPointsOrderY[i].y);
	}
	printf("]\n");*/

	/* Separa la lista de puntos, se mantiene ordeados en eje x y y, la separación*/
	split(listPointsOrderX, listPointsOrderY, lenghtPoints, &lx, &ly, tamL, &rx,
			&ry, tamR);

	/* Se hace el llamado recursivo con la parte izquierda*/
	Minimo dl = getNearestPair(lx, ly, *tamL);
	/* Se hace el llamado recursivo con la parte derecha*/
	Minimo dr = getNearestPair(rx, ry, *tamR);
	/* Nos quedamos con el minimo*/
	Minimo d = dl.d < dr.d ? dl : dr;
	float xPart = lx[*tamL - 1].x + (rx[0].x - lx[*tamL - 1].x) / 2;
	float yPartMin = listPointsOrderY[0].y;
	float yPartMax = listPointsOrderY[lenghtPoints - 1].y;
	/* Hacemos el filtro para obtener la ventana e ir comparadon por cuadritos*/
	filter(listPointsOrderY, lenghtPoints, d.d, xPart, yPartMin, yPartMax,
			&strip, tamStrip);
	for (int i = 0; i < *tamStrip; i++) {
		/* Se prueban los puntos con sus 11 vecinos por arriba y sus 11 vecinos por abajo*/
		Minimo d_ = test(i, strip, *tamStrip);
		/* Obtenemos el minimo*/
		d = d_.d < d.d ? d_ : d;
	}
	return d;
}

DistanceMin::~DistanceMin() {
// TODO Auto-generated destructor stub
}

/*Se prueba el algortimo de fuerza bruta para menos de tres puntos*/
Minimo DistanceMin::bruteForce(Punto * listPoints, int lenghtPoints) {
	Minimo minimo;
	if (lenghtPoints == 2) {
		float d = sqrt(
				pow(listPoints[0].x - listPoints[1].x, 2)
						+ pow(listPoints[0].y - listPoints[1].y, 2));
		minimo = Minimo { listPoints[0], listPoints[1], d };
	} else
		minimo = Minimo { listPoints[0], listPoints[1], 99999999999.999 };
	return minimo;
}

/* Particiona el arreglo en dos y mantiene ordenados los puntos en X y Y*/
void DistanceMin::split(Punto * listPointsOrderX, Punto * listPointsOrderY,
		int lenghtPoints, Punto ** lx, Punto ** ly, int * tamL, Punto ** rx,
		Punto ** ry, int * tamR) {
	int tamParticion = lenghtPoints / 2 + lenghtPoints % 2;
	float xMin, xMax, xTop, yTop;
	xMin = listPointsOrderX[0].x;
	xMax = listPointsOrderX[0].x;
	xTop = listPointsOrderX[tamParticion - 1].x;
	yTop = listPointsOrderX[tamParticion - 1].y;
	//printf("(xTop,yTop) (%f,%f)\n", xTop, yTop);
	/*Se obtiene la particion L y R ordenada en X*/
	for (int i = 0; i < lenghtPoints; i++) {
		if (i < tamParticion) {
			if (listPointsOrderX[i].x < xMin)
				xMin = listPointsOrderX[i].x;
			if (listPointsOrderX[i].x > xMax)
				xMax = listPointsOrderX[i].x;
			*lx = (Punto *) realloc(*lx, sizeof(Punto) * (i + 1));
			*(*lx + i) = listPointsOrderX[i];
			*tamL = *tamL + 1;
		} else {
			*rx = (Punto *) realloc(*rx,
					sizeof(Punto) * (i - tamParticion + 1));
			*(*rx + i - tamParticion) = listPointsOrderX[i];
			*tamR = *tamR + 1;
		}
	}
	/*Se obtiene la particion L ordenada en Y*/
	for (int i = 0, j = 0, k = 0; i < lenghtPoints; i++) {
		if ((listPointsOrderY[i].x < xTop
				|| (listPointsOrderY[i].x == xTop
						&& listPointsOrderY[i].y <= yTop)) && j < *tamL) {
			*ly = (Punto *) realloc(*ly, sizeof(Punto) * (j + 1));
			*(*ly + j) = listPointsOrderY[i];
			j++;
		} else if (k < *tamR) {
			*ry = (Punto *) realloc(*ry, sizeof(Punto) * (k + 1));
			*(*ry + k) = listPointsOrderY[i];
			k++;
		}

	}
	/*Se obtiene la particion R ordenada en Y*/
	/**ry = (Punto *) malloc(sizeof(Punto) * (*tamR));
	 if (*tamR != 1) {
	 for (int i = lenghtPoints - 1, k = *tamR - 1; i >= 0; i--) {
	 if (!(listPointsOrderY[i].x >= xMin && listPointsOrderY[i].x <= xMax)
	 && k >= 0) {
	 *(*ry + k) = listPointsOrderY[i];
	 k--;
	 }
	 }
	 } else
	 *(*ry) = *(*rx);*/
}

/*Filtra los puntos a probar respecto a su distancia minima en el eje x */
void DistanceMin::filter(Punto * listPoints, int lenghtPoints, float d,
		float xPart, float yPartMin, float yPartMax, Punto ** strip,
		int * lenghtPointsStrip) {
	float limiteInferior = xPart - d;
	float limiteSuperior = xPart + d;
	for (int i = 0; i < lenghtPoints; i++) {
		if (listPoints[i].x >= limiteInferior
				|| listPoints[i].x <= limiteSuperior) {
			*strip = (Punto *) realloc(*strip,
					sizeof(Punto) * (*lenghtPointsStrip + 1));
			*(*strip + *lenghtPointsStrip) = listPoints[i];
			*lenghtPointsStrip = *lenghtPointsStrip + 1;
		}
	}
}

/* Método que prueba los 10 vecinos por debajo y los 10 por arriba*/
Minimo DistanceMin::test(int p, Punto * strip, int lenghtPointsStrip) {
	int i = 1;
	float minD = 0;
	Punto p1 = strip[p], p2;
	while (i <= 11 && (p - i) >= 0) {
		float d = sqrt(
				pow(strip[p - i].x - p1.x, 2) + pow(strip[p - i].y - p1.y, 2));
		if (minD == 0 || minD > 0 && d < minD) {
			minD = d;
			p2 = strip[p - i];
		}
		i++;
	}
	i = 1;
	while (i <= 11 && (p + i) <= lenghtPointsStrip - 1) {
		float d = sqrt(
				pow(strip[p + i].x - p1.x, 2) + pow(strip[p + i].y - p1.y, 2));
		if (minD == 0 || minD > 0 && d < minD) {
			minD = d;
			p2 = strip[p + i];
		}
		i++;
	}
	return Minimo { p1, p2, minD };
}

} /* namespace DistanceMin */
