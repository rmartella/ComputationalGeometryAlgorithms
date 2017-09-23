/*
 * Node.cpp
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */

#include "StatusNode.h"
#include "stddef.h"

StatusNode::~StatusNode() {
	// TODO Auto-generated destructor stub
}

StatusNode::StatusNode(Segment segment) {
	key = segment;
	left = NULL;
	right = NULL;
	bf = 0;
}

StatusNode::StatusNode() {
	key = Segment();
	left = NULL;
	right = NULL;
	bf = 0;
}

