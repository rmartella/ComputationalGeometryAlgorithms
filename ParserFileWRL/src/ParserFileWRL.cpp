/*
 * ParserFileWRL.cpp
 *
 *  Created on: 06/09/2015
 *      Author: rey
 */

#include "ParserFileWRL.h"

namespace justin {

ParserFileWRL::ParserFileWRL(const char * file) {
	this->fileOpen = new std::ifstream();
	this->fileOpen->open(file);
}

ParserFileWRL::~ParserFileWRL() {
	// TODO Auto-generated destructor stub
}

std::vector<Polygon> * ParserFileWRL::parserFile() {

	std::vector<Polygon> * out = new std::vector<Polygon>();

	if (this->fileOpen->is_open()) {

		std::string line;

		while (getline(*fileOpen, line)) {

			std::vector<Vertex> * vertex = new std::vector<Vertex>();

			if (line.length() > 0 && line.at(0) != ';') {
				if (line.at(0) == '(' && line.at(line.length() - 1) == ')') {
					TypePolygon * typePolygon = new TypePolygon();
					std::string shape = line.substr(1, line.length() - 2);
					std::istringstream splitM(shape);
					std::string nameLine;
					std::getline(splitM, nameLine, ' ');
					std::getline(splitM, nameLine, ' ');
					if (nameLine == "dimensions") {

					} else if (nameLine == "polygon") {
						bool flag = true;
						std::string type, name;
						do {
							std::getline(splitM, type, ' ');
						} while (type == "");
						if (type == "wall")
							*typePolygon = WALL;
						else if (type == "obstacle")
							*typePolygon = OBSTACLE;
						do {
							std::getline(splitM, name, ' ');
						} while (name == "");
						/*std::getline(splitM, type, ' ');
						 std::getline(splitM, name, ' ');*/
						/*std::cout << "type:" << type << std::endl;
						 std::cout << "name:" << name << std::endl;*/
						do {
							std::string xstr, ystr;
							std::getline(splitM, xstr, ' ');
							std::getline(splitM, ystr, ' ');
							if (xstr == "" || ystr == "")
								flag = false;
							else {
								Vertex vert = { atof(xstr.c_str()), atof(
										ystr.c_str()) };
								vertex->push_back(vert);
							}
						} while (flag);
						out->push_back(Polygon { vertex, typePolygon });
					}
				}
			}

		}
		//std::cout << "size out :" << out->size() << std::endl;
	}
	return out;
}

} /* namespace justin */
