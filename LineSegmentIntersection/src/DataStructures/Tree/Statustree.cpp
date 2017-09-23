/*
 * Statustree.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Statustree.h"
#include <cstdlib>

using namespace std;

Statustree::~Statustree() {
	// TODO Auto-generated destructor stub
}

Statustree::Statustree() {
	root = NULL;
	ac = 0;
}

void Statustree::insert(Segment key, Point point) {
	StatusNode *t = root;
	StatusNode *p = new StatusNode(key);
	this->point = point;
	if (root == NULL) {
		root = p;
		return;
	}
	while (1) {
		if (key.isEqual(t->key))
			//if (key == t->data)
			return;
		if (key.isGreater(t->key, this->point) && t->right == NULL) {
			//if (key > t->data && t->right == NULL) {
			t->right = p;
			break;
		}
		if (key.isLess(t->key, this->point) && t->left == NULL) {
			//if (key < t->data && t->left == NULL) {
			t->left = p;
			break;
		}
		if (key.isLess(t->key, this->point)) {
			//if (key < t->data) {
			t = t->left;
		} else
			t = t->right;
	}
	/*Balanceando*/
	t = getnode(key);
	while (t != NULL) {
		int hd = balance(t);
		if (fabs(hd) > 1) {
			if (hd < 0) {
				if (balance(t->left) < 0) {
					rightrotate(t);
				} else {
					leftrotate(t->left);
					rightrotate(t);
				}
			} else {
				if (balance(t->left) > 0) {
					leftrotate(t);
				} else {
					rightrotate(t->right);
					leftrotate(t);
				}
			}
			break;
		}
		t = getparent(t->key);
	}
}

StatusNode * Statustree::getinordersuccessor(StatusNode *t) {
	t = t->right;
	while (t->left != NULL)
		t = t->left;
	return t;
}

void Statustree::remove(Segment key, Point point) {
	StatusNode *t = getnode(key);
	this->point = point;
	if (t == NULL)
		return;
	StatusNode *q = getparent(key);
	if (t->left != NULL && t->right != NULL) {
		/*Para reemplazo con el sucesor inorder*/
		StatusNode *y = getinordersuccessor(t);
		q = getparent(y->key);
		t->key = y->key;
		t = y;
	}
	StatusNode *p = NULL;
	if (q == NULL && t != root)
		return;
	int rootchild = (q != NULL && q == root) ? 1 : 0;
	int isroot = (t == root) ? 1 : 0;
	int i = (q != NULL && q->left == t) ? 0 : 1;
	if (t->left == NULL && t->right == NULL) {
		if (!isroot) {
			if (!i)
				q->left = NULL;
			else
				q->right = NULL;
			delete t;
			p = q;
		} else {
			root = NULL;
			delete t;
		}
	} else if (t->left == NULL) {
		if (!isroot) {
			if (!i)
				q->left = t->right;
			else
				q->right = t->right;
			delete t;
			p = q;
		} else {
			root = t->right;
			delete t;
		}
	} else if (t->right == NULL) {
		if (!isroot) {
			if (!i)
				q->left = t->left;
			else
				q->right = t->left;
			delete t;
			p = q;
		} else {
			root = t->left;
			delete t;
		}
	}
	if (rootchild)
		root = p;
	/*Balanceando parte*/
	t = p;
	while (t != NULL) {
		int hd = balance(t);
		if (fabs(hd) > 1) {
			if (hd < 0) {
				if (balance(t->left) < 0) {
					rightrotate(t);
				} else {
					leftrotate(t->left);
					rightrotate(t);
				}
			} else {
				if (balance(t->left) > 0) {
					leftrotate(t);
				} else {
					rightrotate(t->right);
					leftrotate(t);
				}
			}
		}
		t = getparent(t->key);
	}
}

int Statustree::height(StatusNode *t) {
	if (t == NULL)
		return 0;
	int hr = height(t->right);
	int hl = height(t->left);
	return 1 + (hr > hl ? hr : hl);
}

int Statustree::balance(StatusNode *t) {
	if (t == NULL)
		return 0;
	return height(t->right) - height(t->left);
}

StatusNode * Statustree::getparent(Segment key) {
	StatusNode *t = getnode(key);
	if (t == NULL || t == root)
		return NULL;
	StatusNode *t1 = root;
	while (1) {
		if (t1 == NULL)
			return NULL;
		//if (t->data > t1->data && t->data == t1->right->data)
		if (t->key.isGreater(t1->key, this->point)
				&& t->key.isEqual(t1->right->key))
			return t1;
		//if (t->data < t1->data && t->data == t1->left->data)
		if (t->key.isLess(t1->key, this->point)
				&& t->key.isEqual(t1->left->key))
			return t1;
		//if (t->data > t1->data)
		if (t->key.isGreater(t1->key, this->point))
			t1 = t1->right;
		else
			t1 = t1->left;
	}
}

void Statustree::rightrotate(StatusNode *t) {
	if (t == NULL)
		return;
	StatusNode *q = getparent(t->key);
	int i, j = 0;
	if (t == root)
		j++;
	if (q != NULL && q->right == t) {
		i = 1;
	} else {
		i = 0;
	}
	StatusNode *u = t->left;
	if (u == NULL)
		return;
	//StatusNode *v=t->right;
	t->left = u->right;
	u->right = t;
	if (q == NULL) {
		root = u;
		return;
	}
	if (i)
		q->right = u;
	else
		q->left = u;
}

