/*
 * main.cpp
 *
 *  Created on: 04/03/2016
 *      Author: rey
 */

#include <cstdio>
#include <vector>

#include "inc/Camera.h"

#include "definition.h"
#include "parserFileWRL.h"
#include "utilSimulator.h"

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

/*Definene el tamaño de la ventana*/
int g_iWindowWidth = 800;
int g_iWindowHeight = 600;
/*En esta variable se almacena el id de la ventana que se va a crear con freeglut*/
int g_iWindowHandle = 0;

/*Variables que se usan para el manejo de las teclas y poder girar la camara alrededor de la escena*/
int g_W, g_A, g_S, g_D, g_Q, g_E;
/*Variable que indica si se esta desplazando a la izquierda o la derecha */
bool g_bShift = false;

/*Variable que almacena el movimiento del mouse, esto es requerido para calcular hasta
 * que punto el mouse se ha movido entre los frames
 */
glm::ivec2 g_MousePos;

/*Esta variable es un cuaternion que es usado para almacenar la rotacion del objeto en la escena
 *
 */
glm::quat g_Rotation;

/*Vairables que son usadas para calcular canto tiempo ha pasado entre frames*/
std::clock_t g_PreviousTicks;
std::clock_t g_CurrentTicks;

/*Incialización de la camara*/
Camera g_Camera;
glm::vec3 g_InitialCameraPosition;
glm::quat g_InitialCameraRotation;

/*Se define una estrctura que almacena la posición y el color de los vertices*/
struct VertexXYZColor {
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
};

std::vector<VertexXYZColor> vertexPolygon;
std::vector<VertexXYZColor> vertexCentroid;
std::vector<VertexXYZColor> vertexDV;

/* Variable que se utiliza para referenciar Vertex Array Object, se utiliza para enlazar todos
 * atributos de vertice y los indices en un solo argumento
 */
GLuint g_vaos[3];

GLuint g_vbos[3];
/* Se utiliza para almacena la refrencia al shader compilado y poder ligarlo al programa,
 * El shader program combina el Vertex shader y el fragment shader en un solo objeto
 * Debemos pensar el shader program como un programa que se ejecuta en la GPU en lugar de la CPU.
 * Los shaders individuales (Vertex y Fragment) primero necesitan ser compilados en forma de codigo objeto
 * los shaders compilados deben ser ligados a un shader program el cual puede ser ejecutado por la GPU
 */
GLuint g_ShaderProgram = 0;
/* Esta variable es usada para hacer referencia a otra variable que esta definida en el program shader.
 * El acronimo MVP indica que la variable del shader define la concatenación model-view-projection matrix
 * que es usada para transformar los vertices del cubo
 */
GLint g_uniformMVP = -1;

/* Función que será llamada cuando ningun evento necesite ser manejado, actualiza la logica del juego*/
void IdleGL();
/* Función que será llamada siempre que el contenido de la ventana necesite ser actalizada
 * Por ejemplo si algo en la escena se mueve o si se redimensiona la pantalla
 */
void DisplayGL();
/* Función que maneja si una teca es presionada, esta función será llamada muchas veces si una tecla
 * se mantiene presionada
 */
void KeyboardGL(unsigned char c, int x, int y);
/* Función que maneja si una tecla deja de ser presionada*/
void KeyboardUpGL(unsigned char c, int x, int y);
/* Función que maneja del mismo modo que la función que maneja las teclas presionadas, solo que esta
 * función es llamada con teclas especiales por ejemplo (shift, control, alt, felchas, etc).
 */
void SpecialGL(int key, int x, int y);
/* Función que maneja si una tecla especial deja de ser presionada*/
void SpecialUpGL(int key, int x, int y);
/* Función que maneja el click del mouse en la ventana*/
void MouseGL(int button, int state, int x, int y);
/* Función que maneja cuando el mouse es arrastrado por la ventana*/
void MotionGL(int x, int y);
/* Función que será llamada cuando la pantalla es redimencionada*/
void ReshapeGL(int w, int h);

