#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define WINDOW_TITLE_PREFIX "Ejemplo 2"

/*
 #include "create_map.h"
 #include "first_depth.h"
 */
#include "dijkstra.h"

#define BUFFER_OFFSET(offset) ((void*)(offset))

int CurrentWidth = 400, CurrentHeight = 300, WindowHandle = 0;
unsigned FrameCount = 0;

/*
 * 1.- Variables globales que contiene los identificadores para los shaders y buffer objects
 */
GLuint VertexShaderId, FragmentShaderId, ProgramId, VaoId[6], VboId[6],
		g_uniformColor, ActiveIndexBuffer = 1;

GLuint sizePolygon;
GLuint sizeModelMap;
GLuint sizeModelExtremos;
GLuint sizeModelRutaDFS;
GLuint sizeModelRutaDJKS;

/*
 * 2.- GLSL vertex
 */
/*
 * ex_Color es la salida de nuestro Shader, sirve como intercambio entre varios Shaders.
 */
const GLchar* VertexShader = { "#version 450\n"

		"layout(location=0) in vec3 in_Position;\n"
		"uniform vec3 in_Color;\n"
		"out vec4 ex_Color;\n"

		"void main(void)\n"
		"{\n"
		"  gl_Position = vec4(in_Position,1);\n"
		"  ex_Color = vec4(in_Color,1);\n"
		"}\n" };
/*
 *
 */
const GLchar* FragmentShader = { "#version 450\n"

		"in vec4 ex_Color;\n"
		"out vec4 out_Color;\n"

		"void main(void)\n"
		"{\n"
		"  out_Color = ex_Color;\n"
		"}\n" };

const glm::vec3 red(1.0f, 0.0f, 0.0f);
const glm::vec3 blue(0.0f, 0.0f, 1.0f);
const glm::vec3 white(1.0f, 1.0f, 1.0f);
const glm::vec3 black(0.0f, 0.0f, 0.0f);

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);

/*
 * 3.- Se agregan las declaraciones de nuevas funciones.
 */
void Cleanup(void);
void CreateVBO(std::vector<glm::vec3> * modelPolygon,
		std::vector<glm::vec3> * modelGrowPolygon,
		std::vector<glm::vec3> * modelMap,
		std::vector<glm::vec3> * modelMapExtremos,
		std::vector<glm::vec3> * modelRutaDFS,
		std::vector<glm::vec3> * modelRutaDJKS);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);
void KeyboardFunction(unsigned char, int, int);

