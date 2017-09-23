/*
 * Node.h
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#ifndef SRC_NODE_H_
#define SRC_NODE_H_

class Node {
public:
	int data;
	Node *left, *right;
	int bf;
	Node(int);
	Node();
	virtual ~Node();
};

#endif /* SRC_NODE_H_ */