/**
 * Initialize the OpenGL context and create a render window.
 */
void InitGL(int argc, char* argv[]) {
	std::cout << "Initialize OpenGL..." << std::endl;

	/* Función que inicializa la pantalla con algunos atributos que se le pasan al main*/
	glutInit(&argc, argv);
	/* Se especifician opciones para controlar el main loop de glut, que regrese al punto del hilo
	 * principal y que termine glut al cerrar la ventana
	 */
	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE,
	GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	/* Se consulta el tamaño de la ventana, para centrar la pantalla de despliegue*/
	int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	/* Se determina como la ventana de despliegue es creada
	 * GLUT_RGBA: Se usa modo RGBA
	 * GLUT_ALPHA: Se especifica para crear buffers de colores con alpha component
	 * GLUT_DOUBLE: Es una bandera que habilita el doble buffer
	 * 				Con el doble buffer todos los comandos se ejecutan fuera de la pantalla,
	 * 				Posteriormente se envían todos los dibujos de una trama y así no mostrar
	 * 				imágenes incompletas.
	 * GLUT_DEPTH: Especifica que el framebuffer debería ser creado con buffer de profundidad,
	 * 			   sin este no se podria optimizar la prueba de profundidad y las primitivas pueden
	 * 			   aparecer en un mal orden durante el render
	 */
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	// Create an OpenGL 3.3 core forward compatible context.
	/*
	 * Se crea un contexto versión 3.3
	 * No incluye ninguna característica que se removió en versiones anteriores
	 * No contendrá características que fueron depreciadas en la versión que se está solicitando
	 */
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags( GLUT_FORWARD_COMPATIBLE);

	/* Se centra la ventana en la posición*/
	glutInitWindowPosition((iScreenWidth - g_iWindowWidth) / 2,
			(iScreenHeight - g_iWindowHeight) / 2);
	/* Se inicia los valores del tamaño de la ventana*/
	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);

	/* Se crea la ventana de despligue y se asigna al Id global para saber si fue o no creado*/
	g_iWindowHandle = glutCreateWindow("OpenGL Template");

	// Register GLUT callbacks.
	glutIdleFunc(IdleGL);
	glutDisplayFunc(DisplayGL);
	glutKeyboardFunc(KeyboardGL);
	glutKeyboardUpFunc(KeyboardUpGL);
	glutSpecialFunc(SpecialGL);
	glutSpecialUpFunc(SpecialUpGL);
	glutMouseFunc(MouseGL);
	glutMotionFunc(MotionGL);
	glutReshapeFunc(ReshapeGL);

	/* Se especifica el color del buffer*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/* Se especifica el valor con el de inicialización del buffer de profundidad*/
	glClearDepth(1.0f);
	/* Se habilita la prueba de profundidad la cual indica que las primitvas que estan mas lejos
	 * del obsevador y no sean dibujadas, la prueba de profundidad del rendering pipeline descarta
	 * Fragmentos que estan por encima de otros fragmentos
	 */
	glEnable(GL_DEPTH_TEST);
	/* Se activa el cull face, esta es para optimizar el renderizado. Habilitando el cull face permite
	 * al stage del pipeline rechazar las primitivas que que no esten frente al observador
	 */
	glEnable(GL_CULL_FACE);

	std::cout << "Initialize OpenGL Success!" << std::endl;
}

void InitGLEW() {
	glewExperimental = GL_TRUE;
	/* Se inicializa glew framework, se debe llamar despues de crear el contexto*/
	if (glewInit() != GLEW_OK) {
		std::cerr << "There was a problem initializing GLEW. Exiting..."
				<< std::endl;
		exit(-1);
	}

	// Check for 3.3 support.
	// I've specified that a 3.3 forward-compatible context should be created.
	// so this parameter check should always pass if our context creation passed.
	// If we need access to deprecated features of OpenGL, we should check
	// the state of the GL_ARB_compatibility extension.
	if (!GLEW_VERSION_3_3) {
		std::cerr << "OpenGL 3.3 required version support not present."
				<< std::endl;
		exit(-1);
	}
#ifdef _WIN32
	if ( WGLEW_EXT_swap_control )
	{
		wglSwapIntervalEXT(0); // Disable vertical sync
	}
#endif
}

