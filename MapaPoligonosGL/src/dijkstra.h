#include "first_depth.h"

#define IN 9999

Nodo dijsktra(Vertex inicio, Vertex fin, int num_polygons, Polygon * polygons,
		Polygon * grownPolygons, bool ** adyacencias, int * sizeListaExpacion,
		float ratio);
float ** createCost(Vertex inicio, Vertex fin, int num_polygons,
		Polygon * polygons, Polygon * grownPolygons, bool ** adyacencias) {
	float ** costos = (float **) malloc(
			(num_polygons + 2) * NUM_MAX_VERTEX * sizeof(float *));

	for (int i = 0; i < (num_polygons + 2) * NUM_MAX_VERTEX; i++) {
		(costos[i] = (float *) malloc(
				(num_polygons + 2) * NUM_MAX_VERTEX * sizeof(float)));
	}
	for (int i = 0; i < (num_polygons + 2) * NUM_MAX_VERTEX; i++) {
		for (int j = 0; j < (num_polygons + 2) * NUM_MAX_VERTEX; j++)
			costos[i][j] = IN;
	}
	for (int i = 0; i <= num_polygons; i++) {
		for (int j = 1; j <= NUM_MAX_VERTEX; j++) {
			for (int k = 0; k <= num_polygons; k++) {
				for (int l = 1; l <= NUM_MAX_VERTEX; l++) {
					if (adyacencias[(i - 0) * NUM_MAX_VERTEX + j - 1][(k - 0)
							* NUM_MAX_VERTEX + l - 1]) {
						Vertex verice1;
						Vertex verice2;
						if (i == 0)
							verice1 = inicio;
						else if (i > 0 && i < num_polygons)
							verice1 = polygons[i].vertex[j];
						else
							verice1 = fin;
						if (k == 0)
							verice2 = inicio;
						else if (k > 0 && k < num_polygons)
							verice2 = polygons[k].vertex[l];
						else
							verice2 = fin;
						costos[(i - 0) * NUM_MAX_VERTEX + j - 1][(k - 0)
								* NUM_MAX_VERTEX + l - 1] =
								getDistanceEuclidean(verice1, verice2);
					}
				}
			}
		}
	}
	return costos;
}
/*int dijsktra(int cost[][N], int num_polygons,int source, int target) {*/
Nodo dijsktra(Vertex inicio, Vertex fin, int num_polygons, Polygon * polygons,
		Polygon * grownPolygons, bool ** adyacencias, int * sizeListaExpacion,
		float ratio) {
	addNodesInitEndToMap(inicio, fin, num_polygons, polygons, grownPolygons,
			adyacencias, ratio);
	float ** cost = createCost(inicio, fin, num_polygons, polygons,
			grownPolygons, adyacencias);
	int prev[(num_polygons + 2) * NUM_MAX_VERTEX], selected[(num_polygons + 2)
			* NUM_MAX_VERTEX], i, m, start, j;
	float dist[(num_polygons + 2) * NUM_MAX_VERTEX], min, d;
	for (i = 0; i < (num_polygons + 2) * NUM_MAX_VERTEX; i++) {
		dist[i] = IN;
		prev[i] = -1;
		selected[i] = 0;
	}
	start = 0;
	selected[start] = 1;
	dist[start] = 0;
	while (selected[(num_polygons - 0) * NUM_MAX_VERTEX] == 0) {
		min = IN;
		m = 0;
		for (i = 0; i < (num_polygons + 2) * NUM_MAX_VERTEX; i++) {
			d = dist[start] + cost[start][i];
			if (d < dist[i] && selected[i] == 0) {
				dist[i] = d;
				prev[i] = start;
			}
			if (min > dist[i] && selected[i] == 0) {
				min = dist[i];
				m = i;
			}
		}
		start = m;
		selected[start] = 1;
	}
	start = (num_polygons - 0) * NUM_MAX_VERTEX;
	int * anterioresPoligonoTmp = (int *) malloc(sizeof(int));
	int * anterioresVerticeTmp = (int *) malloc(sizeof(int));
	j = 0;
	while (start != -1) {
		anterioresPoligonoTmp = (int *) realloc(anterioresPoligonoTmp,
				sizeof(int) * (j + 1));
		anterioresVerticeTmp = (int *) realloc(anterioresVerticeTmp,
				sizeof(int) * (j + 1));
		if (start > 0) {
			int poligono = start / NUM_MAX_VERTEX + 0;
			int vertice = start % NUM_MAX_VERTEX + 1;

			anterioresPoligonoTmp[j] = poligono;
			anterioresVerticeTmp[j] = vertice;
		} else {
			anterioresPoligonoTmp[j] = 0;
			anterioresVerticeTmp[j] = 0;
		}
		j++;
		start = prev[start];
	}
	int * anterioresPoligono = (int *) malloc(sizeof(int) * j);
	int * anterioresVertice = (int *) malloc(sizeof(int) * j);
	for (int i = 0; i < j; i++) {
		anterioresPoligono[i] = anterioresPoligonoTmp[j - i - 1];
		anterioresVertice[i] = anterioresVerticeTmp[j - i - 1];
	}
	free(anterioresPoligonoTmp);
	free(anterioresVerticeTmp);
	free(cost);
	Nodo nodePath = { num_polygons, 0, anterioresPoligono, anterioresVertice, j
			- 1 };
	return nodePath;

}
