/*
 * Utili.cpp
 *
 *  Created on: 09/04/2015
 *      Author: rey
 */

#include "DistanceMin.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

/*Esta función toma como parametro el nombre del archvio donde se almacenearan los puntos.
 *El formato de dicho archivo debe de seguir como el siguiente ejemplo: [(19, -25), (30, 93)].
 *La función regresa un vector de puntos en el formato esperado.
 */
std::vector<DistanceMin::Punto> parserPointsFormFile(const char * ptr_file) {
	string stringLine;
	ifstream infile;
	infile.open(ptr_file);
	string previousLine = "";
	std::vector<DistanceMin::Punto> points;
	while (!infile.eof()) // To get you all the lines.
	{
		getline(infile, stringLine); // Saves the line in STRING.
		try {
			//cout << "stringLine:" << stringLine << endl;
			std::replace(stringLine.begin(), stringLine.end(), '[', ' ');
			std::replace(stringLine.begin(), stringLine.end(), ']', ' ');
			//stringLine = std::replace(stringLine.begin(),stringLine.end(), '[');
			std::vector<std::string> pointsIterator = split(stringLine, '(');
			for (unsigned int i = 1; i < pointsIterator.size(); i++) {
				std::string point = pointsIterator[i];
				std::replace(point.begin(), point.end(), ')', ' ');
				point.erase(std::remove(point.begin(), point.end(), ' '),
						point.end());
				std::vector<std::string> componente = split(point, ',');
				float x = atof(componente[0].c_str());
				float y = atof(componente[1].c_str());
				points.push_back(DistanceMin::Punto { x, y });
			}
			//printf("P (%f,%f)\n", x, y);
		} catch (exception &e) {
			cout << "Exception e:" << e.what() << endl;
			throw e;
		}

	}
	infile.close();
	return points;
}

/*Función auxiliar para separar cadenas, toma como argumento la cadena, la delimitacion, regresa la nueva cadena*/
std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

/*Wrapper de la función split, toma como parametro la cadena y el caracter para separar*/
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

// Función para dividir el array y hacer los intercambios
int divide(DistanceMin::Punto *array, int start, int end, int tipoOrdenacion) {
	int left;
	int right;
	DistanceMin::Punto pivot;

	DistanceMin::Punto temp;
	pivot = *(array + start);
	left = start;
	right = end;

	// Mientras no se cruzen los índices
	while (left < right) {
		if (tipoOrdenacion == 1) {
			while ((array + right)->y > pivot.y) {
				right--;
			}
		} else {
			while ((array + right)->x > pivot.x
					|| ((array + right)->x == pivot.x
							&& (array + right)->y > pivot.y)) {
				//while ((array + right)->x > pivot.x) {
				right--;
			}
		}

		if (tipoOrdenacion == 1) {
			while ((left < right) && ((array + left)->y <= pivot.y)) {
				left++;
			}
		} else {
			while ((left < right)
					&& ((array + left)->x < pivot.x
							|| ((array + left)->x == pivot.x
									&& (array + left)->y <= pivot.y))) {
				left++;
			}
		}

		// Si todavía no se cruzan los indices seguimos intercambiando
		if (left < right) {
			temp.x = (array + left)->x;
			temp.y = (array + left)->y;
			(array + left)->x = (array + right)->x;
			(array + left)->y = (array + right)->y;
			(array + right)->x = temp.x;
			(array + right)->y = temp.y;
		}
	}

	// Los índices ya se han cruzado, ponemos el pivot en el lugar que le corresponde
	temp.x = (array + right)->x;
	temp.y = (array + right)->y;
	(array + right)->x = (array + start)->x;
	(array + right)->y = (array + start)->y;
	(array + start)->x = temp.x;
	(array + start)->y = temp.y;

	// La nueva posición del pivote
	return right;
}

// Función recursiva para hacer el ordenamiento
void quicksort(DistanceMin::Punto * array, int start, int end,
		int tipoOrdenacion) {
	float pivot;

	if (start < end) {
		pivot = divide(array, start, end, tipoOrdenacion);

		// Ordeno la lista de los menores
		quicksort(array, start, pivot - 1, tipoOrdenacion);

		// Ordeno la lista de los mayores
		quicksort(array, pivot + 1, end, tipoOrdenacion);
	}
}