// Loads a shader and returns the compiled shader object.
// If the shader source file could not be opened or compiling the
// shader fails, then this function returns 0.
GLuint LoadShader(GLenum shaderType, const char * vertex_file_path) {
	// Create the shaders
	GLuint shader = glCreateShader(shaderType);

	// Read the Vertex Shader code from the file
	std::string shaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			shaderCode += "\n" + Line;
		VertexShaderStream.close();
	} else {
		printf(
				"Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
				vertex_file_path);
		getchar();
		return 0;
	}
	/* Se crea un shader y es compilado*/
	char const *sources = shaderCode.c_str();

	glShaderSource(shader, 1, &sources, NULL);
	glCompileShader(shader);

	/* Se revisan errroes de compilación, para esto se usua la función glGetShaderiv con el parametro
	 * glGetShaderiv
	 */
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		GLint logLength;
		/* Se utiliza nuevamente la función glGetShaderiv para revisar el tamaño del log
		 * y lo guarda en infoLog el error
		 */
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);
#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		/* Se elimina la variable que almacena el error*/
		delete infoLog;
		return 0;
	}
	return shader;
}

// Create a shader program from a set of compiled shader objects.
GLuint CreateShaderProgram(std::vector<GLuint> shaders) {
	// Create a shader program.
	GLuint program = glCreateProgram();

	// Attach the appropriate shader objects.
	for (GLuint shader : shaders) {
		glAttachShader(program, shader);
	}
	// Link the program
	glLinkProgram(program);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(program, logLength, NULL, infoLog);
#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		/* Se elimina la variable que almacena el error*/
		delete infoLog;
		return 0;
	}
	return program;
}

/*Función que coloca la camara correctamente en base a las dimenciones de la ventana de despliegue*/
void ReshapeGL(int w, int h) {
	/* Se hace para que no truene el programa se valida la altura de la ventana*/
	if (h == 0) {
		h = 1;
	}

	/* Se colocan globalmente los nuevos valores de rediminsionamiento de la pantalla*/
	g_iWindowWidth = w;
	g_iWindowHeight = h;

	/* La matriz de proyección de la camara es inicializada con 60 grados campo de visión
	 * Un aspecto que coincide con el tamaño de la ventana, cerca del plano recortado 0.1
	 * y el plano alejado de 100 unidades
	 */
	g_Camera.SetViewport(0, 0, w, h);
	g_Camera.SetProjectionRH(60.0f, w / (float) h, 0.01f, 100.0f);

	/* Se le indica a GLUT que se agrega un evento de dibujo a la actual ventana, esto hace que
	 * la escena se redibuje
	 */
	glutPostRedisplay();
}

/* Función que se manda a llamar si la ventana de despliegue necesite redibujar
 * Lo cual esto es garantizado si glutPostRedisplay es invocada
 */
