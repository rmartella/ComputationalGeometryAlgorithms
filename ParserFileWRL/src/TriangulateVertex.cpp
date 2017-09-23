/*
 * TriangulateVertex.cpp
 *
 *  Created on: 06/09/2015
 *      Author: rey
 */

#include "TriangulateVertex.h"
#include "UtilGeometry.h"

namespace justin {

TriangulateVertex::TriangulateVertex(std::vector<justin::Polygon> * polygons) {
	this->polygons = polygons;
}

TriangulateVertex::~TriangulateVertex() {
	// TODO Auto-generated destructor stub
}

std::vector<justin::Triangle> TriangulateVertex::Traingulate() {
	std::vector<justin::Triangle> out;
	justin::UtilGeometry ug = justin::UtilGeometry();
	float hightObs = 0.30000000;
	float hightWalls = 0.30000000;
	float hight = 0.00000000;
	int ind1;
	int ind2;
	int ind3;
	int ind4;
	int indexInit = 0;
	for (unsigned int i = 0; i < polygons->size(); i++) {
		if ((*polygons)[i].vertex->size() >= 3) {
			if (*(*polygons)[i].type != WALL) {
				for (unsigned int j = 0; j < (*polygons)[i].vertex->size();
						j++) {
					float det = ug.getDeterminant(
							&(*(*polygons)[i].vertex)[indexInit],
							&(*(*polygons)[i].vertex)[indexInit + 1],
							&(*(*polygons)[i].vertex)[indexInit + 2]);
					if (det >= 0) {
						ind3 = indexInit + 1;
						ind4 = indexInit + 2;
						break;
					} else
						indexInit++;
				}
				hight = hightObs;
			} else if (*(*polygons)[i].type == WALL) {
				indexInit = 0;
				ind3 = indexInit + 1;
				ind4 = indexInit + 2;
				hight = hightWalls;
			}

			for (unsigned int j = 0; j < (*polygons)[i].vertex->size(); j++) {

				if (j + 1 < (*polygons)[i].vertex->size()) {
					ind1 = j;
					ind2 = j + 1;
				} else {
					ind1 = j;
					ind2 = 0;
				}

				if (j + 2 < (*polygons)[i].vertex->size()) {
					if (ind3 >= (*polygons)[i].vertex->size())
						ind3 = 0;
					if (ind4 >= (*polygons)[i].vertex->size())
						ind4 = 0;

					out.push_back(justin::Triangle { justin::Vertex {
							(*(*polygons)[i].vertex)[indexInit].x,
							(*(*polygons)[i].vertex)[indexInit].y, 0.0 },
							justin::Vertex { (*(*polygons)[i].vertex)[ind3].x,
									(*(*polygons)[i].vertex)[ind3].y, 0.0 },
							justin::Vertex { (*(*polygons)[i].vertex)[ind4].x,
									(*(*polygons)[i].vertex)[ind4].y, 0.0 } });

					out.push_back(
							justin::Triangle {
									justin::Vertex {
											(*(*polygons)[i].vertex)[indexInit].x,
											(*(*polygons)[i].vertex)[indexInit].y,
											hight }, justin::Vertex {
											(*(*polygons)[i].vertex)[ind3].x,
											(*(*polygons)[i].vertex)[ind3++].y,
											hight }, justin::Vertex {
											(*(*polygons)[i].vertex)[ind4].x,
											(*(*polygons)[i].vertex)[ind4++].y,
											hight } });
				}

				out.push_back(
						justin::Triangle { justin::Vertex {
								(*(*polygons)[i].vertex)[ind1].x,
								(*(*polygons)[i].vertex)[ind1].y, 0.0 },
								justin::Vertex {
										(*(*polygons)[i].vertex)[ind2].x,
										(*(*polygons)[i].vertex)[ind2].y, 0.0 },
								justin::Vertex {
										(*(*polygons)[i].vertex)[ind1].x,
										(*(*polygons)[i].vertex)[ind1].y, hight } });
				out.push_back(
						justin::Triangle { justin::Vertex {
								(*(*polygons)[i].vertex)[ind2].x,
								(*(*polygons)[i].vertex)[ind2].y, 0.0 },
								justin::Vertex {
										(*(*polygons)[i].vertex)[ind2].x,
										(*(*polygons)[i].vertex)[ind2].y, hight },
								justin::Vertex {
										(*(*polygons)[i].vertex)[ind1].x,
										(*(*polygons)[i].vertex)[ind1].y, hight } });
			}
		}
	}
	return out;
}

} /* namespace justin */
