/*
 * Node.h
 *
 *  Created on: 27/02/2015
 *      Author: rey
 */
#include "../../model/Point.h"
#include "../../model/Segment.h"

#ifndef SRC_StatusNode_H_
#define SRC_StatusNode_H_

class StatusNode {
public:
	Segment key;
	StatusNode *left, *right;
	int bf;
	StatusNode(Segment segment);
	StatusNode();
	virtual ~StatusNode();
};

#endif /* SRC_StatusNode_H_ */