void DisplayGL() {
	/* Primero se asegura que estamos trabajando con un nuevo lienzo para dibujar
	 * GL_COLOR_BUFFER_BIT y GL_DEPTH_BUFFER_BIT, hace que los buffers sean colocados
	 * a los valores que se le indicaron en glClearColor y glClearDepth
	 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Se enlaza el VAO y se activa el shader program*/
	glBindVertexArray(g_vaos[0]);
	glUseProgram(g_ShaderProgram);

	/* Si la camara se mueve o rota el cubo, se necesita actualizar la matriz MVP */
	glm::mat4 mvp = g_Camera.GetProjectionMatrix() * g_Camera.GetViewMatrix()
			* glm::toMat4(g_Rotation);
	/* Se combina la matriz y enviada al shader con una variable uniform, esto se hace por medio
	 * del metodo glUniformMatrix4fv, los parametros de esta matriz son los siguientes:
	 * GLint location: Se especifica la ubicación del valor uniform para ser modificado.
	 * GLsizei count: Se especifica el número de matrices a ser modificadas, este debería de ser
	 * 				  uno si el variabel uniform objetivo no es un arreglo de matrices,
	 * 				  y uno o más si esta es un arreglo de matrices.
	 * GLboolean transpose: Especifica si lo que se desea cargar en la variable uniform es la
	 * 						transpuesta de la matriz.
	 * const GLfloat * value: Especifica el puntero a la variable uniform que se desea actualizar.
	 */
	glUniformMatrix4fv(g_uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));

	/* Se pone en marcha el renderizado de los vertices que se encuentra en el pipeline*/
	glDrawArrays(GL_LINES, 0, vertexPolygon.size());
	glBindVertexArray(g_vaos[1]);
	glPointSize(2.0f);
	glDrawArrays(GL_POINTS, 0, vertexCentroid.size());

	glBindVertexArray(g_vaos[2]);
	glDrawArrays(GL_LINES, 0, vertexDV.size());

	/* Desenlazamos el VAO y el programa shader, dejamos el estado como estaba*/
	glUseProgram(0);
	glBindVertexArray(0);

	/* Hace que el back buffer pase a ser el front buffer y puede ser desplegado en la ventana*/
	glutSwapBuffers();
}

/* La función IdleGL se ejecutara si ningun otro evento necesita ser procesado sobre la ventana
 * sin embargo en realidad no sabemos cuanto tiempo ha pasado entre las llamadas de la función idle,
 * en esta función se trata de determinar el tiempo transucrrido entre cada frame y usarla para mover
 * cosas en la escena.
 */
void IdleGL() {
	/* Se determina el tiempo que ha pasado despues de la ultima llamada a la funcion idle*/
	g_CurrentTicks = std::clock();
	float deltaTicks = (float) (g_CurrentTicks - g_PreviousTicks);
	g_PreviousTicks = g_CurrentTicks;

	float fDeltaTime = deltaTicks / (float) CLOCKS_PER_SEC;

	/* Si la tecla shift es presionada aumenta la velocidad de translación de la camara*/
	float cameraSpeed = 1.0f;
	if (g_bShift) {
		cameraSpeed = 5.0f;
	}

	/* La camara es transladada en base a la tecla que son presionados
	 * A y D: Eje X
	 * Q y E: Eje Y
	 * W y S: Eje Z
	 */
	g_Camera.Translate(
			glm::vec3(g_D - g_A, g_Q - g_E, g_S - g_W) * cameraSpeed
					* fDeltaTime);
	/* El llamado a esta funcipin asegura que sea redibujada la escena*/
	glutPostRedisplay();
}

/* Función que maneja el evento de presionar una tecla*/
void KeyboardGL(unsigned char c, int x, int y) {
	switch (c) {
	/* Si la tecla es presionada activa la tecla*/
	case 'w':
	case 'W':
		g_W = 1;
		break;
	case 'a':
	case 'A':
		g_A = 1;
		break;
	case 's':
	case 'S':
		g_S = 1;
		break;
	case 'd':
	case 'D':
		g_D = 1;
		break;
	case 'q':
	case 'Q':
		g_Q = 1;
		break;
	case 'e':
	case 'E':
		g_E = 1;
		break;
	case 'r':
	case 'R':
		/* Si se presiona la tecla R es presionada la camara es reseteada a los valores iniciales*/
		g_Camera.SetPosition(g_InitialCameraPosition);
		g_Camera.SetRotation(g_InitialCameraRotation);
		g_Rotation = glm::quat();
		break;
	case 27:
		/* Si la tecla esc es presionada el loop glut del main se termina*/
		glutLeaveMainLoop();
		break;
	}
}

