/*
 * Avltree.h
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */
#include "Node.h"
#include "stddef.h"
#include <iostream>
#include <math.h>

#ifndef SRC_AVLTREE_H_
#define SRC_AVLTREE_H_

using namespace std;

class Avltree {
public:
	Avltree();
	virtual ~Avltree();
	Node *root;
	void insert(int);
	Node *getinordersuccessor(Node *);
	void remove(int);
	int height(Node *);
	int balance(Node *);
	Node *getparent(int);
	void rightrotate(Node *);
	void leftrotate(Node *);
	Node *getnode(int);
	void inorder(Node *);
	void preorder(Node *);
	void postorder(Node *);
private:
	Node *arr[100];
	int ac;
};

#endif /* SRC_AVLTREE_H_ */
