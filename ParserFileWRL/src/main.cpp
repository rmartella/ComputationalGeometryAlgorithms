/*
 * main.cpp
 *
 *  Created on: 06/09/2015
 *      Author: rey
 */

#include "ParserFileWRL.h"
#include "TriangulateVertex.h"

int main(int argc, char **argv) {
	justin::ParserFileWRL parser("filesWRL/random_7.wrl");
	//justin::ParserFileWRL parser("filesWRL/roomPlanning.wrl");
	std::vector<justin::Polygon> * polygons = parser.parserFile();
	/*for (unsigned int i = 0; i < polygons->size(); i++) {
		for (unsigned int j = 0; j < (*polygons)[i].vertex->size(); j++) {
			std::cout << " " << (*(*polygons)[i].vertex)[j].x << " "
					<< (*(*polygons)[i].vertex)[j].y;
		}
		std::cout << std::endl;
	}*/

	justin::TriangulateVertex trv(polygons);
	std::vector<justin::Triangle> triangulation = trv.Traingulate();
	std::cout << "size triangulation: " << triangulation.size() << std::endl;
	for (unsigned int i = 0; i < triangulation.size(); i++) {
		std::cout << "--Triangulacion---" << std::endl;
		std::cout << "V1 (" << triangulation[i].v1.x << ","
				<< triangulation[i].v1.y << "," << triangulation[i].v1.z << ")"
				<< std::endl;
		std::cout << "V2 (" << triangulation[i].v2.x << ","
				<< triangulation[i].v2.y << "," << triangulation[i].v2.z << ")"
				<< std::endl;
		std::cout << "V3 (" << triangulation[i].v3.x << ","
				<< triangulation[i].v3.y << "," << triangulation[i].v3.z << ")"
				<< std::endl;
	}

	exit(EXIT_SUCCESS);
}

