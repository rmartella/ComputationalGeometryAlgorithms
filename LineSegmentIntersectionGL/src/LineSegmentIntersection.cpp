#include "DataStructures/Tree/Avltree.h"
#include "DataStructures/Tree/Statustree.h"
#include "Util/UtilityIntersection.h"
#include "model/Segment.h"
#include "model/Result.h"
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#define WINDOW_TITLE_PREFIX "GL"

/*
 #include "DataStructures/Tree/Node.h"
 #include "stddef.h"
 #include <iostream>
 */

using namespace std;

#define N 10

typedef struct {
	float XYZW[4];
	float RGBA[4];
} Vertex;

int CurrentWidth = 600, CurrentHeight = 600, WindowHandle = 0;
unsigned FrameCount = 0;

/*
 * 1.- Variables globales que contiene los identificadores para los shaders y buffer objects
 */
GLuint VertexShaderId, FragmentShaderId, ProgramId, VaoId, BufferId,
		IndexBufferId;
/*float puntos[][2][2] = { { { 0, 0 }, { 3, 3 } }, { { 0, 4 }, { 3, 1 } }, { { 1,
 0 }, { 3, 2.0 } }, { { 2, 3 }, { 4, 2 } }, { { 3.5, 1 }, { 4, 4 } }, { {
 6, 4 }, { 5, 2 } } };*/
/*float puntos[][2][2] = { { { 0, 0 }, { 3, 3 } }, { { 0, 4 }, { 3, 1 } }, { { 1,
 0 }, { 3, 2.0 } }, { { 2, 3 }, { 4, 2 } }, { { 3.5, 1 }, { 4, 4 } }, { {
 6, 4 }, { 5, 2 } }, { { 8, 1.5 }, { 5, 2.5 } } };*/
/*float puntos[][2][2] = { { { 3, 2.0 }, { 3, 3 } }, { { 0, 4 }, { 3, 1 } }, { {
 1, 0 }, { 0, 0 } }, { { 2, 3 }, { 4, 2 } }, { { 3.5, 1 }, { 4, 4 } }, {
 { 6, 4 }, { 5, 2 } }, { { 8, 1.5 }, { 5, 2.5 } } };*/

float puntos[][2][2] = { { { 0, 0 }, { 2, 3 } }, { { 1, 3 }, { 3, 1 } }, { { -2,
		0.5 }, { -0.5, 2 } }, { { 4, 1 }, { 3, 3 } }, { { -3, 0 }, { 0, 3 } }, {
		{ 1, 0 }, { -2, 3 } }, { { 4, 0 }, { 5, 2.5 } },
		{ { 2, 1 }, { 3.5, 3 } }, { { 1.5, 1.5 }, { 5, 3 } }, { { -4, 4 }, { -1,
				1 } } };

//float puntos[][2][2] = { { { 1, 2 }, { 3, 0 } }, { { 5, 2 }, { 2, 1 } } };
/*
 * 2.- GLSL vertex
 */
/*
 * ex_Color es la salida de nuestro Shader, sirve como intercambio entre varios Shaders.
 */
const GLchar* VertexShader = { "#version 450\n"

		"layout(location=0) in vec4 in_Position;\n"
		"layout(location=1) in vec4 in_Color;\n"
		"out vec4 ex_Color;\n"

		"void main(void)\n"
		"{\n"
		"  gl_Position = in_Position;\n"
		"  ex_Color = in_Color;\n"
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

Avltree initiTreeEventPoints(float puntos[][2][2]);
void procesaEvento(EventPoint event, Statustree *t, Avltree * eventTree);
void Initialize(int, char*[], Vertex Vertices[], GLubyte * Indices,
		int noVertices);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);

/*
 * 3.- Se agregan las declaraciones de nuevas funciones.
 */
void Cleanup(void);
void CreateVBO(Vertex * Vertices, GLubyte *Indices, int noVertices);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);

UtilInt::UtilityIntersection util;

vector<Result::Result> resultado;

/*Vertex * Vertices = (Vertex *) malloc(2 * size_t(sizeof(Vertex)));*/
/*Vertex Vertices[2] = {
 { { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, { { -0.2f,
 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } } };*/
