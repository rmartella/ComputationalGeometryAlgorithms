#include "camposPotenciales.h"

#define N 1

int main() {
	int i = 0;
	int j = 0;
	Polygon poligonos[N];
	Polygon poligono;
	Vertex vertice;
	poligono.num_vertex = 0;
	vertice = {0, 0};
	poligono.vertex[j++] = vertice;
	poligonos[i++] = poligono;
	/*poligono.num_vertex = 5;
	 vertice = {1, 1};
	 poligono.vertex[j++] = vertice;
	 vertice = {5, 3};
	 poligono.vertex[j++] = vertice;
	 vertice = {5, -6};
	 poligono.vertex[j++] = vertice;
	 vertice = {0, -6};
	 poligono.vertex[j++] = vertice;
	 vertice = {3, -2};
	 poligono.vertex[j] = vertice;
	 poligonos[i++] = poligono;*/
	/*j = 0;
	 poligono.num_vertex = 4;
	 vertice = {0, 0};
	 poligono.vertex[j++] = vertice;
	 vertice = {3, 1};
	 poligono.vertex[j++] = vertice;
	 vertice = {3, 3};
	 poligono.vertex[j++] = vertice;
	 vertice = {1, 3};
	 poligono.vertex[j++] = vertice;
	 vertice = {1, 1};
	 poligono.vertex[j] = vertice;
	 poligonos[i] = poligono;*/

	i = 0;
	//Polygon * nuevosPoligonos = getPolygonInClockwiseOrder(N, poligonos);
	/*coord * centroide = getCentroid(N, poligonos);
	 for (i = 0; i < N; i++) {
	 printf("centroide1 P(%f,%f)\n", centroide[i].xc, centroide[i].yc);
	 }
	 coord robot;
	 robot.xc = 1;
	 robot.yc = 1;
	 coord * fuerzas = getFuerzasRepulsion(robot, poligonos, N);
	 printf("fuerza P(%f,%f)\n", fuerzas->xc, fuerzas->yc);
	 free(fuerzas);*/
	float angRobot = 4.324600;
	coord robot = { 0.722500, 0.727500, angRobot };
	coord destino = { 0.400000, 0.585000, 0 };
	coord * cordenada = getFuerzaTotal(robot, poligonos, N, destino);
	float moduloCoor = getModule(*cordenada);
	coord * siguiente = (coord *) malloc(sizeof(coord));
	siguiente->xc = robot.xc - DELTA * cordenada->xc / moduloCoor;
	siguiente->yc = robot.yc - DELTA * cordenada->yc / moduloCoor;
	float giro = obtenerGiro(robot, *siguiente);

	printf("Siguiente punto P(%f,%f)\n", siguiente->xc, siguiente->yc);
	printf("giro %f\n", giro);
}
