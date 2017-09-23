/*
 * ConvexHull.cpp
 *
 *  Created on: 01/04/2015
 *      Author: rey
 */

#include "ConvexHull.h"

namespace ConvexHull {

ConvexHull::ConvexHull() {
	// TODO Auto-generated constructor stub

}

ConvexHull::ConvexHull(std::vector<Punto> points) {
	this->points = points;
}

ConvexHull::~ConvexHull() {
	// TODO Auto-generated destructor stub
}

/*Este método sirve para calcular el cierre convexo de un conunto de puntos,
 * regresa el apuntador de apuntadores a la lista de puntos, el conjunto de puntos
 * se le pasa al constructor de la clase, y el algortimo actuará respecto a esta lista de puntos.
 * La complejidad para este algortimo es O(nlogn), ya que su cota minima se reduce al problema de ordenación,
 * El método de ordenación es el algortimo quicksort el cual en promedio tiene un costo de O(nlogn),
 * lo cúal para el algortimo del cierre convexo se revisan dos veces a lo mas los vertices (uno parte izquierda,
 * y el otro parte derecha).
 */
ptrPila ConvexHull::calConvexHull() {
	int i = 0;
	ptrPila pila = NULL;

	//float * min = *puntos;
	Punto * min = &points[i];
	Punto * puntosEspacio = (Punto *) malloc(sizeof(Punto) * points.size());
	Punto puntoMinimo;
	Punto puntoMax;
	//puntosEspacio->x = (*(*puntos + 0));
	//puntosEspacio->y = (*(*puntos + 1));
	puntosEspacio->x = points[i].x;
	puntosEspacio->y = points[i].y;

	for (unsigned int i = 1; i < points.size(); i++) {
		(puntosEspacio + i)->x = points[i].x;
		(puntosEspacio + i)->y = points[i].y;
		//(puntosEspacio + i)->x = *(*(puntos + i) + 0);
		//(puntosEspacio + i)->y = *(*(puntos + i) + 1);
		//if ((*(*(puntos + i) + 1)) <= *(min + 1)) {
		if (points[i].y <= min->y) {
			//if ((*(*(puntos + i) + 1)) == *(min + 1)) {
			if (points[i].y == min->y) {
				if (points[i].x < min->x) {
					min = &points[i];
				}
			} else {
				min = &points[i];
			}
		}
	}

	puntoMinimo = *min;
	//printf("Punto minimo == P(%f,%f)\n", puntoMinimo.x, puntoMinimo.y);

	quicksort(puntosEspacio, 0, points.size() - 1);

	Punto *p = &puntoMinimo;
	Punto *q = puntosEspacio + 1;
	Punto *r;
	int limite = points.size();
	int signo = 1;
	int limitSigno;
	i = 2;

	for (; i <= limite; i++) {
		if (i == points.size()) {
			//printf("Mitad de Cierre convexo\n");
			//nodos_pila(pila);
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
				//printf("Determinante %f \n", det);
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
	printf("\n");

	return pila;
}

} /* namespace UtilInt */
