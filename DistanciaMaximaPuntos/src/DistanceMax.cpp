/*
 * DistanciaMinima.cpp
 *
 *  Created on: 01/04/2015
 *      Author: rey
 */

#include "DistanceMax.h"

namespace DistanceMax {

int getIndexMaxPoint(std::vector<ConvexHull::Punto> listPoints);
int getIndexMinPoint(std::vector<ConvexHull::Punto> listPoints);

DistanceMax::DistanceMax() {
	// TODO Auto-generated constructor stub

}

DistanceMax::DistanceMax(std::vector<ConvexHull::Punto> listPoints) {
	this->listPoints = listPoints;

}

DistanceMax::~DistanceMax() {
	// TODO Auto-generated destructor stub
}

/* Método que obtiene la pareja de puntos mas distantes, el algortimo toma complejidad lineal,
 * Una vez ya calculado el cierre convexo, se para el algortimo cuando la linea de barrido rotacional
 * da un giro de 180°
 */
std::vector<ConvexHull::Punto> DistanceMax::getPointsMaxDistance() {
	std::vector<ConvexHull::Punto> maxDistancePoints;
	/* Se obtiene el indice en el cúal va a empezar a barrer la primera linea*/
	unsigned int index1 = getIndexMaxPoint(listPoints);
	unsigned int index1Sig = index1;
	/* Como el algortimo de cierre convezo mantiene en la base de la pila el punto de menor valor en el eje y,
	 * Se toma el ulitmo elemento del arreglo*/
	unsigned int index2 = listPoints.size() - 1;
	unsigned int index2Sig = index2;
	float angle = 0;
	float angleTmp = angle;
	float theta1;
	float theta2;
	int indicesMin1 = index1;
	int indicesMin2 = index2;

	/*Comienza la iteración del algortimo rotacional*/
	do {

		/* Si los indices ya llego al primer elemento, el siguiente indice es el final del arreglo,
		 * Si el segundo indice al ir decrementando llega al primer elemento el siguiente indice es el final del arreglo
		 */
		if (index1 == 0)
			index1Sig = listPoints.size() - 1;
		else if (index1 == index1Sig)
			index1Sig--;
		if (index2 == 0)
			index2Sig = listPoints.size() - 1;
		else if (index2 == index2Sig)
			index2Sig--;

		/*Si se encontraron los puntos minimos en ese momento de la linea de barrido se guardan los indices*/
		if (sqrt(
				pow(listPoints[index1].x - listPoints[index2].x, 2)
						+ pow(listPoints[index1].y - listPoints[index2].y, 2))
				> sqrt(
						pow(
								listPoints[indicesMin1].x
										- listPoints[indicesMin2].x, 2)
								+ pow(
										listPoints[indicesMin1].y
												- listPoints[indicesMin2].y,
										2))) {
			indicesMin1 = index1;
			indicesMin2 = index2;
		}

		/*Para la primera linea superior se calcula el angulo que forma el primer punto y el segundo donde se puede apoyar la recta, con el eje x*/
		theta1 = acos(
				(listPoints[index1].x - listPoints[index1Sig].x)
						/ (sqrt(
								pow(
										listPoints[index1Sig].x
												- listPoints[index1].x, 2)
										+ pow(
												listPoints[index1Sig].y
														- listPoints[index1].y,
												2))));
		/*Para la segunda linea inferior se calcula el angulo que forma el primer punto y el segundo donde se puede apoyar la recta, con el eje x*/
		theta2 =
				acos(
						(listPoints[index2Sig].x - listPoints[index2].x)
								/ (sqrt(
										pow(
												listPoints[index2].x
														- listPoints[index2Sig].x,
												2)
												+ pow(
														listPoints[index2].y
																- listPoints[index2Sig].y,
														2))));

		angleTmp = angle;

		/* Se compara de que lado se apoyará primero parte superior o inferior, se toma en cuenta el angulo acumulado*/
		if (theta1 - angle < theta2 - angle) {
			angle = theta1;
			index1 = index1Sig;
		} else if (theta1 - angle > theta2 - angle) {
			angle = theta2;
			index2 = index2Sig;
		}
		/* Cuando el agoritmo va calculando los puntos se incrementa el angulo acumulado,
		 * cuando existe un cambio de incrementar a decrementar significa que las lineas ya dieron un giro de 180°
		 * Por lo que termina el algortimo
		 */
	} while (angle >= angleTmp);
	maxDistancePoints.push_back(listPoints[indicesMin1]);
	maxDistancePoints.push_back(listPoints[indicesMin2]);
	return maxDistancePoints;
}

/*Este método sirve para encontrar el punto maximo con respecto al eje y,
 *En caso que haya dos se toma el punto con menor x
 */
int getIndexMaxPoint(std::vector<ConvexHull::Punto> listPoints) {
	int index = 0;
	for (unsigned int i = 1; i < listPoints.size(); i++) {
		if (listPoints[i].y >= listPoints[index].y) {
			if (listPoints[i].y == listPoints[index].y) {
				if (listPoints[i].x > listPoints[index].x) {
					index = i;
				}
			} else {
				index = i;
			}
		}
	}
	return index;
}

int getIndexMinPoint(std::vector<ConvexHull::Punto> listPoints) {
	int index = 0;
	for (unsigned int i = 1; i < listPoints.size(); i++) {
		if (listPoints[i].y <= listPoints[index].y) {
			if (listPoints[i].y == listPoints[index].y) {
				if (listPoints[i].x < listPoints[index].x) {
					index = i;
				}
			} else {
				index = i;
			}
		}
	}
	return index;
}

} /* namespace UtilInt */