int main(int argc, char* argv[]) {
	/* Esta función es llamada para evitar que truene el programa*/
	pthread_getconcurrency();
	Initialize(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[]) {
	GLenum GlewInitResult;
	InitWindow(argc, argv);
	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		fprintf(
		stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}
	fprintf(
	stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

	/*
	 * 4.- Se agregan funciones para crear Shaders y VBO.
	 */
	CreateShaders();

	std::vector<glm::vec3> modelPolygon;
	std::vector<glm::vec3> modelGrowPolygon;
	std::vector<glm::vec3> modelMap;
	std::vector<glm::vec3> modelMapInicio;
	std::vector<glm::vec3> modelRutaDFS;
	std::vector<glm::vec3> modelRutaDJKS;

	/*Modificación codigo para emparejar vertices*/

	Polygon * poligonos = (Polygon *) malloc(sizeof(Polygon) * 23);

	int offset = 4;

	poligonos[5 - offset].num_vertex = 4;
	poligonos[5- offset].vertex[1] = {0.000000 ,0.000000};
	poligonos[5- offset].vertex[2] = {0.000000 ,0.995000};
	poligonos[5- offset].vertex[3] = {0.005000 ,0.995000};
	poligonos[5- offset].vertex[4] = {0.005000 ,0.000000};

	poligonos[6 - offset].num_vertex = 4;
	poligonos[6- offset].vertex[1] = {0.000000 ,0.995000};
	poligonos[6- offset].vertex[2] = {0.000000 ,1.000000};
	poligonos[6- offset].vertex[3] = {1.000000 ,1.000000};
	poligonos[6- offset].vertex[4] = {1.000000 ,0.995000};

	poligonos[7 - offset].num_vertex = 4;
	poligonos[7- offset].vertex[1] = {0.995000 ,0.000000};
	poligonos[7- offset].vertex[2] = {0.995000 ,1.000000};
	poligonos[7- offset].vertex[3] = {1.000000 ,1.000000};
	poligonos[7- offset].vertex[4] = {1.000000 ,0.000000};

	poligonos[8 - offset].num_vertex = 4;
	poligonos[8- offset].vertex[1] = {0.000000 ,0.000000};
	poligonos[8- offset].vertex[2] = {0.000000 ,0.005000};
	poligonos[8- offset].vertex[3] = {1.000000 ,0.005000};
	poligonos[8- offset].vertex[4] = {1.000000 ,0.000000};

	poligonos[9 - offset].num_vertex = 4;
	poligonos[9- offset].vertex[1] = {0.005000 ,0.995000};
	poligonos[9- offset].vertex[2] = {0.655000 ,0.995000};
	poligonos[9- offset].vertex[3] = {0.655000 ,0.955000};
	poligonos[9- offset].vertex[4] = {0.005000 ,0.955000};

	poligonos[10 - offset].num_vertex = 4;
	poligonos[10- offset].vertex[1] = {0.655000 ,0.995000};
	poligonos[10- offset].vertex[2] = {0.695000 ,0.995000};
	poligonos[10- offset].vertex[3] = {0.695000 ,0.495000};
	poligonos[10- offset].vertex[4] = {0.655000 ,0.495000};

	poligonos[11 - offset].num_vertex = 4;
	poligonos[11- offset].vertex[1] = {0.695000 ,0.495000};
	poligonos[11- offset].vertex[2] = {0.695000 ,0.535000};
	poligonos[11- offset].vertex[3] = {0.815000 ,0.535000};
	poligonos[11- offset].vertex[4] = {0.815000 ,0.495000};

	poligonos[12 - offset].num_vertex = 4;
	poligonos[12- offset].vertex[1] = {0.005000 ,0.495000};
	poligonos[12- offset].vertex[2] = {0.005000 ,0.535000};
	poligonos[12- offset].vertex[3] = {0.475000 ,0.535000};
	poligonos[12- offset].vertex[4] = {0.475000 ,0.495000};

	poligonos[13 - offset].num_vertex = 4;
	poligonos[13- offset].vertex[1] = {0.005000 ,0.535000};
	poligonos[13- offset].vertex[2] = {0.005000 ,0.575000};
	poligonos[13- offset].vertex[3] = {0.385000 ,0.575000};
	poligonos[13- offset].vertex[4] = {0.385000 ,0.535000};

	poligonos[14 - offset].num_vertex = 4;
	poligonos[14- offset].vertex[1] = {0.615000 ,0.745000};
	poligonos[14- offset].vertex[2] = {0.655000 ,0.745000};
	poligonos[14- offset].vertex[3] = {0.655000 ,0.575000};
	poligonos[14- offset].vertex[4] = {0.615000 ,0.575000};

	poligonos[15 - offset].num_vertex = 4;
	poligonos[15- offset].vertex[1] = {0.005000 ,0.955000};
	poligonos[15- offset].vertex[2] = {0.045000 ,0.955000};
	poligonos[15- offset].vertex[3] = {0.045000 ,0.745000};
	poligonos[15- offset].vertex[4] = {0.005000 ,0.745000};

	poligonos[16 - offset].num_vertex = 4;
	poligonos[16- offset].vertex[1] = {0.005000 ,0.745000};
	poligonos[16- offset].vertex[2] = {0.205000 ,0.745000};
	poligonos[16- offset].vertex[3] = {0.205000 ,0.705000};
	poligonos[16- offset].vertex[4] = {0.005000 ,0.705000};

	poligonos[17 - offset].num_vertex = 4;
	poligonos[17- offset].vertex[1] = {0.385000 ,0.745000};
	poligonos[17- offset].vertex[2] = {0.615000 ,0.745000};
	poligonos[17- offset].vertex[3] = {0.615000 ,0.705000};
	poligonos[17- offset].vertex[4] = {0.385000 ,0.705000};

	poligonos[18 - offset].num_vertex = 4;
	poligonos[18- offset].vertex[1] = {0.005000 ,0.575000};
	poligonos[18- offset].vertex[2] = {0.005000 ,0.705000};
	poligonos[18- offset].vertex[3] = {0.045000 ,0.705000};
	poligonos[18- offset].vertex[4] = {0.045000 ,0.575000};

	poligonos[19 - offset].num_vertex = 4;
	poligonos[19- offset].vertex[1] = {0.005000 ,0.005000};
	poligonos[19- offset].vertex[2] = {0.005000 ,0.045000};
	poligonos[19- offset].vertex[3] = {0.307500 ,0.045000};
	poligonos[19- offset].vertex[4] = {0.307500 ,0.005000};

	poligonos[20 - offset].num_vertex = 4;
	poligonos[20- offset].vertex[1] = {0.307500 ,0.005000};
	poligonos[20- offset].vertex[2] = {0.307500 ,0.045000};
	poligonos[20- offset].vertex[3] = {0.615000 ,0.045000};
	poligonos[20- offset].vertex[4] = {0.615000 ,0.005000};

	poligonos[21 - offset].num_vertex = 4;
	poligonos[21- offset].vertex[1] = {0.005000 ,0.045000};
	poligonos[21- offset].vertex[2] = {0.005000 ,0.2775};
	poligonos[21- offset].vertex[3] = {0.045 ,0.2775};
	poligonos[21- offset].vertex[4] = {0.045 ,0.045000};

	poligonos[22 - offset].num_vertex = 4;
	poligonos[22- offset].vertex[1] = {0.005000 ,0.2775};
	poligonos[22- offset].vertex[2] = {0.005000 ,0.3175};
	poligonos[22- offset].vertex[3] = {0.307500 ,0.3175};
	poligonos[22- offset].vertex[4] = {0.307500 ,0.2775};

	poligonos[23 - offset].num_vertex = 4;
	poligonos[23- offset].vertex[1] = {0.2675 ,0.045000};
	poligonos[23- offset].vertex[2] = {0.2675 ,0.2775};
	poligonos[23- offset].vertex[3] = {0.307500 ,0.2775};
	poligonos[23- offset].vertex[4] = {0.307500 ,0.045000};

	poligonos[24 - offset].num_vertex = 4;
	poligonos[24- offset].vertex[1] = {0.575 ,0.045000};
	poligonos[24- offset].vertex[2] = {0.575 ,0.2775};
	poligonos[24- offset].vertex[3] = {0.615000 ,0.2775};
	poligonos[24- offset].vertex[4] = {0.615000 ,0.045000};

	poligonos[25 - offset].num_vertex = 4;
	poligonos[25- offset].vertex[1] = {0.3075 ,0.2775};
	poligonos[25- offset].vertex[2] = {0.3075 ,0.3175};
	poligonos[25- offset].vertex[3] = {0.435 ,0.3175};
	poligonos[25- offset].vertex[4] = {0.435 ,0.2775};

	poligonos[26 - offset].num_vertex = 4;
	poligonos[26- offset].vertex[1] = {0.575 ,0.2775};
	poligonos[26- offset].vertex[2] = {0.575 ,0.3175};
	poligonos[26- offset].vertex[3] = {0.815000 ,0.3175};
	poligonos[26- offset].vertex[4] = {0.815000 ,0.2775};

	/*
	 ( polygon wall wall_16  )
	 ( polygon wall wall_16  )
	 ( polygon wall wall_16  )*/

	Polygon * grownPolygons = (Polygon *) malloc(sizeof(Polygon));

	int num_polygons = 23;

	grownPolygons = grownPoligons(num_polygons, 0.03, poligonos, grownPolygons);

	for (int i = 1; i < num_polygons; i++) {
		for (int j = 1; j <= grownPolygons[i].num_vertex; j++) {
			if (j < grownPolygons[i].num_vertex) {
				modelGrowPolygon.push_back(
						glm::vec3(grownPolygons[i].vertex[j].x,
								grownPolygons[i].vertex[j].y, 0.0f));
				modelGrowPolygon.push_back(
						glm::vec3(grownPolygons[i].vertex[j + 1].x,
								grownPolygons[i].vertex[j + 1].y, 0.0f));
				modelPolygon.push_back(
						glm::vec3(poligonos[i].vertex[j].x,
								poligonos[i].vertex[j].y, 0.0f));
				modelPolygon.push_back(
						glm::vec3(poligonos[i].vertex[j + 1].x,
								poligonos[i].vertex[j + 1].y, 0.0f));
			} else {
				modelGrowPolygon.push_back(
						glm::vec3(grownPolygons[i].vertex[j].x,
								grownPolygons[i].vertex[j].y, 0.0f));
				modelGrowPolygon.push_back(
						glm::vec3(grownPolygons[i].vertex[1].x,
								grownPolygons[i].vertex[1].y, 0.0f));
				modelPolygon.push_back(
						glm::vec3(poligonos[i].vertex[j].x,
								poligonos[i].vertex[j].y, 0.0f));
				modelPolygon.push_back(
						glm::vec3(poligonos[i].vertex[1].x,
								poligonos[i].vertex[1].y, 0.0f));
			}
		}
	}

	sizePolygon = modelGrowPolygon.size();

	bool ** adyacencias = getMapTopologic(num_polygons, poligonos,
			grownPolygons, 0.03);

	printf("Indexacion Matriz de adyacencias\n");
	for (int i = 1; i < num_polygons; i++) {
		for (int j = 1; j < num_polygons; j++) {
			for (int k = 1; k <= NUM_MAX_VERTEX; k++) {
				for (int l = 1; l <= NUM_MAX_VERTEX; l++) {
					bool adyacente = adyacencias[(i - 0) * NUM_MAX_VERTEX + k
							- 1][(j - 0) * NUM_MAX_VERTEX + l - 1];
					if (adyacente) {
						//if (adyacente && i == j) {
						modelMap.push_back(
								glm::vec3(grownPolygons[i].vertex[k].x,
										grownPolygons[i].vertex[k].y, 0.0f));
						modelMap.push_back(
								glm::vec3(grownPolygons[j].vertex[l].x,
										grownPolygons[j].vertex[l].y, 0.0f));
						/*printf("%f,%f\t\n%f,%f\t\n",
						 grownPolygons[i].vertex[k].x,
						 grownPolygons[i].vertex[k].y,
						 grownPolygons[j].vertex[l].x,
						 grownPolygons[j].vertex[l].y);*/

					}
					//printf("%d\n", adyacente);
				}
			}
		}
	}
	/*printf("Matriz de adyacencias\n");
	 for (int i = 4; i <= num_polygons; i++) {
	 for (int j = 1; j <= NUM_MAX_VERTEX; j++) {
	 bool * adyacentes = adyacencias[(i - 4) * NUM_MAX_VERTEX + j - 1];
	 for (int k = 4; k <= num_polygons; k++) {
	 for (int l = 1; l <= NUM_MAX_VERTEX; l++) {
	 printf("%d,", adyacentes[(k - 4) * NUM_MAX_VERTEX + l - 1]);
	 }
	 }
	 printf("\n");
	 }
	 }*/

	printf("\nExpacion\n");
	sizeModelMap = modelMap.size();

	/*Vertex inicio = { 0.170000, 0.077500 };
	 Vertex fin = { 0.847500, 0.830000 };*/

	/*Vertex inicio = { 0.587309, 0.899922 };
	 Vertex fin = { 0.240000, 0.925000 };*/

	/*Vertex inicio = { 0.100000, 0.065000 };
	 Vertex fin = { 0.902500, 0.917500 };*/

	/*Vertex inicio = { 0.082500, 0.072500 };
	 Vertex fin = { 0.882500, 0.907500 };*/

	/*Fallo*/
	/*Vertex inicio = { 0.392500, 0.192500 };
	 Vertex fin = { 0.510000, 0.932500 };*/

	/*Vertex inicio = { 0.900000, 0.910000 };
	 Vertex fin = { 0.157500, 0.090000 };*/

	/*Vertex inicio = { 0.925000, 0.087500 };
	 Vertex fin = { 0.182500, 0.897500 };*/

	/*Vertex inicio = { 0.440000, 0.530000 };
	 Vertex fin = { 0.907500, 0.840000 };*/

	/*Funciono pero pasó algo extraño revisar un poco mas*/
	/*Vertex inicio = { 0.867500, 0.500000 };
	 Vertex fin = { 0.195000, 0.782500 };*/

	/*Vertex inicio = { 0.297500, 0.782500 };
	 Vertex fin = { 0.830000, 0.512500 };*/

	/*Vertex inicio = { 0.720000, 0.870000 };
	 Vertex fin = { 0.137500, 0.165000 };*/

	/*Vertex inicio = { 0.412500, 0.087500 };
	 Vertex fin = { 0.472500, 0.942500 };*/

	/*Probar con este el pico*/
	//Vertex inicio = { 0.2025, 0.195 };
	/*Vertex inicio = { 0.335000, 0.877500 };
	 Vertex fin = { 0.535000, 0.087500 };*/

	/*Vertex inicio = { 0.897500, 0.897500 };
	 Vertex fin = { 0.317500, 0.837500 };*/

	/*Vertex inicio = { 0.322500, 0.892500 };
	 Vertex fin = { 0.732500, 0.107500 };*/

	/*Vertex inicio = { 0.112500, 0.377500 };
	 Vertex fin = { 0.387500, 0.487500 };*/

	/*Vertex inicio = { 0.112500, 0.630000 };
	 Vertex fin = { 0.440000, 0.110000 };*/
	/*Vertex inicio = { 0.440000, 0.110000 };
	 Vertex fin = { 0.047500, 0.397500 };*/
	/*Vertex inicio = { 0.047500, 0.397500 };
	Vertex fin = { 0.440000, 0.110000 };*/
	/*Vertex inicio = { 0.440000, 0.110000 };
	 Vertex fin = { 0.047500, 0.397500 };*/
	/*Vertex inicio = { 0.047500, 0.397500 };
	 Vertex fin = { 0.440000, 0.110000 };*/
	Vertex inicio = { 0.440000, 0.110000 };
	 Vertex fin = { 0.090000, 0.915000 };
	/*Vertex inicio = { 0.434010, 0.407693 };
	 Vertex fin = { 0.440000, 0.110000 };*/
	/*Vertex inicio = { 0.440000, 0.110000 };
	 Vertex fin = { 0.047500, 0.397500 };*/
	/*Vertex inicio = { 0.047500, 0.397500 };
	 Vertex fin = { 0.440000, 0.110000 };*/
	/*Vertex inicio = { 0.440000, 0.110000 };
	 Vertex fin = { 0.090000, 0.915000 };*/

	int * sizeListaExpacion = (int *) malloc(sizeof(int));

	Nodo listaExpacionDFS = firstDepth(inicio, fin, num_polygons, poligonos,
			grownPolygons, adyacencias, sizeListaExpacion, 0.03);

	/*Se revisan los poligonos que se enlazan con el primero*/
	bool * expancionInicio = adyacencias[0];
	for (int i = 1; i < num_polygons; i++) {
		for (int j = 1; j <= grownPolygons[i].num_vertex; j++) {
			if (expancionInicio[(i - 0) * NUM_MAX_VERTEX + j - 1]) {
				modelMapInicio.push_back(glm::vec3(inicio.x, inicio.y, 0.0f));
				modelMapInicio.push_back(
						glm::vec3(grownPolygons[i].vertex[j].x,
								grownPolygons[i].vertex[j].y, 0.0f));
				/*printf("%f,%f\n%f,%f\n", inicio.x, inicio.y,
				 grownPolygons[i].vertex[j].x,
				 grownPolygons[i].vertex[j].y);*/
			}
		}
	}
	/*Se revisan los poligonos que se enlazan con el fin*/
	expancionInicio = adyacencias[(num_polygons - 0) * NUM_MAX_VERTEX];
	for (int i = 1; i < num_polygons; i++) {
		for (int j = 1; j <= grownPolygons[i].num_vertex; j++) {
			if (expancionInicio[(i - 0) * NUM_MAX_VERTEX + j - 1]) {
				modelMapInicio.push_back(glm::vec3(fin.x, fin.y, 0.0f));
				modelMapInicio.push_back(
						glm::vec3(grownPolygons[i].vertex[j].x,
								grownPolygons[i].vertex[j].y, 0.0f));
				/*printf("%f,%f\n%f,%f\n", fin.x, fin.y,
				 grownPolygons[i].vertex[j].x,
				 grownPolygons[i].vertex[j].y);*/
			}
		}
	}
	sizeModelExtremos = modelMapInicio.size();

	printf("Matriz de adyacencias\n");
	for (int i = 1; i <= num_polygons; i++) {
		for (int j = 1; j <= NUM_MAX_VERTEX; j++) {
			bool * adyacentes = adyacencias[(i - 0) * NUM_MAX_VERTEX + j - 1];
			for (int k = 4; k <= num_polygons; k++) {
				for (int l = 1; l <= NUM_MAX_VERTEX; l++) {
					printf("%d,", adyacentes[(k - 0) * NUM_MAX_VERTEX + l - 1]);
				}
			}
			printf("\n");
		}
	}
	printf("DFS\n");
	glm::vec3 anterior;
	for (int i = 0; i <= listaExpacionDFS.anteriores; i++) {
		if (i != listaExpacionDFS.anteriores
				&& listaExpacionDFS.anterioresPoligono[i] == 0) {
			anterior = glm::vec3(inicio.x, inicio.y, 0.0f);
			//modelRuta.push_back(anterior);
			printf("%f,%f\n", inicio.x, inicio.y);
		} else if (i == listaExpacionDFS.anteriores) {
			modelRutaDFS.push_back(anterior);
			modelRutaDFS.push_back(glm::vec3(fin.x, fin.y, 0.0f));
			printf("%f,%f\n", fin.x, fin.y);
		} else {
			modelRutaDFS.push_back(anterior);
			anterior =
					glm::vec3(
							grownPolygons[listaExpacionDFS.anterioresPoligono[i]].vertex[listaExpacionDFS.anterioresVertice[i]].x,
							grownPolygons[listaExpacionDFS.anterioresPoligono[i]].vertex[listaExpacionDFS.anterioresVertice[i]].y,
							0.0f);
			modelRutaDFS.push_back(anterior);

			printf("%f,%f\n",
					grownPolygons[listaExpacionDFS.anterioresPoligono[i]].vertex[listaExpacionDFS.anterioresVertice[i]].x,
					grownPolygons[listaExpacionDFS.anterioresPoligono[i]].vertex[listaExpacionDFS.anterioresVertice[i]].y);
		}
	}

	sizeModelRutaDFS = modelRutaDFS.size();

	Nodo listaExpacionDJKS = dijsktra(inicio, fin, num_polygons, poligonos,
			grownPolygons, adyacencias, sizeListaExpacion, 0.03);
	glm::vec3 anteriorDJKS;
	printf("DJKS\n");
	for (int i = 0; i <= listaExpacionDJKS.anteriores; i++) {
		if (i != listaExpacionDJKS.anteriores
				&& listaExpacionDJKS.anterioresPoligono[i] == 0) {
			anteriorDJKS = glm::vec3(inicio.x, inicio.y, 0.0f);
			printf("%f,%f\n", inicio.x, inicio.y);
		} else if (i == listaExpacionDJKS.anteriores) {
			modelRutaDJKS.push_back(anteriorDJKS);
			modelRutaDJKS.push_back(glm::vec3(fin.x, fin.y, 0.0f));
			printf("%f,%f\n", fin.x, fin.y);
		} else {
			modelRutaDJKS.push_back(anteriorDJKS);
			anteriorDJKS =
					glm::vec3(
							grownPolygons[listaExpacionDJKS.anterioresPoligono[i]].vertex[listaExpacionDJKS.anterioresVertice[i]].x,
							grownPolygons[listaExpacionDJKS.anterioresPoligono[i]].vertex[listaExpacionDJKS.anterioresVertice[i]].y,
							0.0f);
			modelRutaDJKS.push_back(anteriorDJKS);
			printf("%f,%f\n",
					grownPolygons[listaExpacionDJKS.anterioresPoligono[i]].vertex[listaExpacionDJKS.anterioresVertice[i]].x,
					grownPolygons[listaExpacionDJKS.anterioresPoligono[i]].vertex[listaExpacionDJKS.anterioresVertice[i]].y);
		}
	}

	sizeModelRutaDJKS = modelRutaDJKS.size();
	CreateVBO(&modelPolygon, &modelGrowPolygon, &modelMap, &modelMapInicio,
			&modelRutaDFS, &modelRutaDJKS);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void InitWindow(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(4, 5);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(
	GLUT_ACTION_ON_WINDOW_CLOSE,
	GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(CurrentWidth, CurrentHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);
	if (WindowHandle < 1) {
		fprintf(
		stderr, "ERROR: Could not create a new rendering window.\n");
		exit(EXIT_FAILURE);
	}
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);

	/*
	 * 5.- Se agrega función que se encarga de finalizar la ejecución.
	 */
	glutCloseFunc(Cleanup);

	glutKeyboardFunc(KeyboardFunction);
}

void KeyboardFunction(unsigned char Key, int X, int Y) {
	switch (Key) {
	case '1': {
		ActiveIndexBuffer = 1;
		break;
	}
	case '2': {
		ActiveIndexBuffer = 2;
		break;
	}
	case '3': {
		ActiveIndexBuffer = 3;
		break;
	}
	case '4': {
		ActiveIndexBuffer = 4;
		break;
	}
	default:
		break;
	}
}

void ResizeFunction(int Width, int Height) {
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void) {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	/*
	 * 7.- Se dibuja el contenido de VBO.
	 * Primer parámetro es el tipo de Matriz.
	 * Segundo parámetro es el índice del primer vértice.
	 * Cuantos de los índices están Habilitados.
	 */
	/*glBindVertexArray(VaoId[0]);
	 glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	 glDrawArrays(GL_LINE, 0, sizePolygon);*/

	glBindVertexArray(VaoId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glDrawArrays(GL_LINES, 0, sizePolygon);
	glPointSize(3.0f);

	/*glBindVertexArray(VaoId[1]);
	 glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	 glDrawArrays(GL_LINES, 0, sizePolygon);
	 glPointSize(3.0f);*/

	if (ActiveIndexBuffer == 1) {
		glBindVertexArray(VaoId[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
		glDrawArrays(GL_LINES, 0, sizeModelMap);
	} else if (ActiveIndexBuffer == 2) {
		glBindVertexArray(VaoId[3]);
		glBindBuffer(GL_ARRAY_BUFFER, VboId[3]);
		glDrawArrays(GL_LINES, 0, sizeModelExtremos);
	} else if (ActiveIndexBuffer == 3) {
		glBindVertexArray(VaoId[4]);
		glBindBuffer(GL_ARRAY_BUFFER, VboId[4]);
		glDrawArrays(GL_LINES, 0, sizeModelRutaDFS);
	} else if (ActiveIndexBuffer == 4) {
		glBindVertexArray(VaoId[5]);
		glBindBuffer(GL_ARRAY_BUFFER, VboId[5]);
		glDrawArrays(GL_LINES, 0, sizeModelRutaDJKS);
	}

	glutSwapBuffers();
}

void IdleFunction(void) {
	glutPostRedisplay();
}

void TimerFunction(int Value) {
	if (0 != Value) {
		char* TempString = (char*) malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(TempString, "%s: %d Frames Per Second @ %d x %d",
		WINDOW_TITLE_PREFIX, FrameCount * 4, CurrentWidth, CurrentHeight);

		glutSetWindowTitle(TempString);
		free(TempString);
	}
	FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}

/*
 * 6.- Se destruyen los Shaders y VBO
 */
void Cleanup(void) {
	DestroyShaders();
	DestroyVBO();
}

void CreateVBO(std::vector<glm::vec3> * modelPolygon,
		std::vector<glm::vec3> * modelGrowPolygon,
		std::vector<glm::vec3> * modelMap,
		std::vector<glm::vec3> * modelMapExtremos,
		std::vector<glm::vec3> * modelRutaDFS,
		std::vector<glm::vec3> * modelRutaDJKS) {

	GLenum ErrorCheckValue = glGetError();
	size_t BufferSize = modelPolygon->size() * sizeof(glm::vec3);
//const size_t BufferSize = modelPoints.size();
	const size_t VertexSize = sizeof(modelPolygon[0]);

	glGenBuffers(6, VboId);

	printf("BufferSize == %zu\n", BufferSize);
	printf("VertexSize == %zu\n", VertexSize);

	glGenVertexArrays(6, VaoId);
	glBindVertexArray(VaoId[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelPolygon->data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(black));

	glEnableVertexAttribArray(0);

	glBindVertexArray(VaoId[1]);

	BufferSize = modelGrowPolygon->size() * sizeof(glm::vec3);

	printf("BufferSize == %zu\n", BufferSize);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelGrowPolygon->data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(black));

	glEnableVertexAttribArray(0);

	glBindVertexArray(VaoId[2]);

	BufferSize = modelMap->size() * sizeof(glm::vec3);

	printf("BufferSize == %zu\n", BufferSize);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelMap->data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(black));

	glEnableVertexAttribArray(0);

	glBindVertexArray(VaoId[3]);

	BufferSize = modelMapExtremos->size() * sizeof(glm::vec3);

	printf("BufferSize == %zu\n", BufferSize);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[3]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelMapExtremos->data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(black));

	glEnableVertexAttribArray(0);

	glBindVertexArray(VaoId[4]);

	BufferSize = modelRutaDFS->size() * sizeof(glm::vec3);

	printf("BufferSize == %zu\n", BufferSize);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[4]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelRutaDFS->data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(black));

	glEnableVertexAttribArray(0);

	glBindVertexArray(VaoId[5]);

	BufferSize = modelRutaDJKS->size() * sizeof(glm::vec3);

	printf("BufferSize == %zu\n", BufferSize);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[5]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelRutaDJKS->data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(black));

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR) {
		fprintf(
		stderr, "ERROR: Could not create a VBO: %s\n",
				gluErrorString(ErrorCheckValue));

		exit(-1);
	}
}

/*
 * 9.-
 */
void DestroyVBO(void) {
	GLenum ErrorCheckValue = glGetError();

	/*
	 * Se deshabilita los atributos de vértice.
	 */
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	/*
	 * Se enlaza el buffer con el tipo objetivo.
	 * Se eliminan los buffers.
	 */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(5, VboId);

	/*
	 * Se Enlazan y elimina el VAO.
	 */
	glBindVertexArray(0);
	glDeleteVertexArrays(5, VaoId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR) {
		fprintf(
		stderr, "ERROR: Could not destroy the VBO: %s \n",
				gluErrorString(ErrorCheckValue));

		exit(-1);
	}
}

/*
 * 10.-
 */
void CreateShaders(void) {
	GLenum ErrorCheckValue = glGetError();

	/*
	 * Antes de que un Shader pueda ser ejecutado debe ser compilado y ligado.
	 * Se crean los Shaders tanto para los Vertex Shader como el  Fragment Shader.
	 */
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	/*
	 * Se copia el código Shader para su compilación.
	 * Segundo parámetro es el número de cadenas del Shader Code.
	 */
	glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
	/*
	 * Se compila el Shader.
	 */
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
	glCompileShader(FragmentShaderId);

	/*
	 * Una vez creado los Shaders se deben combinar con un programa de Shader.
	 * Se crea el programa, se puede pensar un Shader programa como un ejecutable.
	 */
	ProgramId = glCreateProgram();
	/*
	 * Se adjuntan los Shaders al programa.
	 */
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	/*
	 * El último paso es ligarlo.
	 */
	glLinkProgram(ProgramId);
	/*
	 * Se crea el programa con shaders.
	 */
	glUseProgram(ProgramId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR) {
		fprintf(
		stderr, "ERROR: Could not create the shaders: %s \n",
				gluErrorString(ErrorCheckValue));

		exit(-1);
	}
}

/*
 * 11.-
 */
void DestroyShaders(void) {
	GLenum ErrorCheckValue = glGetError();

	/*
	 * Se le indica a OpenGL que se dejó de utilizar los shaders sin aprobación.
	 */
	glUseProgram(0);

	/*
	 * Se desconecta el Shader fragment y el Vertex Shader.
	 */
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	/*
	 * Se eliminan los Shaders.
	 */
	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	/*
	 * Se elimina el programa Shader.
	 */
	glDeleteProgram(ProgramId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR) {
		fprintf(
		stderr, "ERROR: Could not destroy the shaders: %s \n",
				gluErrorString(ErrorCheckValue));

		exit(-1);
	}
}
