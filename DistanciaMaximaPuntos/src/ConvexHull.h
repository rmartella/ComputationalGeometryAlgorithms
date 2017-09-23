/*
 * ConvexHull.h
 *
 *  Created on: 01/04/2015
 *      Author: rey
 */
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef CONVEXHULL_H_
#define CONVEXHULL_H_

namespace ConvexHull {

typedef struct _punto {
	float x, y;
} Punto;

typedef struct _nodo {
	Punto punto;
	struct _nodo *siguiente;
} noto_t;

typedef noto_t * ptrNodo;
typedef noto_t * ptrPila;

class ConvexHull {
public:
	ConvexHull();
	ConvexHull(std::vector<Punto> points);
	virtual ~ConvexHull();
	void setPoints(std::vector<Punto> points) {
		this->points = points;
	}
	std::vector<Punto> getPoints() {
		return this->points;
	}
	ptrPila calConvexHull();
	/*
	 Elimina el primer nodo de la lista ligada
	 *pila es el apuntador que apunta al primer nodo de la lista ligada (la cima de la pila)
	 */
	static Punto pop(ptrPila *pila) {
		// Crea un nuevo nodo
		ptrNodo nodo;
		Punto punto;

		// El nuevo nodo va a apuntar al primer nodo de la lista ligada
		nodo = *pila;
		punto.x = (*pila)->punto.x;
		punto.y = (*pila)->punto.y;

		// Ahora el segundo nodo de la lista ligada va a ser el primero
		*pila = (*pila)->siguiente;
		// Borra el primer nodo de la lista ligada
		free(nodo);
		// Regresa el valor que contenía el nodo que se eliminó
		return punto;
	}
	static void nodos_pila(ptrNodo nodo) {
		if (nodo == NULL)
			printf("La pila está vacia\n");
		else {
			while (nodo != NULL) {
				printf("P(%f,%f)\n", nodo->punto.x, nodo->punto.y);
				nodo = nodo->siguiente;
			}
		}
	}

	// Función para dividir el array y hacer los intercambios
	static int divide(Punto *array, int start, int end) {
		int left;
		int right;
		float pivot;

		Punto temp;

		pivot = (array + start)->y;
		left = start;
		right = end;

		// Mientras no se cruzen los índices
		while (left < right) {
			while ((array + right)->y > pivot) {
				right--;
			}

			while ((left < right) && ((array + left)->y <= pivot)) {
				left++;
			}

			// Si todavía no se cruzan los indices seguimos intercambiando
			if (left < right) {
				temp.x = (array + left)->x;
				temp.y = (array + left)->y;
				(array + left)->x = (array + right)->x;
				(array + left)->y = (array + right)->y;
				(array + right)->x = temp.x;
				(array + right)->y = temp.y;
			}
		}

		// Los índices ya se han cruzado, ponemos el pivot en el lugar que le corresponde
		temp.x = (array + right)->x;
		temp.y = (array + right)->y;
		(array + right)->x = (array + start)->x;
		(array + right)->y = (array + start)->y;
		(array + start)->x = temp.x;
		(array + start)->y = temp.y;

		// La nueva posición del pivot
		return right;
	}

	// Función recursiva para hacer el ordenamiento
	static void quicksort(Punto * array, int start, int end) {
		float pivot;

		if (start < end) {
			pivot = divide(array, start, end);

			// Ordeno la lista de los menores
			quicksort(array, start, pivot - 1);

			// Ordeno la lista de los mayores
			quicksort(array, pivot + 1, end);
		}
	}

	/*
	 Agrega un nodo al inicio de la lista ligada
	 *pila es el apuntador que apunta al primer nodo de la lista ligada (la cima de la pila)
	 */
	static void push(ptrPila *pila, Punto x) {
		// Crea un nuevo nodo
		ptrNodo nodo;
		nodo = (ptrNodo) malloc(sizeof(noto_t));
		if (nodo != NULL) {
			nodo->punto = x;
			// El apuntador nodo->siguiente va a apuntar al primer nodo de la lista ligada
			nodo->siguiente = *pila;
			// pila va a apuntar al nuevo nodo, con esto hacemos que el nuevo nodo sea ahora el primer nodo de la lista ligada
			*pila = nodo;
		}
	}

	/*
	 Regresa 1 si no hay nodos en la lista ligada y cero en caso contrario
	 *pila es el apuntador que apunta al primer nodo de la lista ligada (la cima de la pila)
	 */
	static int pila_vacia(ptrPila *pila) {
		return (*pila == NULL ? 1 : 0);
	}

	static float calculaDeterminante(Punto p, Punto q, Punto r) {
		float determinante = (q.x - p.x) * (r.y - p.y)
				- (q.y - p.y) * (r.x - p.x);
		return determinante;
	}
private:
	std::vector<Punto> points;
};

} /* namespace UtilInt */

#endif /* CONVEXHULL_H_ */
