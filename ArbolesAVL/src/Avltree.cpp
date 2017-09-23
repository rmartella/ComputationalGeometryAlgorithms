/*
 * Avltree.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Avltree.h"

Avltree::~Avltree() {
	// TODO Auto-generated destructor stub
}

Avltree::Avltree() {
	root = NULL;
	ac = 0;
}

void Avltree::insert(int key) {
	Node *t = root;
	Node *p = new Node(key);
	if (root == NULL) {
		root = p;
		return;
	}
	while (1) {
		if (key == t->data)
			return;
		if (key > t->data && t->right == NULL) {
			t->right = p;
			break;
		}
		if (key < t->data && t->left == NULL) {
			t->left = p;
			break;
		}
		if (key < t->data) {
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
		t = getparent(t->data);
	}
}

Node * Avltree::getinordersuccessor(Node *t) {
	t = t->right;
	while (t->left != NULL)
		t = t->left;
	return t;
}

void Avltree::remove(int key) {
	Node *t = getnode(key);
	if (t == NULL)
		return;
	Node *q = getparent(key);
	if (t->left != NULL && t->right != NULL) {
		/*Para reemplazo con el sucesor inorder*/
		Node *y = getinordersuccessor(t);
		q = getparent(y->data);
		t->data = y->data;
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
		t = getparent(t->data);
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

Node * Avltree::getparent(int key) {
	Node *t = getnode(key);
	if (t == NULL || t == root)
		return NULL;
	Node *t1 = root;
	while (1) {
		if (t1 == NULL)
			return NULL;
		if (t->data > t1->data && t->data == t1->right->data)
			return t1;
		if (t->data < t1->data && t->data == t1->left->data)
			return t1;
		if (t->data > t1->data)
			t1 = t1->right;
		else
			t1 = t1->left;
	}
}

void Avltree::rightrotate(Node *t) {
	if (t == NULL)
		return;
	Node *q = getparent(t->data);
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
	Node *q = getparent(t->data);
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

Node * Avltree::getnode(int key) {
	Node *t = root;
	while (1) {
		if (t == NULL)
			return NULL;
		if (t->data == key)
			return t;
		if (key > t->data)
			t = t->right;
		else
			t = t->left;
	}
}

void Avltree::inorder(Node *t) {
	if (t == NULL)
		return;
	inorder(t->left);
	cout << "  " << t->data;
	inorder(t->right);
}

void Avltree::preorder(Node *t) {
	if (t == NULL)
		return;
	cout << "  " << t->data;
	preorder(t->left);
	preorder(t->right);
}

void Avltree::postorder(Node *t) {
	if (t == NULL)
		return;
	postorder(t->left);
	postorder(t->right);
	cout << "  " << t->data;
}

