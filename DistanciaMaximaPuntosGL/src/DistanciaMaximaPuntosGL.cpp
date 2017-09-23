#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ConvexHull.h"
#include "Utility.h"
#include "DistanceMax.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define WINDOW_TITLE_PREFIX "Ejemplo 2"

#define BUFFER_OFFSET(offset) ((void*)(offset))

typedef struct {
	float XYZW[4];
	float RGBA[4];
} Vertex;

int CurrentWidth = 800, CurrentHeight = 600, WindowHandle = 0;
unsigned FrameCount = 0;

/*
 * 1.- Variables globales que contiene los identificadores para los shaders y buffer objects
 */
GLuint VertexShaderId, FragmentShaderId, ProgramId, VaoId[3], VboId[3],
		g_uniformColor;

GLuint sizeConvex;

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

int tamanioPuntos;

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
void CreateVBO(std::vector<glm::vec3> modelPoints,
		std::vector<glm::vec3> modelConvex,
		std::vector<glm::vec3> modelLineMax);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);

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

	std::vector<glm::vec3> modelPoints;
	std::vector<glm::vec3> modelConvex;
	std::vector<glm::vec3> modelLineMax;
	std::vector<ConvexHull::Punto> puntosConvert = parserPointsFormFile(
			"coordenadas.txt");
	if (puntosConvert.size() > 2) {
		for (unsigned int i = 0; i < puntosConvert.size(); i++) {
			modelPoints.push_back(
					glm::vec3(puntosConvert[i].x * 0.005,
							puntosConvert[i].y * 0.005, 0));
		}
		tamanioPuntos = puntosConvert.size();
		ConvexHull::ConvexHull algoConvex = ConvexHull::ConvexHull(
				puntosConvert);
		/* Se calcula el cierre convexo*/
		ConvexHull::ptrPila pila = algoConvex.calConvexHull();
		std::vector<ConvexHull::Punto> pointsConvexHullVector;
		int pareja = 1;
		/* Se vacia la pila que contiene el cierre convexo en un vector*/
		while (ConvexHull::ConvexHull::pila_vacia(&pila) != 1) {
			ConvexHull::Punto punto = ConvexHull::ConvexHull::pop(&pila);
			pointsConvexHullVector.push_back(punto);
			modelConvex.push_back(
					glm::vec3(punto.x * 0.005, punto.y * 0.005, 0));
			if (pareja == 2) {
				modelConvex.push_back(modelConvex[modelConvex.size() - 1]);
				pareja = 1;
			}
			pareja++;
		}

		sizeConvex = modelConvex.size();

		DistanceMax::DistanceMax algoDistance = DistanceMax::DistanceMax(
				pointsConvexHullVector);
		/* Se obtiene la pareja de puntos mas distantes*/
		std::vector<ConvexHull::Punto> pointsMaxDistance =
				algoDistance.getPointsMaxDistance();

		for (unsigned int i = 0; i < pointsMaxDistance.size(); i++) {
			printf("P%d(%f,%f)\n", i + 1, pointsMaxDistance[i].x,
					pointsMaxDistance[i].y);
			modelLineMax.push_back(
					glm::vec3(pointsMaxDistance[i].x * 0.005,
							pointsMaxDistance[i].y * 0.005, 0.0f));
		}
	} else {
		printf("Puntos mas distantes\n");
		if (puntosConvert.size() > 0) {
			if (puntosConvert.size() == 1) {
				printf("P%d(%f,%f)\n", 1, puntosConvert[0].x,
						puntosConvert[0].y);
				modelPoints.push_back(
						glm::vec3(puntosConvert[0].x * 0.005,
								puntosConvert[0].y * 0.005, 0));
				modelConvex.push_back(
						glm::vec3(puntosConvert[0].x * 0.005,
								puntosConvert[0].y * 0.005, 0));
				modelLineMax.push_back(
						glm::vec3(puntosConvert[0].x * 0.005,
								puntosConvert[0].y * 0.005, 0.0f));
			}
			if (puntosConvert.size() == 2) {
				printf("P%d(%f,%f)\n", 1, puntosConvert[0].x,
						puntosConvert[0].y);
				printf("P%d(%f,%f)\n", 2, puntosConvert[1].x,
						puntosConvert[1].y);
				modelPoints.push_back(
						glm::vec3(puntosConvert[0].x * 0.005,
								puntosConvert[0].y * 0.005, 0));
				modelPoints.push_back(
						glm::vec3(puntosConvert[1].x * 0.005,
								puntosConvert[1].y * 0.005, 0));
				modelConvex.push_back(
						glm::vec3(puntosConvert[0].x * 0.005,
								puntosConvert[0].y * 0.005, 0));
				modelConvex.push_back(
						glm::vec3(puntosConvert[1].x * 0.005,
								puntosConvert[1].y * 0.005, 0));
				modelLineMax.push_back(
						glm::vec3(puntosConvert[0].x * 0.005,
								puntosConvert[0].y * 0.005, 0.0f));
				modelLineMax.push_back(
						glm::vec3(puntosConvert[1].x * 0.005,
								puntosConvert[1].y * 0.005, 0.0f));
			}
		}
	}

	/*printf("Puntos mas distantes\n");

	 for (unsigned int i = 0; i < pointsMaxDistance.size(); i++)
	 printf("P%d(%f,%f)\n", i + 1, pointsMaxDistance[i].x,
	 pointsMaxDistance[i].y);*/

	CreateVBO(modelPoints, modelConvex, modelLineMax);

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
}