/* Esta función maneja cuando se deja de presionar el mouse, solo resetea el estado de la tecla*/
void KeyboardUpGL(unsigned char c, int x, int y) {
	switch (c) {
	case 'w':
	case 'W':
		g_W = 0;
		break;
	case 'a':
	case 'A':
		g_A = 0;
		break;
	case 's':
	case 'S':
		g_S = 0;
		break;
	case 'd':
	case 'D':
		g_D = 0;
		break;
	case 'q':
	case 'Q':
		g_Q = 0;
		break;
	case 'e':
	case 'E':
		g_E = 0;
		break;

	default:
		break;
	}
}

/* Maneja el evento de la tecla de shift, solo se activa la bandera*/
void SpecialGL(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_SHIFT_L:
	case GLUT_KEY_SHIFT_R: {
		g_bShift = true;
	}
		break;
	}
}

/* Maneja el evento cuando se deja de presionar la tecla de shift, solo se resetea la bandera*/
void SpecialUpGL(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_SHIFT_L:
	case GLUT_KEY_SHIFT_R: {
		g_bShift = false;
	}
		break;
	}
}

/* Esta función se ejecuta cuando el usuario presiona el boton del mouse, se obtiene la posición
 * del click y se calcula el incremento cuando el mouse es deslizado posteriormente
 */
void MouseGL(int button, int state, int x, int y) {
	g_MousePos = glm::ivec2(x, y);
}

/* Funcion que se ejecuta cuando el mouse es deslizado*/
void MotionGL(int x, int y) {
	glm::ivec2 mousePos = glm::ivec2(x, y);
	glm::vec2 delta = glm::vec2(mousePos - g_MousePos);
	g_MousePos = mousePos;

	std::cout << "dX: " << delta.x << " dy: " << delta.y << std::endl;

	glm::quat rotX = glm::angleAxis<float>(glm::radians(delta.y) * 0.5f,
			glm::vec3(1, 0, 0));
	glm::quat rotY = glm::angleAxis<float>(glm::radians(delta.x) * 0.5f,
			glm::vec3(0, 1, 0));

	//g_Camera.Rotate( rotX * rotY );
	g_Rotation = (rotX * rotY) * g_Rotation;
}

#include "voronoi/Voronoi.h"
#include "voronoi/VPoint.h"

vor::Voronoi * v;
vor::Vertices * ver;
vor::Vertices * dir;
vor::Edges * edg;
double w = 50;

