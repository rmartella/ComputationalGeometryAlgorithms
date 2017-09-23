#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 100

typedef struct _punto {
	float x, y;
} Punto;

typedef struct _nodo {
	Punto punto;
	struct _nodo *siguiente;
} noto_t;

typedef noto_t * ptrNodo;
typedef noto_t * ptrPila;

void push(ptrPila *pila, Punto x);
Punto pop(ptrPila *pila);
void quicksort(Punto *array, int start, int end);
float calculaDeterminante(Punto p, Punto q, Punto r);
int pila_vacia(ptrPila *pila);
void nodos_pila(ptrNodo nodo);

int main(int argc, char** argv) {

	ptrPila pila = NULL;

	float puntos[N][2] = { { 19, -25 }, { 30, 93 }, { 38, -87 }, { 33, -13 }, {
			-45, 90 }, { 61, 71 }, { 40, 70 }, { 20, -25 }, { 44, 82 }, { -73,
			48 }, { 73, -73 }, { -35, -18 }, { 36, -96 }, { 1, 25 }, { 90, 22 },
			{ -82, 5 }, { -77, 5 }, { -45, 92 }, { -8, -23 }, { 36, -24 }, {
					-19, 22 }, { 98, -70 }, { 44, 71 }, { 8, -93 }, { -65, 51 },
			{ 93, -90 }, { -2, 53 }, { 51, 14 }, { 30, 56 }, { 16, 79 }, { -48,
					41 }, { 37, -21 }, { 100, 12 }, { -34, 95 }, { -98, -34 }, {
					-79, -57 }, { 32, 19 }, { -82, 73 }, { 56, -74 },
			{ 77, -36 }, { -66, -17 }, { 100, -40 }, { 82, 46 }, { 85, -2 }, {
					-94, -74 }, { -93, 93 }, { -52, -59 }, { 71, -36 }, { -43,
					-79 }, { 76, -57 }, { -3, 17 }, { -64, 7 }, { 33, -22 }, {
					-89, 34 }, { 27, -22 }, { -75, 64 }, { 25, -60 },
			{ -50, -71 }, { 54, 87 }, { -30, 62 }, { -47, 2 }, { 32, 28 }, { 90,
					-36 }, { 71, 24 }, { -11, -69 }, { 27, -40 }, { -61, 71 }, {
					-62, -2 }, { -92, -16 }, { -29, 7 }, { -82, -83 },
			{ 40, -81 }, { -10, 47 }, { 67, -72 }, { 56, -82 }, { -84, 13 }, {
					4, 70 }, { -50, 75 }, { -9, -80 }, { -55, 56 }, { -79, -7 },
			{ 25, -93 }, { -43, -16 }, { -10, -4 }, { 97, -5 }, { 2, -33 }, {
					68, 47 }, { -89, -52 }, { -28, 54 }, { 28, -20 },
			{ 57, -42 }, { -6, 4 }, { 22, -56 }, { 18, 31 }, { 21, -41 }, { -63,
					3 }, { 35, 74 }, { 49, -18 }, { -22, -8 }, { 79, 45 } };

	/*float puntos[N][2] = { { 0, 5 }, { 1, 7 }, { 2, 8 }, { 3, 7 }, { 4, 7 }, {
	 5, 5 }, { 4, 4 }, { 3, 2 }, { 2, 3 }, { 1, 3 }, { 2.5, 6 },
	 { 3.5, 6 }, { 1.5, 6 } };*/

	int i = 0;

	float * min = *puntos;
	Punto * puntosEspacio = (Punto *) malloc(sizeof(Punto) * N);
	Punto puntoMinimo;
	Punto puntoMax;
	puntosEspacio->x = (*(*puntos + 0));
	puntosEspacio->y = (*(*puntos + 1));

	for (int i = 1; i < N; i++) {
		(puntosEspacio + i)->x = *(*(puntos + i) + 0);
		(puntosEspacio + i)->y = *(*(puntos + i) + 1);
		if ((*(*(puntos + i) + 1)) <= *(min + 1)) {
			if ((*(*(puntos + i) + 1)) == *(min + 1)) {
				if (*(*(puntos + i)) < *min) {
					min = *(puntos + i);
				}
			} else {
				min = *(puntos + i);
			}
		}
	}

	puntoMinimo.x = *min;
	puntoMinimo.y = *(min + 1);
	printf("Punto minimo == P(%f,%f)\n", puntoMinimo.x, puntoMinimo.y);

	quicksort(puntosEspacio, 0, N - 1);

	Punto *p = &puntoMinimo;
	Punto *q = puntosEspacio + 1;
	Punto *r;
	int limite = N;
	int signo = 1;
	int limitSigno;
	i = 2;

	for (; i <= limite; i++) {
		if (i == N) {
			printf("Mitad de Cierre convexo\n");
			nodos_pila(pila);
			signo = -1;
			//i -= 2;
			i *= signo;
			limite = 0;
			Punto pin = pop(&pila);
			Punto qin = pop(&pila);
			q = &pin;
			p = &qin;
			push(&pila, *p);
			push(&pila, *q);
			i++;
			limitSigno = q->x;
			puntoMax = *q;
		}
		r = puntosEspacio + i * signo;

		if ((signo == -1 && r->x < limitSigno) || signo == 1
				|| (signo == -1
						&& calculaDeterminante(puntoMinimo, puntoMax, *r) >= 0
						&& r->y != puntoMax.y && r->x != puntoMax.x)) {
			float det;
			int vacia = 0;
			do {
				vacia = 0;
				det = calculaDeterminante(*p, *q, *r);
				printf("Determinante %f \n", det);
				if ((det == 0.0 && (q->y == r->y && q->x < r->x))
						|| det < 0.0) {
					if (pila_vacia(&pila) == 1) {
						push(&pila, *p);
						push(&pila, *r);
						q = r;
						vacia = 1;
					} else {
						Punto punto1;
						Punto punto2;
						pop(&pila);
						punto1 = pop(&pila);
						if (pila_vacia(&pila) != 1) {
							punto2 = pop(&pila);
							q = &punto1;
							p = &punto2;
						} else {
							p = &punto1;
							q = r;
						}
						push(&pila, *p);
						push(&pila, *q);
						if (det == 0) {
							det = -1;
						}
					}

				} else {
					if (pila_vacia(&pila) == 1) {
						push(&pila, *p);
						push(&pila, *q);
						push(&pila, *r);
						p = q;
						q = r;
					} else {
						if (q != r) {
							if (r->x != puntoMinimo.x
									&& r->y != puntoMinimo.y) {
								push(&pila, *r);
								p = q;
								q = r;
							}
						}
					}
				}
			} while (det < 0 && vacia != 1);
		}

	}

	printf("Cierre convexo\n");
	nodos_pila(pila);

	return 0;

}

void nodos_pila(ptrNodo nodo) {
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
int divide(Punto *array, int start, int end) {
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
void quicksort(Punto * array, int start, int end) {
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
void push(ptrPila *pila, Punto x) {
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
 Elimina el primer nodo de la lista ligada
 *pila es el apuntador que apunta al primer nodo de la lista ligada (la cima de la pila)
 */
Punto pop(ptrPila *pila) {
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

/*
 Regresa 1 si no hay nodos en la lista ligada y cero en caso contrario
 *pila es el apuntador que apunta al primer nodo de la lista ligada (la cima de la pila)
 */
int pila_vacia(ptrPila *pila) {
	return (*pila == NULL ? 1 : 0);
}

float calculaDeterminante(Punto p, Punto q, Punto r) {
	float determinante = (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
	return determinante;
}
