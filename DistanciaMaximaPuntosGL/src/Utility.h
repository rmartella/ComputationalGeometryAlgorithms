/*
 * Utili.cpp
 *
 *  Created on: 09/04/2015
 *      Author: rey
 */

#include "ConvexHull.h"
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
std::vector<ConvexHull::Punto> parserPointsFormFile(const char * ptr_file) {
	string stringLine;
	ifstream infile;
	infile.open(ptr_file);
	string previousLine = "";
	std::vector<ConvexHull::Punto> points;
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
				points.push_back(ConvexHull::Punto { x, y });
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
