/*
 * Node.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "Node.h"
#include "stddef.h"

Node::~Node() {
	// TODO Auto-generated destructor stub
}

Node::Node(EventPoint d) {
	event = d;
	left = NULL;
	right = NULL;
	bf = 0;
}

Node::Node() {
	event = EventPoint();
	left = NULL;
	right = NULL;
	bf = 0;
}

