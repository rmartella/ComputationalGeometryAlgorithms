#include "create_map.h"

typedef struct _nodo {
	int poligono;
	int vertice;
	int * anterioresPoligono;
	int * anterioresVertice;
	int anteriores;
} Nodo;

typedef struct _nodoPila {
	Nodo nodo;
	struct _nodoPila *siguiente;
} noto_pila;

typedef noto_pila * ptrNodo;
typedef noto_pila * ptrPila;

void push(ptrPila *pila, Nodo x);
Nodo pop(ptrPila *pila);

Nodo firstDepth(Vertex inicio, Vertex fin, int num_polygons, Polygon * polygons,
		Polygon * grownPolygons, bool ** adyacencias, int * sizeListaExpacion,
		float ratio);
void agregaNodosExpacion(Nodo nodo, int num_polygons, bool ** adyacencias,
		Nodo * listaExpacion, int * sizeListaExpacion, ptrPila * pila);
bool decideExpandir(Nodo nodo, Nodo * listaExpacion, int sizeListaExpacion);

Nodo firstDepth(Vertex inicio, Vertex fin, int num_polygons, Polygon * polygons,
		Polygon * grownPolygons, bool ** adyacencias, int * sizeListaExpacion,
		float ratio) {

	Nodo * listaExpacion = (Nodo *) malloc(sizeof(Nodo));
	/*sizeListaExpacion = (int *) malloc(sizeof(int));*/

	Nodo nodo = Nodo { 0, 0, (int *) malloc(sizeof(int)), (int *) malloc(
			sizeof(int)), 0 };
	ptrPila pila = NULL;
	*sizeListaExpacion = 0;
	listaExpacion[0] = nodo;
	*sizeListaExpacion = 1;
	addNodesInitEndToMap(inicio, fin, num_polygons, polygons, grownPolygons,
			adyacencias, ratio);
	push(&pila, nodo);
	nodo = pop(&pila);
	while (nodo.poligono != num_polygons) {
		agregaNodosExpacion(nodo, num_polygons, adyacencias, listaExpacion,
				sizeListaExpacion, &pila);
		nodo = pop(&pila);
		while (!decideExpandir(nodo, listaExpacion, *sizeListaExpacion)) {
			nodo = pop(&pila);
		}
		listaExpacion = (Nodo *) realloc(listaExpacion,
				sizeof(Nodo) * (*sizeListaExpacion + 1));
		listaExpacion[*sizeListaExpacion] = nodo;
		*sizeListaExpacion = *sizeListaExpacion + 1;
	}
	printf("Entro aqui\n");
	return nodo;
}

void agregaNodosExpacion(Nodo nodo, int num_polygons, bool ** adyacencias,
		Nodo * listaExpacion, int * sizeListaExpacion, ptrPila * pila) {
	bool * expancion;
	int indiceVertice = nodo.vertice;
	int indicePoligono = nodo.poligono;
	if (nodo.poligono == 0) {
		indiceVertice = 1;
		indicePoligono = 0;
	}
	expancion = adyacencias[(indicePoligono - 0) * NUM_MAX_VERTEX
			+ indiceVertice - 1];
	for (int i = 1; i <= num_polygons; i++) {
		for (int j = 1; j <= NUM_MAX_VERTEX; j++) {
			if (expancion[(i - 0) * NUM_MAX_VERTEX + j - 1]) {
				int * anterioresPoligono = (int *) malloc(
						sizeof(int) * (nodo.anteriores + 1));
				anterioresPoligono[nodo.anteriores] = nodo.poligono;
				int * anterioresVertice = (int *) malloc(
						sizeof(int) * (nodo.anteriores + 1));
				anterioresVertice[nodo.anteriores] = nodo.vertice;
				memcpy(anterioresPoligono, nodo.anterioresPoligono,
						nodo.anteriores * sizeof(int));
				memcpy(anterioresVertice, nodo.anterioresVertice,
						nodo.anteriores * sizeof(int));
				Nodo nuevoNodo = { i, j, anterioresPoligono, anterioresVertice,
						nodo.anteriores + 1 };
				push(pila, nuevoNodo);
			}
		}

	}
}

bool decideExpandir(Nodo nodo, Nodo * listaExpacion, int sizeListaExpacion) {
	bool flagExpacion = true;
	for (int i = 0; i < sizeListaExpacion && flagExpacion; i++) {
		if (nodo.poligono == listaExpacion[i].poligono
				&& nodo.vertice == listaExpacion[i].vertice) {
			flagExpacion = false;
		}
	}
	return flagExpacion;
}

/*
 Agrega un nodo al inicio de la lista ligada
 *pila es el apuntador que apunta al primer nodo de la lista ligada (la cima de la pila)
 */
void push(ptrPila *pila, Nodo nodo) {
// Crea un nuevo nodo
	ptrNodo nodoPila;
	nodoPila = (ptrNodo) malloc(sizeof(noto_pila));
	if (nodoPila != NULL) {
		nodoPila->nodo = nodo;
		// El apuntador nodo->siguiente va a apuntar al primer nodo de la lista ligada
		nodoPila->siguiente = *pila;
		// pila va a apuntar al nuevo nodo, con esto hacemos que el nuevo nodo sea ahora el primer nodo de la lista ligada
		*pila = nodoPila;
	}
}

/*
 Elimina el primer nodo de la lista ligada
 *pila es el apuntador que apunta al primer nodo de la lista ligada (la cima de la pila)
 */
Nodo pop(ptrPila *pila) {
// Crea un nuevo nodo
	ptrNodo nodoPila;
	Nodo nodo;

// El nuevo nodo va a apuntar al primer nodo de la lista ligada
	nodoPila = *pila;
	nodo.poligono = (*pila)->nodo.poligono;
	nodo.vertice = (*pila)->nodo.vertice;
	nodo.anterioresPoligono = (*pila)->nodo.anterioresPoligono;
	nodo.anterioresVertice = (*pila)->nodo.anterioresVertice;
	nodo.anteriores = (*pila)->nodo.anteriores;
	/*punto.x = (*pila)->punto.x;
	 punto.y = (*pila)->punto.y;*/

// Ahora el segundo nodo de la lista ligada va a ser el primero
	*pila = (*pila)->siguiente;
// Borra el primer nodo de la lista ligada
	free(nodoPila);
// Regresa el valor que contenía el nodo que se eliminó
	return nodo;
}