void ResizeFunction(int Width, int Height) {
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void) {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * 7.- Se dibuja el contenido de VBO.
	 * Primer parámetro es el tipo de Matriz.
	 * Segundo parámetro es el índice del primer vértice.
	 * Cuantos de los índices están Habilitados.
	 */
	glBindVertexArray(VaoId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glDrawArrays(GL_POINTS, 0, tamanioPuntos);
	glPointSize(3.0f);

	glBindVertexArray(VaoId[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glDrawArrays(GL_LINE_LOOP, 0, sizeConvex);

	glBindVertexArray(VaoId[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
	glDrawArrays(GL_LINES, 0, 2);

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

void CreateVBO(std::vector<glm::vec3> modelPoints,
		std::vector<glm::vec3> modelConvex,
		std::vector<glm::vec3> modelLineMax) {

	GLenum ErrorCheckValue = glGetError();
	size_t BufferSize = modelPoints.size() * sizeof(glm::vec3);
//const size_t BufferSize = modelPoints.size();
	const size_t VertexSize = sizeof(modelPoints[0]);

	glGenBuffers(3, VboId);

	printf("BufferSize == %zu\n", BufferSize);
	printf("VertexSize == %zu\n", VertexSize);

	glGenVertexArrays(3, VaoId);
	glBindVertexArray(VaoId[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelPoints.data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(red));

	glEnableVertexAttribArray(0);

	glBindVertexArray(VaoId[1]);

	BufferSize = modelConvex.size() * sizeof(glm::vec3);

	printf("BufferSize == %zu\n", BufferSize);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelConvex.data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(blue));

	glEnableVertexAttribArray(0);

	glBindVertexArray(VaoId[2]);

	BufferSize = modelLineMax.size() * sizeof(glm::vec3);

	printf("BufferSize == %zu\n", BufferSize);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, modelLineMax.data(),
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	g_uniformColor = glGetUniformLocation(ProgramId, "in_Color");

	glUniform3fv(g_uniformColor, 1, glm::value_ptr(blue));

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
	glDeleteBuffers(1, VboId);

	/*
	 * Se Enlazan y elimina el VAO.
	 */
	glBindVertexArray(0);
	glDeleteVertexArrays(1, VaoId);

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
