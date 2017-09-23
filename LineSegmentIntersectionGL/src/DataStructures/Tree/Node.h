/*
 * Node.h
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */
#include "../../model/EventPoint.h"

#ifndef SRC_NODE_H_
#define SRC_NODE_H_

class Node {
public:
	EventPoint event;
	Node *left, *right;
	int bf;
	Node(EventPoint event);
	Node();
	virtual ~Node();
};

#endif /* SRC_NODE_H_ */