int main(int argc, char ** argv) {
	/* Esta función es llamada para evitar que truene el programa*/
	pthread_getconcurrency();

	v = new vor::Voronoi();
	ver = new vor::Vertices();

	std::vector<biorobotics::Polygon> polygons = biorobotics::parserFile(
			"filesWRL/test_world2.wrl");

	std::vector<biorobotics::Vertex2> points = biorobotics::computeCentroids(
			polygons);

	for (int i = 0; i < polygons.size(); i++) {
		biorobotics::Polygon polygon = polygons.at(i);
		for (int j = 0; j < polygon.num_vertex; j++) {
			VertexXYZColor vertexGL1;
			VertexXYZColor vertexGL2;
			biorobotics::Vertex2 vertex1 = polygon.vertex[j];
			biorobotics::Vertex2 vertex2;
			vertexGL1.m_Color = glm::vec3(0.0f, 0.0f, 1.0f);
			vertexGL1.m_Pos = glm::vec3(vertex1.x, vertex1.y, 0.0f);
			vertexPolygon.push_back(vertexGL1);
			if (j < polygon.num_vertex - 1)
				vertex2 = polygon.vertex[j + 1];
			else
				vertex2 = polygon.vertex[0];
			vertexGL2.m_Color = glm::vec3(0.0f, 0.0f, 1.0f);
			vertexGL2.m_Pos = glm::vec3(vertex2.x, vertex2.y, 0.0f);
			vertexPolygon.push_back(vertexGL2);
			//ver->push_back(new VPoint(vertex1.x, vertex1.y));
			//ver->push_back(new VPoint(vertex2.x, vertex2.y));
			//std::cout << vertex1.x << " " << vertex1.y << std::endl;
		}
	}
	for (int i = 0; i < polygons.size(); i++) {
		biorobotics::Polygon polygon = polygons.at(i);
		for (int j = 0; j < polygon.num_vertex; j++) {
			biorobotics::Vertex2 vertex1 = polygon.vertex[j];
			ver->push_back(new VPoint(vertex1.x, vertex1.y));
			std::cout << vertex1.x << " " << vertex1.y << std::endl;
		}
	}
	for (int j = 0; j < points.size(); j++) {
		VertexXYZColor vertexGL1;
		biorobotics::Vertex2 vertex1 = points[j];
		vertexGL1.m_Color = glm::vec3(0.0f, 1.0f, 1.0f);
		vertexGL1.m_Pos = glm::vec3(vertex1.x, vertex1.y, 0.0f);
		vertexCentroid.push_back(vertexGL1);
		/*ver->push_back(new VPoint(vertex1.x, vertex1.y));
		 std::cout << vertex1.x << " " << vertex1.y << std::endl;*/
	}

	edg = v->GetEdges(ver, w, w);

	for (vor::Edges::iterator i = edg->begin(); i != edg->end(); ++i) {
		VertexXYZColor vertexGL1;
		VertexXYZColor vertexGL2;
		/*vertexGL1.m_Color = glm::vec3(1.0f, 0.0f, 1.0f);
		 vertexGL1.m_Pos = glm::vec3(-1 + 2 * (*i)->start->x / w,
		 -1 + 2 * (*i)->start->y / w, 0.0f);
		 vertexGL2.m_Color = glm::vec3(1.0f, 0.0f, 1.0f);
		 vertexGL2.m_Pos = glm::vec3(-1 + 2 * (*i)->end->x / w,
		 -1 + 2 * (*i)->end->y / w, 0.0f);*/

		vertexGL1.m_Color = glm::vec3(1.0f, 0.0f, 1.0f);
		vertexGL1.m_Pos = glm::vec3((*i)->start->x, (*i)->start->y, 0.0f);
		vertexGL2.m_Color = glm::vec3(1.0f, 0.0f, 1.0f);
		vertexGL2.m_Pos = glm::vec3((*i)->end->x, (*i)->end->y, 0.0f);
		vertexDV.push_back(vertexGL1);
		vertexDV.push_back(vertexGL2);

	}

	/* Se obtiene la fecha actual del sistema*/
	g_PreviousTicks = std::clock();
	/* Estas variable almacenan el estatus de las teclas: 1 presionadas, 0 no presionadas*/
	g_A = g_W = g_S = g_D = g_Q = g_E = 0;

	/* La camara se inicia en la posición de 10 unidades en el eje positivo Z*/
	g_InitialCameraPosition = glm::vec3(0, 0, 20);
	g_Camera.SetPosition(g_InitialCameraPosition);
	g_Camera.SetRotation(g_InitialCameraRotation);
	/* Se inicializa GLUT y GLEW*/
	InitGL(argc, argv);
	InitGLEW();
// Carga los shaders
	GLuint vertexShader = LoadShader( GL_VERTEX_SHADER,
			"src/simpleShader.vert.glsl");
	GLuint fragmentShader = LoadShader( GL_FRAGMENT_SHADER,
			"src/simpleShader.frag.glsl");

	/* Se agregan los shaders al vector que despues se le pasará al shader program*/
	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	/* Se crea el program shader */
	g_ShaderProgram = CreateShaderProgram(shaders);
	/* Valida que la creación del program shader no haya tenido errores*/
	assert(g_ShaderProgram != 0);
	/* Se hace un query al shader program, para conocer el lugar del atributo para los dos
	 * atributos de vertices (posición y color), es decir consulta el location id por nombre
	 * del atributo de vertice
	 */
	GLint positionAtribID = glGetAttribLocation(g_ShaderProgram, "in_position");
	GLint colorAtribID = glGetAttribLocation(g_ShaderProgram, "in_color");
	/* Se utiliza de la misma forma el location id de la variable uniform*/
	g_uniformMVP = glGetUniformLocation(g_ShaderProgram, "MVP");

	/* Se crea un VAO para el cubo para encapsular los atributos de vertice de la geometría*/
	glGenVertexArrays(3, g_vaos);

	/* Se activa el VAO para el cubo*/
	glBindVertexArray(g_vaos[0]);
	/* Se crean los vertex Buffer object (VBO) para los vertices y los indices, el primer
	 * parametro corresponde a la cantidad de VBO que se necesitan
	 */

	glGenBuffers(3, g_vbos);

	/*Se activa el buffer de vertices e indica que se enlaza a un arreglo de vertices*/
	glBindBuffer( GL_ARRAY_BUFFER, g_vbos[0]);
	/* Se envia la información
	 * GL_ARRAY_BUFFER: es el target buffer object que se le indico en glBindBuffer
	 * sizeof(g_Vertices): Especifica el tamaño en bytes del buffer
	 * g_Vertices: Datos del buffer
	 * GL_STATIC_DRAW: indica que se carga una sola vez y se dibuja muchas veces
	 */
	glBufferData(GL_ARRAY_BUFFER, vertexPolygon.size() * sizeof(VertexXYZColor),
			vertexPolygon.data(),
			GL_STATIC_DRAW);

	/*
	 * Nuestros datos ya se encuentran en la memoria de OpenGL,
	 * todavía no se sabe qué tipo de dato se trata,
	 * para esto entra en juego la función glVertexAttribPointer, que proporciona como se van a utilizar
	 * los bloques de datos que han sido enviados a la GPU toma los siguientes parametros:
	 * positionAtribID : indice de posición del arreglo de atributos de vertices
	 * 3 : es el número de componentes por atributo de vertices
	 * GL_FLOAT : Tipo de dato
	 * GL_FALSE : Indica que los datos no serán normalizados
	 * sizeof(VertexXYZColor): indica el offset en bytes entre cada componente
	 *						   del arreglo de vertices.
	 * MEMBER_OFFSET(VertexXYZColor, m_Pos): Indica el offset del primer componente del atributo de vertice
	 */
	glVertexAttribPointer(positionAtribID, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Pos));
	/* Se activa el atributo de vertice*/
	glEnableVertexAttribArray(positionAtribID);
	/* Esto es muy parecido solo que el offset del atributo de color debe ser respecto al color*/
	glVertexAttribPointer(colorAtribID, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Color));
	glEnableVertexAttribArray(colorAtribID);

	glBindVertexArray(g_vaos[1]);
	glBindBuffer( GL_ARRAY_BUFFER, g_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER,
			vertexCentroid.size() * sizeof(VertexXYZColor),
			vertexCentroid.data(),
			GL_STATIC_DRAW);
	glVertexAttribPointer(positionAtribID, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Pos));
	/* Se activa el atributo de vertice*/
	glEnableVertexAttribArray(positionAtribID);
	/* Esto es muy parecido solo que el offset del atributo de color debe ser respecto al color*/
	glVertexAttribPointer(colorAtribID, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Color));
	glEnableVertexAttribArray(colorAtribID);

	glBindVertexArray(g_vaos[2]);
	glBindBuffer( GL_ARRAY_BUFFER, g_vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexDV.size() * sizeof(VertexXYZColor),
			vertexDV.data(),
			GL_STATIC_DRAW);
	glVertexAttribPointer(positionAtribID, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Pos));
	/* Se activa el atributo de vertice*/
	glEnableVertexAttribArray(positionAtribID);
	/* Esto es muy parecido solo que el offset del atributo de color debe ser respecto al color*/
	glVertexAttribPointer(colorAtribID, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Color));
	glEnableVertexAttribArray(colorAtribID);

// Make sure we disable and unbind everything to prevent rendering issues later.
	glBindVertexArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(positionAtribID);
	glDisableVertexAttribArray(colorAtribID);
	/* Se da inicio al loop del juego*/
	glutMainLoop();

	return 0;
}

