/*
 * Avltree.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Avltree.h"
#include <cstdlib>

using namespace std;

Avltree::~Avltree() {
	// TODO Auto-generated destructor stub
}

Avltree::Avltree() {
	root = NULL;
	ac = 0;
}

void Avltree::insert(EventPoint event) {
	Node *t = root;
	Node *p = new Node(event);
	if (root == NULL) {
		root = p;
		return;
	}
	while (1) {
		//if (key == t->data)
		/*if (event.isEqual(t->event.getPointEvent())
		 && event.getTypeEventPoint() == t->event.getTypeEventPoint())*/
		if (event.isEqual(t->event.getPointEvent()))
			return;
		if (event.isGreater(t->event.getPointEvent()) && t->right == NULL) {
			//if (key > t->data && t->right == NULL) {
			t->right = p;
			break;
		}
		if (event.isLess(t->event.getPointEvent()) && t->left == NULL) {
			//if (key < t->data && t->left == NULL) {
			t->left = p;
			break;
		}
		if (event.isLess(t->event.getPointEvent())) {
			//if (key < t->data) {
			t = t->left;
		} else
			t = t->right;
	}
	/*Balanceando*/
	t = getnode(event);
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
		t = getparent(t->event);
	}
}

Node * Avltree::getinordersuccessor(Node *t) {
	t = t->right;
	while (t->left != NULL)
		t = t->left;
	return t;
}

void Avltree::remove(EventPoint event) {
	Node *t = getnode(event);
	if (t == NULL)
		return;
	Node *q = getparent(event);
	if (t->left != NULL && t->right != NULL) {
		/*Para reemplazo con el sucesor inorder*/
		Node *y = getinordersuccessor(t);
		q = getparent(y->event);
		t->event = y->event;
		t = y;
	}
	Node *p = NULL;
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
		t = getparent(t->event);
	}
}

int Avltree::height(Node *t) {
	if (t == NULL)
		return 0;
	int hr = height(t->right);
	int hl = height(t->left);
	return 1 + (hr > hl ? hr : hl);
}

int Avltree::balance(Node *t) {
	if (t == NULL)
		return 0;
	return height(t->right) - height(t->left);
}

Node * Avltree::getparent(EventPoint event) {
	Node *t = getnode(event);
	if (t == NULL || t == root)
		return NULL;
	Node *t1 = root;
	while (1) {
		if (t1 == NULL)
			return NULL;
		//if (t->data > t1->data && t->data == t1->right->data)
		if (t->event.isGreater(t1->event.getPointEvent())
				&& t->event.isEqual(t1->right->event.getPointEvent()))
			return t1;
		//if (t->data < t1->data && t->data == t1->left->data)
		if (t->event.isLess(t1->event.getPointEvent())
				&& t->event.isEqual(t1->left->event.getPointEvent()))
			return t1;
		//if (t->data > t1->data)
		if (t->event.isGreater(t1->event.getPointEvent()))
			t1 = t1->right;
		else
			t1 = t1->left;
	}
}

void Avltree::rightrotate(Node *t) {
	if (t == NULL)
		return;
	Node *q = getparent(t->event);
	int i, j = 0;
	if (t == root)
		j++;
	if (q != NULL && q->right == t) {
		i = 1;
	} else {
		i = 0;
	}
	Node *u = t->left;
	if (u == NULL)
		return;
	//node *v=t->right;
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

void Avltree::leftrotate(Node *t) {
	if (t == NULL)
		return;
	Node *q = getparent(t->event);
	int i, j = 0;
	if (t == root)
		j++;
	if (q != NULL && q->right == t) {
		i = 1;
	} else {
		i = 0;
	}
	Node *u = t->right;
	if (u == NULL)
		return;
	//node *v=t->left;
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

Node * Avltree::getnode(EventPoint event) {
	Node *t = root;
	while (1) {
		if (t == NULL)
			return NULL;
		//if (t->data == key)
		if (t->event.isEqual(event.getPointEvent()))
			return t;
		//if (key > t->data)
		if (event.isGreater(t->event.getPointEvent()))
			t = t->right;
		else
			t = t->left;
	}
}

void Avltree::inorder(Node *t) {
	if (t == NULL)
		return;
	inorder(t->left);
	cout << "(" << t->event.getPointEvent().getX() << ","
			<< t->event.getPointEvent().getY() << "),";
	inorder(t->right);
}

void Avltree::preorder(Node *t) {
	if (t == NULL)
		return;
	cout << "(" << t->event.getPointEvent().getX() << ","
			<< t->event.getPointEvent().getY() << "),";
	preorder(t->left);
	preorder(t->right);
}

void Avltree::postorder(Node *t) {
	if (t == NULL)
		return;
	postorder(t->left);
	postorder(t->right);
	cout << "(" << t->event.getPointEvent().getX() << ","
			<< t->event.getPointEvent().getY() << "),";
}

Node * Avltree::getMax(Node *ta) {
	if (ta == NULL)
		return NULL;
	Node * node = NULL;
	node = getMax(ta->right);
	if (node == NULL) {
		node = ta;
	}
	return node;
}
