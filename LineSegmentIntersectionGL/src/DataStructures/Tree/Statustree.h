/*
 * Avltree.h
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */
#include "StatusNode.h"
#include "stddef.h"
#include <iostream>
#include <math.h>

#ifndef SRC_Statustree_H_
#define SRC_Statustree_H_

using namespace std;

class Statustree {
public:
	Statustree();
	virtual ~Statustree();
	StatusNode *root;
	void insert(Segment segment, Point point);
	StatusNode *getinordersuccessor(StatusNode *);
	void remove(Segment key, Point point);
	int height(StatusNode *);
	int balance(StatusNode *);
	StatusNode *getparent(Segment key);
	void rightrotate(StatusNode *);
	void leftrotate(StatusNode *);
	StatusNode *getnode(Segment key);
	void inorder(StatusNode *);
	void preorder(StatusNode *);
	void postorder(StatusNode *);
	StatusNode * getMax(StatusNode *);
	StatusNode * getMin(StatusNode *ta);
	void changeNode(Segment key1, Segment key2);
	StatusNode * getCambioDeGiroTree(StatusNode * node, int tipoGiro);
	void getneighbors(StatusNode * t, Segment key, StatusNode ** neighborLeft,
			StatusNode ** neighborRight);
	Point point;
private:
	int ac;
};

#endif /* Statustree */