//Vertex Vertices[2];
//GLubyte Indices[] = { 0, 1 };
int main(int argc, char* argv[]) {

	Avltree t = initiTreeEventPoints(puntos);
	cout << endl;
	t.inorder(t.root);
	cout << endl;
	t.preorder(t.root);
	cout << endl;
	t.postorder(t.root);
	cout << endl;

	Statustree status;
	while (t.root != NULL) {
		Node * node = t.getMax(t.root);
		cout << "Orden de eventos" << endl;
		t.inorder(t.root);
		cout << endl;
		printf("Procesando evento, (%f,%f)\n",
				node->event.getPointEvent().getX(),
				node->event.getPointEvent().getY());
		procesaEvento(node->event, &status, &t);
		t.remove(node->event);
	}
	cout << "Termina" << endl;
	printf("Total de cruces=%d\n", resultado.size());
	for (int i = 0; i < resultado.size(); i++) {
		printf("Interseccion S1={(%f,%f),(%f,%f)}, S2={(%f,%f),(%f,%f)}\n",
				resultado[i].getSegement1().getPoint1().getX(),
				resultado[i].getSegement1().getPoint1().getY(),
				resultado[i].getSegement1().getPoint2().getX(),
				resultado[i].getSegement1().getPoint2().getY(),
				resultado[i].getSegement2().getPoint1().getX(),
				resultado[i].getSegement2().getPoint1().getY(),
				resultado[i].getSegement2().getPoint2().getX(),
				resultado[i].getSegement2().getPoint2().getY());
	}

	/*Vertex Vertices[2];
	 Vertex vertice1 = { { 0.0f, 0.0f, 0.0f, 1.0 }, { 0.0f, 0.0f, 1.0f, 1.0f } };
	 Vertices[0] = vertice1;
	 Vertex vertice2 = { { -0.2, 0.8, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 } };
	 Vertices[1] = vertice2;*/
//Vertex * Vertices = initPointGL(puntos, 1);
	int tam = N * 2 + resultado.size() * 4;
	GLubyte Indices[tam];
	Vertex Vertices[tam];
	for (int i = 0, j = 0; i < N; i++, j++) {
		Vertex vertice1 = { { puntos[i][0][0] / 10, puntos[i][0][1] / 10, 0.0f,
				1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
		Vertex vertice2 = { { puntos[i][1][0] / 10, puntos[i][1][1] / 10, 0.0f,
				1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
		printf("puntos P(%f,%f)\n", puntos[i][0][0] / 10, puntos[i][0][1] / 10);
		printf("puntos P(%f,%f)\n", puntos[i][1][0] / 10, puntos[i][1][1] / 10);
		Indices[j] = j;
		Vertices[j++] = vertice1;
		Indices[j] = j;
		Vertices[j] = vertice2;
	}
	for (unsigned int i = 0, j = N * 2; i < resultado.size(); i++, j++) {
		Vertex vertice1 = { { resultado[i].getSegement1().getPoint1().getX()
				/ 10, resultado[i].getSegement1().getPoint1().getY() / 10, 0.0f,
				1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } };
		Vertex vertice2 = { { resultado[i].getSegement1().getPoint2().getX()
				/ 10, resultado[i].getSegement1().getPoint2().getY() / 10, 0.0f,
				1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } };
		Indices[j] = j;
		Vertices[j++] = vertice1;
		Indices[j] = j;
		Vertices[j++] = vertice2;
		vertice1 = { {resultado[i].getSegement2().getPoint1().getX()
				/ 10, resultado[i].getSegement2().getPoint1().getY() / 10, 0.0f,
				1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}};
		vertice2 = { {resultado[i].getSegement2().getPoint2().getX()
				/ 10, resultado[i].getSegement2().getPoint2().getY() / 10, 0.0f,
				1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}};
		Indices[j] = j;
		Vertices[j++] = vertice1;
		Indices[j] = j;
		Vertices[j] = vertice2;
	}
	Initialize(argc, argv, Vertices, Indices, tam);
	glutMainLoop();
	exit(EXIT_SUCCESS);

}

Avltree initiTreeEventPoints(float puntos[][2][2]) {
	Avltree * tree = new Avltree();
	printf("Tamanio inicialización %d\n", N);
	for (unsigned int i = 0; i < N; i++) {
		EventPoint * eventPoint1 = (EventPoint *) malloc(
				size_t(sizeof(EventPoint)));
		EventPoint * eventPoint2 = (EventPoint *) malloc(
				size_t(sizeof(EventPoint)));
		Segment * segment = (Segment*) malloc(size_t(sizeof(Segment)));
		Point point1 = Point(puntos[i][0][0], puntos[i][0][1]);
		Point point2 = Point(puntos[i][1][0], puntos[i][1][1]);
		printf("puntos P(%f,%f)\n", puntos[i][0][0], puntos[i][0][1]);
		printf("puntos P(%f,%f)\n", puntos[i][1][0], puntos[i][1][1]);
		segment->setPoint1(point1);
		segment->setPoint2(point2);
		eventPoint1->setSegment(*segment);
		eventPoint2->setSegment(*segment);
		eventPoint1->setPointEvent(point1);
		eventPoint2->setPointEvent(point2);
		if (point1.getY() > point2.getY()) {
			eventPoint1->setType(upperendpoint);
			eventPoint2->setType(lowerendpoint);
		} else if (point1.getY() < point2.getY()) {
			eventPoint1->setType(lowerendpoint);
			eventPoint2->setType(upperendpoint);
		} else if (point1.getY() == point2.getY()
				&& point1.getX() < point2.getX()) {
			eventPoint1->setType(upperendpoint);
			eventPoint2->setType(lowerendpoint);
		} else {
			eventPoint1->setType(lowerendpoint);
			eventPoint2->setType(upperendpoint);
		}
		printf("eventPoint1 P(%f,%f)\n", eventPoint1->getPointEvent().getX(),
				eventPoint1->getPointEvent().getY());
		printf("eventPoint2 P(%f,%f)\n\n", eventPoint2->getPointEvent().getX(),
				eventPoint2->getPointEvent().getY());
		tree->insert(*eventPoint1);
		tree->insert(*eventPoint2);
	}
	return *tree;
}

void procesaEvento(EventPoint event, Statustree * tree, Avltree * eventTree) {
	if (event.getTypeEventPoint() == upperendpoint) {
		cout << "Evento de entrada" << endl;
		Point point = event.getPointEvent();
		Segment segment = event.getSegment();
		tree->insert(segment, point);
		StatusNode ** iz = (StatusNode **) malloc(size_t(sizeof(StatusNode *)));
		StatusNode ** de = (StatusNode **) malloc(size_t(sizeof(StatusNode *)));
		tree->inorder(tree->root);
		cout << endl;
		tree->getneighbors(tree->root, segment, iz, de);
		if (*de != NULL && util.isIntersect(segment, (*de)->key)) {
			Point * p = util.getIntersection(segment, (*de)->key);
			//TODO VALIDAR CASOS DEGENREADOS
			if (p->getY() < event.getPointEvent().getY()) {
				EventPoint * event = (EventPoint *) malloc(
						size_t(sizeof(EventPoint)));
				event->setPointEvent(*p);
				event->setSegment(segment);
				event->setSegmentInt((*de)->key);
				event->setType(intersection);
				eventTree->insert(*event);
			}
		}
		if (*iz != NULL && util.isIntersect((*iz)->key, segment)) {
			Point * p = util.getIntersection((*iz)->key, segment);
			//TODO VALIDAR CASOS DEGENREADOS
			if (p->getY() < event.getPointEvent().getY()) {
				EventPoint * event = (EventPoint *) malloc(
						size_t(sizeof(EventPoint)));
				event->setPointEvent(*p);
				event->setSegment((*iz)->key);
				event->setSegmentInt(segment);
				event->setType(intersection);
				eventTree->insert(*event);
			}
		}
	} else if (event.getTypeEventPoint() == lowerendpoint) {
		cout << "Evento de salida" << endl;
		Point point = event.getPointEvent();
		StatusNode ** iz = (StatusNode **) malloc(size_t(sizeof(StatusNode *)));
		StatusNode ** de = (StatusNode **) malloc(size_t(sizeof(StatusNode *)));
		Segment segment = event.getSegment();
		/*tree->inorder(tree->root);
		 cout << endl;
		 tree->preorder(tree->root);
		 cout << endl;
		 tree->postorder(tree->root);
		 cout << endl;*/
		tree->inorder(tree->root);
		cout << endl;
		tree->point = point;
		tree->getneighbors(tree->root, segment, iz, de);
		tree->remove(segment, point);
		if (*iz != NULL && *de != NULL
				&& util.isIntersect((*iz)->key, (*de)->key)) {
			Point * p = util.getIntersection((*iz)->key, (*de)->key);
			//TODO VALIDAR CASOS DEGENREADOS
			if (p->getY() < event.getPointEvent().getY()) {
				EventPoint * event = (EventPoint *) malloc(
						size_t(sizeof(EventPoint)));
				event->setPointEvent(*p);
				event->setSegment((*iz)->key);
				event->setSegmentInt((*de)->key);
				event->setType(intersection);
				eventTree->insert(*event);
			}
		}
		free(iz);
		free(de);
	} else if (event.getTypeEventPoint() == intersection) {
		cout << "Evento de interseccion" << endl;
		resultado.push_back(
				Result::Result(event.getSegment(), event.getSegmentInt()));
		Segment segment1 = event.getSegment();
		Segment segment2 = event.getSegmentInt();
		cout << "Sin remover" << endl;
		tree->inorder(tree->root);
		cout << endl;
		/*cout << "Antes de cambiarlos" << endl;
		 tree->inorder(tree->root);
		 cout << endl;
		 tree->preorder(tree->root);
		 cout << endl;
		 tree->postorder(tree->root);
		 cout << endl;*/
		float epsilon = 0.00005;
		Point pdesp = Point(0.0, event.getPointEvent().getY() + epsilon);
		tree->remove(segment1, pdesp);
		tree->remove(segment2, pdesp);
		cout << "Con remover" << endl;
		tree->inorder(tree->root);
		cout << endl;
		/*cout << "Despues de eliminarlos" << endl;
		 tree->inorder(tree->root);
		 cout << endl;
		 tree->preorder(tree->root);
		 cout << endl;
		 tree->postorder(tree->root);
		 cout << endl;*/
		pdesp = Point(0.0, event.getPointEvent().getY() - epsilon);
		tree->insert(segment1, pdesp);
		tree->insert(segment2, pdesp);
		cout << "Intercambiadas" << endl;
		tree->inorder(tree->root);
		StatusNode ** iz1 = (StatusNode **) malloc(
				size_t(sizeof(StatusNode *)));
		StatusNode ** iz2 = (StatusNode **) malloc(
				size_t(sizeof(StatusNode *)));
		StatusNode ** de1 = (StatusNode **) malloc(
				size_t(sizeof(StatusNode *)));
		StatusNode ** de2 = (StatusNode **) malloc(
				size_t(sizeof(StatusNode *)));
		tree->getneighbors(tree->root, segment2, iz1, de1);
		tree->getneighbors(tree->root, segment1, iz2, de2);
		if (*iz1 != NULL && util.isIntersect((*iz1)->key, segment2)) {
			Point * p = util.getIntersection((*iz1)->key, segment2);
			//TODO VALIDAR CASOS DEGENREADOS
			if (p->getY() < event.getPointEvent().getY()) {
				EventPoint * event = (EventPoint *) malloc(
						size_t(sizeof(EventPoint)));
				event->setPointEvent(*p);
				event->setSegment((*iz1)->key);
				event->setSegmentInt(segment2);
				event->setType(intersection);
				eventTree->insert(*event);
			}
		}
		if (*de2 != NULL && util.isIntersect((*de2)->key, segment1)) {
			Point * p = util.getIntersection((*de2)->key, segment1);
			//TODO VALIDAR CASOS DEGENREADOS
			if (p->getY() < event.getPointEvent().getY()) {
				EventPoint * event = (EventPoint *) malloc(
						size_t(sizeof(EventPoint)));
				event->setPointEvent(*p);
				event->setSegment(segment1);
				event->setSegmentInt((*de2)->key);
				event->setType(intersection);
				eventTree->insert(*event);
			}
		}
		free(iz1);
		free(de1);
		free(iz2);
		free(de2);

		tree->point = event.getPointEvent();
		/*cout << "Despues de cambiarlos" << endl;
		 tree->inorder(tree->root);
		 cout << endl;
		 tree->preorder(tree->root);
		 cout << endl;
		 tree->postorder(tree->root);
		 cout << endl;*/
	}
}

void Initialize(int argc, char* argv[], Vertex * Vertices, GLubyte * Indices,
		int noVertices) {
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
	CreateVBO(Vertices, Indices, noVertices);

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
	glDrawElements(GL_LINES, 100, GL_UNSIGNED_BYTE,
			(GLvoid*) (sizeof(GLubyte) * 0));

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

void CreateVBO(Vertex * Vertices, GLubyte * Indices, int noVertices) {

	GLenum ErrorCheckValue = glGetError();

	const size_t BufferSize = 8 * sizeof(Vertices) * noVertices;
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t RgbOffset = sizeof(Vertices[0].XYZW);

	glGenBuffers(1, &BufferId);

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glBindBuffer(GL_ARRAY_BUFFER, BufferId);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);

	printf("floatSize == %d\n", sizeof(float));
	printf("BufferSize == %zu\n", BufferSize);
	printf("VertexSize == %zu\n", VertexSize);
	printf("RgbOffset == %zu\n", RgbOffset);
	printf("sizeof Indices == %zu\n", sizeof(Indices));

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize,
			(GLvoid*) RgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &IndexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, noVertices, Indices,
	GL_STATIC_DRAW);

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
	glDeleteBuffers(1, &BufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IndexBufferId);

	/*
	 * Se Enlazan y elimina el VAO.
	 */
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);

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
