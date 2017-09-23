#include <string>
#include <iostream>
/*Esta cabecera se utiliza para leer del system file los shaders que necesitemis cargar*/
#include <fstream>
#include <vector>
/*Se utiliza para leer el reloj del sistema y determinar el tiempo que pasa entre cada frame*/
#include <ctime>

/*Esta definición de macro se utiliza cuando estamos ligando una libreria estatica
 La vamos a comentar ya que se esta ligando hacia librerias dinamicas*/
//#define GLEW_STATIC
/*Se carga la libreria glew se utiliza para cargar extensiones de OpenGL, se debe cargar antes de cargar
 *las librerias de freeglut
 */
#include <GL/glew.h>
/*Este include se requiera para utilizar la funcion llamada wglSwapInterval, la cual es una función
 *usara para desactivar vsync y permitir que los programas tan rapidos como puedan (En lugar de esperar
 *a una refresh de 60HZ, si no se desactivara vsync los programas nunca podran corren mas haya de la
 *velocidad de actualización del monitor), solo se usa para windows.
 */
//#include <GL/wglew.h> // For wglSwapInterval
/*Esta definición de macro se utiliza cuando estamos ligando una libreria estatica
 La vamos a comentar ya que se esta ligando hacia librerias dinamicas*/
//#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <pthread.h>

/*Se utiliza GLM_FORCE_RADIANS, macro que asegura que GLM use radianes y no grados*/
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
