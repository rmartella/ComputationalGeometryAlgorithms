#include "DataStructures/Tree/Avltree.h"
#include "DataStructures/Tree/Statustree.h"
#include "Util/UtilityIntersection.h"
#include "model/Segment.h"
#include "model/Result.h"
#include <cstdlib>
#include <stdio.h>
#include <vector>

/*
 #include "DataStructures/Tree/Node.h"
 #include "stddef.h"
 #include <iostream>
 */

using namespace std;

#define N 10

Avltree initiTreeEventPoints(float puntos[N][2][2]);
void procesaEvento(EventPoint event, Statustree *t, Avltree * eventTree);

/*float puntos[][2][2] = { { { 0, 0 }, { 3, 3 } }, { { 0, 4 }, { 3, 1 } }, { { 1,
 0 }, { 3, 2.0 } }, { { 2, 3 }, { 4, 2 } }, { { 3.5, 1 }, { 4, 4 } }, { {
 6, 4 }, { 5, 2 } }, { { 8, 1.5 }, { 5, 2.5 } } };*/
/*float puntos[][2][2] = { { { 0, 0 }, { 3, 3 } }, { { 0, 4 }, { 3, 1 } }, { { 1,
 0 }, { 3, 2.0 } }, { { 2, 3 }, { 4, 2 } }, { { 3.5, 1 }, { 4, 4 } }, { {
 6, 4 }, { 5, 2 } } };*/
/*float puntos[][2][2] = { { { 3, 2.0 }, { 3, 3 } }, { { 0, 4 }, { 3, 1 } }, { {
 1, 0 }, { 0, 0 } }, { { 2, 3 }, { 4, 2 } }, { { 3.5, 1 }, { 4, 4 } }, {
 { 6, 4 }, { 5, 2 } }, { { 8, 1.5 }, { 5, 2.5 } } };*/
float puntos[][2][2] = { { { 0, 0 }, { 2, 3 } }, { { 1, 3 }, { 3, 1 } }, { { -2,
		0.5 }, { -0.5, 2 } }, { { 4, 1 }, { 3, 3 } }, { { -3, 0 }, { 0, 3 } }, {
		{ 1, 0 }, { -2, 3 } }, { { 4, 0 }, { 5, 2.5 } },
		{ { 2, 1 }, { 3.5, 3 } }, { { 1.5, 1.5 }, { 5, 3 } }, { { -4, 4 }, { -1,
				1 } } };
//float puntos[][2][2] = { { { 1, 2 }, { 3, 0 } }, { { 5, 2 }, { 2, 1 } } };
UtilInt::UtilityIntersection util;

vector<Result::Result> resultado;

int main() {

	/*EventPoint event1;
	 EventPoint event2;
	 Point pointE1;
	 Point pointE2;
	 pointE1.setX(4.0);
	 pointE1.setY(3.0);
	 pointE2.setX(4.0);
	 pointE2.setY(3.0);
	 event1.setPointEvent(pointE1);
	 event2.setPointEvent(pointE2);
	 bool resulEqual = event1.isEqual(event2.getPointEvent());
	 bool resulLess = event1.isLess(event2.getPointEvent());
	 bool resulGreater = event1.isGreater(event2.getPointEvent());
	 cout << "resulEqual=" << resulEqual << ",resulLess=" << resulLess
	 << ",resulGreater=" << resulGreater << endl;*/

	/*Avltree t;
	 int pruebas = 15;

	 for (int i = 0; i < pruebas; i++) {
	 EventPoint event;
	 Point point;
	 point.setX(i / 1.0);
	 point.setY((pruebas - i) / 1.0);
	 event.setPointEvent(point);
	 t.insert(event);
	 }
	 EventPoint event;
	 Point point;
	 point.setX(50.0);
	 point.setY(5.0);
	 event.setPointEvent(point);
	 t.insert(event);
	 cout << endl;
	 t.inorder(t.root);
	 cout << endl;
	 t.preorder(t.root);
	 cout << endl;
	 t.postorder(t.root);*/

	/*EL BUENO*/

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

	/*Statustree status;
	 Point point1 = Point(0.0, 4.0);
	 Point point2 = Point(3.0, 1.0);
	 Segment segment = Segment(point1, point2);
	 status.insert(segment, point1);
	 point1 = Point(4.0, 4.0);
	 point2 = Point(3.5, 1.0);
	 segment = Segment(point1, point2);
	 status.insert(segment, point1);
	 point1 = Point(6.0, 4.0);
	 point2 = Point(5.0, 2.0);
	 segment = Segment(point1, point2);
	 status.insert(segment, point1);
	 point1 = Point(2.0, 3.0);
	 point2 = Point(4.0, 2.0);
	 segment = Segment(point1, point2);
	 status.insert(segment, point1);
	 point1 = Point(3.0, 3.0);
	 point2 = Point(0.0, 0.0);
	 segment = Segment(point1, point2);
	 status.insert(segment, point1);
	 status.inorder(status.root);
	 cout << endl;
	 status.preorder(status.root);
	 cout << endl;
	 status.postorder(status.root);
	 StatusNode ** neighborLeft = (StatusNode **) malloc(
	 size_t(sizeof(StatusNode *)));
	 StatusNode ** neighborRight = (StatusNode **) malloc(
	 size_t(sizeof(StatusNode *)));
	 point1 = Point(6.0, 4.0);
	 point2 = Point(5.0, 2.0);
	 status.getneighbors(status.root, Segment(point1, point2), neighborLeft,
	 neighborRight);
	 if (*neighborLeft != NULL) {
	 cout << "L Point(" << (*neighborLeft)->key.getPoint1().getX() << ","
	 << (*neighborLeft)->key.getPoint1().getY() << "),";
	 cout << "L Point(" << (*neighborLeft)->key.getPoint2().getX() << ","
	 << (*neighborLeft)->key.getPoint2().getY() << ")," << endl;
	 }
	 if (*neighborRight != NULL) {
	 cout << "R Point(" << (*neighborRight)->key.getPoint1().getX() << ","
	 << (*neighborRight)->key.getPoint1().getY() << "),";
	 cout << "R Point(" << (*neighborRight)->key.getPoint2().getX() << ","
	 << (*neighborRight)->key.getPoint2().getY() << ")," << endl;
	 }*/

}

Avltree initiTreeEventPoints(float puntos[][2][2]) {
	Avltree * tree = new Avltree();
	/*printf("Tamanio inicialización %d\n", N);*/
	for (unsigned int i = 0; i < N; i++) {
		EventPoint * eventPoint1 = (EventPoint *) malloc(
				size_t(sizeof(EventPoint)));
		EventPoint * eventPoint2 = (EventPoint *) malloc(
				size_t(sizeof(EventPoint)));
		Segment * segment = (Segment*) malloc(size_t(sizeof(Segment)));
		Point point1 = Point(puntos[i][0][0], puntos[i][0][1]);
		Point point2 = Point(puntos[i][1][0], puntos[i][1][1]);
		/*printf("puntos P(%f,%f)\n", puntos[i][0][0], puntos[i][0][1]);
		 printf("puntos P(%f,%f)\n", puntos[i][1][0], puntos[i][1][1]);*/
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
		/*printf("eventPoint1 P(%f,%f)\n", eventPoint1->getPointEvent().getX(),
		 eventPoint1->getPointEvent().getY());
		 printf("eventPoint2 P(%f,%f)\n\n", eventPoint2->getPointEvent().getX(),
		 eventPoint2->getPointEvent().getY());*/
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