void Statustree::leftrotate(StatusNode *t) {
	if (t == NULL)
		return;
	StatusNode *q = getparent(t->key);
	int i, j = 0;
	if (t == root)
		j++;
	if (q != NULL && q->right == t) {
		i = 1;
	} else {
		i = 0;
	}
	StatusNode *u = t->right;
	if (u == NULL)
		return;
	//StatusNode *v=t->left;
	t->right = u->left;
	u->left = t;
	if (q == NULL) {
		root = u;
		return;
	}
	if (i)
		q->right = u;
	else
		q->left = u;
}

StatusNode * Statustree::getnode(Segment key) {
	StatusNode *t = root;
	while (1) {
		if (t == NULL)
			return NULL;
		//if (t->data == key)
		if (t->key.isEqual(key))
			return t;
		//if (key > t->data)
		if (key.isGreater(t->key, this->point))
			t = t->right;
		else
			t = t->left;
	}
}

void Statustree::inorder(StatusNode *t) {
	if (t == NULL)
		return;
	inorder(t->left);
	cout << "P1 (" << t->key.getPoint1().getX() << ","
			<< t->key.getPoint1().getY() << "),";
	cout << "P2 (" << t->key.getPoint2().getX() << ","
			<< t->key.getPoint2().getY() << ")" << endl;
	inorder(t->right);
}

void Statustree::preorder(StatusNode *t) {
	if (t == NULL)
		return;
	cout << "P1 (" << t->key.getPoint1().getX() << ","
			<< t->key.getPoint1().getY() << "),";
	cout << "P2 (" << t->key.getPoint2().getX() << ","
			<< t->key.getPoint2().getY() << ")" << endl;
	preorder(t->left);
	preorder(t->right);
}

void Statustree::postorder(StatusNode *t) {
	if (t == NULL)
		return;
	postorder(t->left);
	postorder(t->right);
	cout << "P1 (" << t->key.getPoint1().getX() << ","
			<< t->key.getPoint1().getY() << "),";
	cout << "P2 (" << t->key.getPoint2().getX() << ","
			<< t->key.getPoint2().getY() << ")" << endl;
}

StatusNode * Statustree::getMax(StatusNode *ta) {
	if (ta == NULL)
		return NULL;
	StatusNode * node = NULL;
	node = getMax(ta->right);
	if (node == NULL) {
		node = ta;
	}
	return node;
}

/*void Statustree::getneighbors(StatusNode * t, Point key, StatusNode * status,
 int *i, int *j) {
 if (t == NULL)
 return;
 getneighbors(t->left, key, status, i, j);
 status[*i] = *t;
 status = (StatusNode *) realloc(status, size_t(sizeof(StatusNode)));
 if (t->key.isEqual(key)) {
 *j = *i;
 }
 *i = *i + 1;
 getneighbors(t->right, key, status, i, j);
 }*/

StatusNode * Statustree::getMin(StatusNode *ta) {
	if (ta == NULL)
		return NULL;
	StatusNode * node = NULL;
	node = getMin(ta->left);
	if (node == NULL) {
		node = ta;
	}
	return node;
}

StatusNode * Statustree::getCambioDeGiroTree(StatusNode * node, int tipoGiro) {
	bool cambioGiro = false;
	StatusNode * parent;
	StatusNode * nodeTmp = node;
	StatusNode * nodeResult;
	while (!cambioGiro) {
		parent = getparent(nodeTmp->key);
		if (parent == NULL) {
			nodeResult = parent;
			cambioGiro = true;
		} else {
			if (tipoGiro == 1) {
				if (parent->left->key.isEqual(nodeTmp->key))
					nodeTmp = parent;
				else {
					nodeResult = parent;
					cambioGiro = true;
				}
			} else {
				if (parent->right->key.isEqual(nodeTmp->key))
					nodeTmp = parent;
				else {
					nodeResult = parent;
					cambioGiro = true;
				}
			}
		}
	}
	return nodeResult;
}

void Statustree::getneighbors(StatusNode * t, Segment key,
		StatusNode ** neighborLeft, StatusNode ** neighborRight) {
	cout << "Line search point (" << point.getX() << "," << point.getY() << ")"
			<< endl;
	StatusNode * node = getnode(key);
	if (node->left == NULL && node->right == NULL) {
		StatusNode * parent = getparent(key);
		if (parent != NULL) {
			StatusNode * grandparent = getparent(parent->key);
			if (parent->left != NULL && parent->left->key.isEqual(key)) {
				if (grandparent != NULL) { // X X
					*neighborLeft = getCambioDeGiroTree(parent, 1);
					*neighborRight = parent;
				} else { // *
					*neighborLeft = NULL;
					*neighborRight = parent;
				}
			} else {
				if (grandparent != NULL) { // X X
					*neighborRight = getCambioDeGiroTree(parent, 2);
					*neighborLeft = parent;
				} else { // *
					*neighborRight = NULL;
					*neighborLeft = parent;
				}
			}
		} else { //*
			*neighborRight = NULL;
			*neighborLeft = NULL;
		}
	} else {
		if (node->right != NULL)
			*neighborRight = getMin(node->right);
		else {
			StatusNode * parent = getparent(key);
			if (parent != NULL) {
				StatusNode * grandparent = getparent(parent->key);
				if (parent->left != NULL && parent->left->key.isEqual(key)) {
					*neighborRight = parent;
				} else {
					if (grandparent != NULL)
						*neighborRight = getCambioDeGiroTree(parent, 2);
					else
						*neighborRight = NULL;
				}

				/*if (grandparent != NULL)
				 *neighborRight = grandparent;
				 else
				 *neighborRight = parent;*/
			} else
				*neighborRight = NULL;
		}
		if (node->left != NULL)
			*neighborLeft = getMax(node->left);
		else
			*neighborLeft = NULL;
	}
}
